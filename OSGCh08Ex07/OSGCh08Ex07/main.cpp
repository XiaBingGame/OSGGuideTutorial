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
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/GUIEventHandler>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

osg::ref_ptr<osg::Image> g_image = new osg::Image;
int screen_capture_index = 0;

struct CaptureDrawCallback : public osg::Camera::DrawCallback
{
	CaptureDrawCallback(osg::ref_ptr<osg::Image> image)
	{
		_image = image;
	}

	virtual void operator () (const osg::Camera& camera) const
	{
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi =
			osg::GraphicsContext::getWindowingSystemInterface();

		unsigned int width, height;
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		width = 800;
		height = 600;
		_image->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		_image->readPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE);
	}

	osg::ref_ptr<osg::Image> _image;
};

class ImageHandler : public osgGA::GUIEventHandler
{
public:
	ImageHandler() {}
	~ImageHandler() {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) 
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (viewer == NULL)
			return false;

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
			if (ea.getKey() == 'c' || ea.getKey() == 'C')
			{
				char filename[128];
				memset(filename, 0, 128);
				sprintf(filename, "screenshot_%04d.bmp", screen_capture_index++);
				osgDB::writeImageFile(*(g_image.get()), filename);
				return true;
			}
			break;

		default:
			return false;
			break;
		}

		return false;
	}

};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	//设置绘制回调
	viewer->getCamera()->setPostDrawCallback(new CaptureDrawCallback(g_image));
	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	//添加抓图事件
	viewer->addEventHandler(new ImageHandler);

	viewer->realize();
	viewer->run();

	return 0;
}