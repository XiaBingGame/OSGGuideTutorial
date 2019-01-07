#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>

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

//绘制多个预定义的几何体
osg::ref_ptr<osg::Geode> createShape()
{
	//创建一个叶节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	//设置半径和高度
	float radius = 0.8f;
	float height = 1.0f;

	//创建精细度对象，精细度越高，细分就越多
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	//设置精细度为0.5f
	hints->setDetailRatio(1.0f);

	//添加一个球体，第一个参数是预定义几何体对象，第二个是精细度，默认为0
	geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), radius), hints.get()));
	//添加一个正方体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(2.0f, 0.0f, 0.0f), 2 * radius), hints.get()));
	//添加一个圆锥
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(4.0f, 0.0f, 0.0f), radius, height), hints.get()));
	//添加一个圆柱体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(6.0f, 0.0f, 0.0f), radius, height), hints.get()));
	//添加一个太空舱
	geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(8.0f, 0.0f, 0.0f), radius, height), hints.get()));

	return geode.get();
}

int main(int argc, char** argv)
{
	//创建Viewer对象，场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//添加到场景
	root->addChild(createShape());
	osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode;
	polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	root->getOrCreateStateSet()->setAttribute(polygonMode.get());
	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}