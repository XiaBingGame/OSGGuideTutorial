#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ClipPlane>

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

	//创建一个裁剪平面
	osg::ref_ptr<osg::ClipPlane> cp1 = new osg::ClipPlane();
	//设置裁剪平面
	cp1->setClipPlane(0, 1, 1, 1);
	//指定平面索引
	cp1->setClipPlaneNum(0);

	//创建一个裁剪平面
	osg::ref_ptr<osg::ClipPlane> cp2 = new osg::ClipPlane();
	//设置裁剪平面
	cp2->setClipPlane(1, 0, 0, 1);
	//指定平面索引
	cp2->setClipPlaneNum(1);

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());

	//启用裁剪平面
	root->getOrCreateStateSet()->setAttributeAndModes(cp1.get(), osg::StateAttribute::ON);
	root->getOrCreateStateSet()->setAttributeAndModes(cp2.get(), osg::StateAttribute::ON);

	//优化场景数据
	osgUtil::Optimizer optimzier;
	optimzier.optimize(root.get());

	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}