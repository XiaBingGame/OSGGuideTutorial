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
#include <osgViewer/CompositeViewer>
#include <iostream>

int main(int argc, char** argv)
{
	//创建一个CompositeViewer对象
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();

	//创建两个视图
	osg::ref_ptr<osgViewer::View> viewer1 = new osgViewer::View();
	osg::ref_ptr<osgViewer::View> viewer2 = new osgViewer::View();

	int xoffset = 50;
	int yoffset = 200;

	//视图一的节点
	osg::ref_ptr<osg::Node> viewer1Node = osgDB::readNodeFile("cessna.osg");

	//视图二的节点
	osg::ref_ptr<osg::Node> viewer2Node = osgDB::readNodeFile("cow.osg");

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(viewer1Node.get());
	optimizer.optimize(viewer2Node.get());

	//视图窗口一
	{
		//设置图形环境特性
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
		traits->x = xoffset + 0;
		traits->y = yoffset + 0;
		traits->width = 600;
		traits->height = 480;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		//创建图形环境
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		//创建相机
		osg::ref_ptr<osg::Camera> camera1 = viewer1->getCamera();
		//设置图形环境
		camera1->setGraphicsContext(gc.get());
		//设置视口
		camera1->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		//设置缓冲
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera1->setDrawBuffer(buffer);
		camera1->setReadBuffer(buffer);

		//设置场景数据
		viewer1->setSceneData(viewer1Node.get());
	}

	//视图窗口二
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
		traits->x = xoffset + 600;
		traits->y = yoffset + 0;
		traits->width = 600;
		traits->height = 480;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera2 = viewer2->getCamera();
		camera2->setGraphicsContext(gc.get());
		camera2->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera2->setDrawBuffer(buffer);
		camera2->setReadBuffer(buffer);

		viewer2->setSceneData(viewer2Node.get());
	}

	//添加视图
	viewer->addView(viewer1.get());
	viewer->addView(viewer2.get());

	viewer->realize();

	viewer->run();

	return 0;
}