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
#include <osgGA/AnimationPathManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>
#include "ManipulatorTravel.h"

int main(int argc, char** argv)
{
	//创建Viewer对象，场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	viewer->setUpViewInWindow(50, 50, 800, 600);
	//把漫游器加入到场景中
	TravelManipulator::TravelToScene(viewer.get());

	osg::ref_ptr<osg::Group> root = new osg::Group();
	//读取地形模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");
	//添加到场景
	root->addChild(node.get());
	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}