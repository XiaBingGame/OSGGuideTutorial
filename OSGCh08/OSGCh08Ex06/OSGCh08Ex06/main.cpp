#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>

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
	//����һ��CompositeViewer����
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();

	//����������ͼ
	osg::ref_ptr<osgViewer::View> viewer1 = new osgViewer::View();
	osg::ref_ptr<osgViewer::View> viewer2 = new osgViewer::View();

	int xoffset = 50;
	int yoffset = 200;

	//��ͼһ�Ľڵ�
	//osg::ref_ptr<osg::Node> viewer1Node = osgDB::readNodeFile("cessna.osg");
	//osg::ref_ptr<osg::Node> viewer1Node = osgDB::readNodeFile("cessna.osg");
	//geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 2)));
	osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;
	geode1->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 2)));

	osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
	geode2->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 2)));



	//��ͼ���Ľڵ�
	//osg::ref_ptr<osg::Node> viewer2Node = osgDB::readNodeFile("cow.osg");

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(geode1.get());
	optimizer.optimize(geode2.get());

	//��ͼ����һ
	{
		//����ͼ�λ�������
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
		traits->x = xoffset + 0;
		traits->y = yoffset + 0;
		traits->width = 600;
		traits->height = 480;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 4;

		//����ͼ�λ���
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		//�������
		osg::ref_ptr<osg::Camera> camera1 = viewer1->getCamera();
		//����ͼ�λ���
		camera1->setGraphicsContext(gc.get());
		//�����ӿ�
		camera1->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		//���û���
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera1->setDrawBuffer(buffer);
		camera1->setReadBuffer(buffer);

		//���ó�������
		viewer1->setSceneData(geode1.get());
	}

	//��ͼ���ڶ�
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
		traits->x = xoffset + 600;
		traits->y = yoffset + 0;
		traits->width = 600;
		traits->height = 480;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 4;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera2 = viewer2->getCamera();
		camera2->setGraphicsContext(gc.get());
		camera2->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera2->setDrawBuffer(buffer);
		camera2->setReadBuffer(buffer);

		viewer2->setSceneData(geode2.get());
	}

	//�����ͼ
	viewer->addView(viewer1.get());
	viewer->addView(viewer2.get());

	viewer->realize();

	viewer->run();

	return 0;
}