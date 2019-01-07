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
	//����CompositeViewer����
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();

	//��ȡţ��ģ��
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");

	//��ȡ�ɻ�ģ��
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(cow.get());
	optimizer.optimize(cessna.get());

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 100;
	traits->y = 100;
	traits->width = 900;
	traits->height = 700;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc->valid())
	{
		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

		//���������ɫ�������ɫ����Ȼ���
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}

	//��ͼһ
	{
		//������ͼһ
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
		viewer->addView(view.get());

		//������ͼ��������
		view->setSceneData(cow.get());

		//��������ӿڼ�ͼ�λ���
		// ÿ�� view �����������
		view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width / 2, traits->height / 2));
		view->getCamera()->setGraphicsContext(gc.get());

		//���ò�����
		view->setCameraManipulator(new osgGA::TrackballManipulator);

		//����¼�����
		view->addEventHandler(new osgViewer::StatsHandler);
		view->addEventHandler(new osgViewer::WindowSizeHandler);
		view->addEventHandler(new osgViewer::ThreadingHandler);
		view->addEventHandler(new osgViewer::RecordCameraPathHandler);
	}

	//��ͼ��
	{
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
		viewer->addView(view.get());

		view->setSceneData(cessna.get());

		view->getCamera()->setViewport(new osg::Viewport(traits->width / 2, 0, traits->width / 2, traits->height / 2));
		view->getCamera()->setGraphicsContext(gc.get());

		view->setCameraManipulator(new osgGA::TrackballManipulator);
	}

	//��ͼ��
	{
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
		viewer->addView(view.get());

		view->setSceneData(cessna.get());

		//���ݷֱ�����ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β�����
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