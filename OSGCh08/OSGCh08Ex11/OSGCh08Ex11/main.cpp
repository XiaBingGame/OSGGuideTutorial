#include <Windows.h>
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

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>
#include <osgFX/Scribe>

//����ѡȡ�¼�������
class PickHandler : public osgGA::GUIEventHandler
{
public:

	PickHandler() :
		_mx(0.0f),
		_my(0.0f)
	{
		//
	}

	~PickHandler()
	{
		//
	}
	//�¼�������
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*>(&aa);
		if (!view) return false;

		switch (ea.getEventType())
		{
			//��갴��
		case(osgGA::GUIEventAdapter::PUSH):
		{
			//�������λ��
			_mx = ea.getX();
			_my = ea.getY();
			break;
		}
		case(osgGA::GUIEventAdapter::RELEASE):
		{
			if (_mx == ea.getX() && _my == ea.getY())
			{
				//ִ�ж���ѡȡ
				pick(view.get(), ea.getX(), ea.getY());
			}
			break;
		}
		default:
			break;
		}
		return false;
	}
	//����ѡȡ�¼�������
	void pick(osg::ref_ptr<osgViewer::View> view, float x, float y)
	{
		osg::ref_ptr<osg::Node> node = new osg::Node();
		osg::ref_ptr<osg::Group> parent = new osg::Group();
		//����һ���߶ν�����⺯��
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (view->computeIntersections(x, y, intersections))
		{
			osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
			osg::NodePath& nodePath = intersection.nodePath;
			//�õ�ѡ�������
			node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
			parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
		}

		//��һ�ָ�����ʾ����ʾ�����Ѿ���ѡ��
		if (parent.get() && node.get())
		{
			osg::ref_ptr<osgFX::Scribe> parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent.get());
			if (!parentAsScribe)
			{
				//�������ѡ�񵽣�������ʾ
				osg::ref_ptr<osgFX::Scribe> scribe = new osgFX::Scribe();
				scribe->addChild(node.get());
				parent->replaceChild(node.get(), scribe.get());
			}
			else
			{
				//���û��û��ѡ�񵽣����Ƴ�������ʾ�Ķ���
				osg::Node::ParentList parentList = parentAsScribe->getParents();
				for (osg::Node::ParentList::iterator itr = parentList.begin();
					itr != parentList.end();
					++itr)
				{
					(*itr)->replaceChild(parentAsScribe.get(), node.get());
				}
			}
		}

	}
public:
	//�õ�����λ��
	float _mx;
	float _my;

};

int main(int argc, char** argv)
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->addEventHandler(new PickHandler());

	//����������ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ���ڵ㣬��ȡţ��ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	//��ӵ�����
	root->addChild(node.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}