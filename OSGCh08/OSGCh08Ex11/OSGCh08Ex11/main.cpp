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

//对象选取事件处理器
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
	//事件处理函数
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*>(&aa);
		if (!view) return false;

		switch (ea.getEventType())
		{
			//鼠标按下
		case(osgGA::GUIEventAdapter::PUSH):
		{
			//更新鼠标位置
			_mx = ea.getX();
			_my = ea.getY();
			break;
		}
		case(osgGA::GUIEventAdapter::RELEASE):
		{
			if (_mx == ea.getX() && _my == ea.getY())
			{
				//执行对象选取
				pick(view.get(), ea.getX(), ea.getY());
			}
			break;
		}
		default:
			break;
		}
		return false;
	}
	//对象选取事件处理器
	void pick(osg::ref_ptr<osgViewer::View> view, float x, float y)
	{
		osg::ref_ptr<osg::Node> node = new osg::Node();
		osg::ref_ptr<osg::Group> parent = new osg::Group();
		//创建一个线段交集检测函数
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (view->computeIntersections(x, y, intersections))
		{
			osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
			osg::NodePath& nodePath = intersection.nodePath;
			//得到选择的物体
			node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
			parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
		}

		//用一种高亮显示来显示物体已经被选中
		if (parent.get() && node.get())
		{
			osg::ref_ptr<osgFX::Scribe> parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent.get());
			if (!parentAsScribe)
			{
				//如果对象选择到，高亮显示
				osg::ref_ptr<osgFX::Scribe> scribe = new osgFX::Scribe();
				scribe->addChild(node.get());
				parent->replaceChild(node.get(), scribe.get());
			}
			else
			{
				//如果没有没有选择到，则移除高亮显示的对象
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
	//得到鼠标的位置
	float _mx;
	float _my;

};

int main(int argc, char** argv)
{
	//创建Viewer对象，场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->addEventHandler(new PickHandler());

	//创建场景组节点
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建一个节点，读取牛的模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	//添加到场景
	root->addChild(node.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}