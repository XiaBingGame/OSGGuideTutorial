#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Scissor>

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

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//创建一个裁剪面
	osg::ref_ptr<osg::Scissor> cc = new osg::Scissor();
	//设置裁剪面矩形信息(左下角坐标、宽度和高度)
	cc->setScissor(150, 150, 800, 600);

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());

	//启用裁剪平面
	root->getOrCreateStateSet()->setAttributeAndModes(cc.get(), osg::StateAttribute::ON);

	//优化场景数据
	osgUtil::Optimizer optimzier;
	optimzier.optimize(root.get());
	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}