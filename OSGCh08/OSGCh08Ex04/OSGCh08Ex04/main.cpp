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

#include <osg/ShapeDrawable>

#include <osgViewer/Viewer>
#include <iostream>


//�����ڶ������Ⱦ
//��������Ĳ�ͬ�������������õ��ӿڲ�ͬ
void singleWindowMultipleCameras(osg::ref_ptr<osgViewer::Viewer> viewer)
{
	//��������ϵͳ�ӿ�
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}

	//�õ����ڷֱ���
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->samples = 4;

	//����ͼ�λ���
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	if (gc->valid())
	{
		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

		//ȷ����������ɾ�
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}

	//�õ�cameraMaster(�����)
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//����ͼ�λ���
	cameraMaster->setGraphicsContext(gc.get());

	//���ݷֱ�����ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β�����
	double fovy, aspectRatio, zNear, zFar;
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//�����ӿ�
	cameraMaster->setViewport(new osg::Viewport(0, 0, width, height));
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//���û�����
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);

	//�����������
	osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	cameraClient->setGraphicsContext(gc.get());
	cameraClient->setViewport(new osg::Viewport(0, 0, 400, 400));
	GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	cameraClient->setDrawBuffer(bufferClient);
	cameraClient->setReadBuffer(bufferClient);

	//��Ӵ������
	viewer->addSlave(cameraClient, osg::Matrix::scale(aspectRatio, 1.0, 1.0), osg::Matrix());
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//��ȡţ��ģ��

	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 2)));

	//���õ���ͼ�������Ⱦ
	singleWindowMultipleCameras(viewer.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(geode.get());

	viewer->setSceneData(geode.get());
	//viewer->setUpViewInWindow(50, 50, 800, 600);

	viewer->realize();

	viewer->run();

	return 0;
}