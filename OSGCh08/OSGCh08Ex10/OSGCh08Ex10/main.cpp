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
#include "PickHandler.h"

int main(int argc, char** argv)
{
	//创建Viewer对象，场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取地形模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");

	osg::ref_ptr<osgText::Text> updatetext = new osgText::Text();
	CreateHUD *hudText = new CreateHUD();

	//添加到场景
	root->addChild(node.get());
	root->addChild(hudText->createHUD(updatetext.get()));

	//添加PICK事件处理器
	viewer->addEventHandler(new CPickHandler(updatetext.get()));

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}