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
#include <osgViewer/ViewerEventHandlers>
#include <iostream>

int main(int argc, char** argv)
{
	//创建CompositeViewer对象
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();

	//读取牛的模型
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");

	//读取飞机模型
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(cow.get());
	optimizer.optimize(cessna.get());

	//设置图形环境特性
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 100;
	traits->y = 100;
	traits->width = 900;
	traits->height = 700;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//创建图形环境特性
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc->valid())
	{
		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

		//清除窗口颜色及清除颜色和深度缓冲
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}

	//视图一
	{
		//创建视图一
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
		viewer->addView(view.get());

		//设置视图场景数据
		view->setSceneData(cow.get());

		//设置相机视口及图形环境
		// 每个 view 都有它的相机
		view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width / 2, traits->height / 2));
		view->getCamera()->setGraphicsContext(gc.get());

		//设置操作器
		view->setCameraManipulator(new osgGA::TrackballManipulator);

		//添加事件处理
		view->addEventHandler(new osgViewer::StatsHandler);
		view->addEventHandler(new osgViewer::WindowSizeHandler);
		view->addEventHandler(new osgViewer::ThreadingHandler);
		view->addEventHandler(new osgViewer::RecordCameraPathHandler);
	}

	//视图二
	{
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
		viewer->addView(view.get());

		view->setSceneData(cessna.get());

		view->getCamera()->setViewport(new osg::Viewport(traits->width / 2, 0, traits->width / 2, traits->height / 2));
		view->getCamera()->setGraphicsContext(gc.get());

		view->setCameraManipulator(new osgGA::TrackballManipulator);
	}

	//视图三
	{
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
		viewer->addView(view.get());

		view->setSceneData(cessna.get());

		//根据分辨率来确定合适的投影来保证显示的图形不变形
		double fovy, aspectRatio, zNear, zFar;
		view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		double newAspectRatio = double(traits->width) / double(traits->height / 2);
		double aspectRatioChange = newAspectRatio / aspectRatio;
		if (aspectRatioChange != 1.0)
		{
			view->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
		}

		view->getCamera()->setViewport(new osg::Viewport(0, traits->height / 2, traits->width, traits->height / 2));
		view->getCamera()->setGraphicsContext(gc.get());

		view->setCameraManipulator(new osgGA::TrackballManipulator);
	}

	viewer->realize();

	viewer->run();

	return 0;
}