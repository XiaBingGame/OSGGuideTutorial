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


//单窗口多相机渲染
//两个相机的不同仅在于它们所用的视口不同
void singleWindowMultipleCameras(osg::ref_ptr<osgViewer::Viewer> viewer)
{
	//创建窗口系统接口
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}

	//得到窗口分辨率
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	//设置图形环境特性
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//创建图形环境
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	if (gc->valid())
	{
		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

		//确保窗口清除干净
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}

	//得到cameraMaster(主相机)
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//设置图形环境
	cameraMaster->setGraphicsContext(gc.get());

	//根据分辨率来确定合适的投影来保证显示的图形不变形
	double fovy, aspectRatio, zNear, zFar;
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//设置视口
	cameraMaster->setViewport(new osg::Viewport(0, 0, width, height));
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);

	//创建从属相机
	osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	cameraClient->setGraphicsContext(gc.get());
	cameraClient->setViewport(new osg::Viewport(0, 0, 400, 400));
	GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	cameraClient->setDrawBuffer(bufferClient);
	cameraClient->setReadBuffer(bufferClient);

	//添加从属相机
	viewer->addSlave(cameraClient, osg::Matrix::scale(aspectRatio, 1.0, 1.0), osg::Matrix());
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//读取牛的模型
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");

	//启用单视图多相机渲染
	singleWindowMultipleCameras(viewer.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(cow.get());

	viewer->setSceneData(cow.get());

	viewer->realize();

	viewer->run();

	return 0;
}