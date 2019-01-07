#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/EventVisitor>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>


class MyEventCallback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		// �жϷ���������
		if (nv->getVisitorType() == osg::NodeVisitor::EVENT_VISITOR)
		{
			// ��ȡ�¼�������
			osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
			if (ev)
			{
				// ִ�ж���
				osgGA::GUIActionAdapter* aa = ev->getActionAdapter();
				// �¼�����
				osgGA::EventQueue::Events& events = ev->getEvents();

				osgGA::EventQueue::Events::iterator it = events.begin();
				while (it != events.end())
				{
					handle((osgGA::GUIEventAdapter&)(*(*it)), *aa);
					it++;
				}
			}
		}
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&(const_cast<osgGA::GUIActionAdapter&>(aa)));
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(viewer->getSceneData());

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
			{
				if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)
				{
					osg::Matrix mT;
					mT.makeTranslate(0.0, 0.0, 1.0);
					mt->setMatrix(mT);
				}
				if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)
				{
					osg::Matrix mT;
					mT.makeTranslate(0.0, 0.0, -1.0);
					mt->setMatrix(mT);
				}
			}
			break;
		default:
			break;
		}
		return false;
	}
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//����ţ��ģ�ͣ�ע����Ҫ����osg::MatrixTransform����任����
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->addChild(node);

	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

	camera->setEventCallback(new MyEventCallback());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(mt.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(mt.get());

	viewer->realize();

	viewer->run();
	return 0;
}