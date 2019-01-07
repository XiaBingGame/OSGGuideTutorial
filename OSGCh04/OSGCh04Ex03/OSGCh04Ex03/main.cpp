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

//���ƶ��Ԥ����ļ�����
osg::ref_ptr<osg::Geode> createShape()
{
	//����һ��Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	//���ð뾶�͸߶�
	float radius = 0.8f;
	float height = 1.0f;

	//������ϸ�ȶ��󣬾�ϸ��Խ�ߣ�ϸ�־�Խ��
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	//���þ�ϸ��Ϊ0.5f
	hints->setDetailRatio(1.0f);

	//���һ�����壬��һ��������Ԥ���弸������󣬵ڶ����Ǿ�ϸ�ȣ�Ĭ��Ϊ0
	geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), radius), hints.get()));
	//���һ��������
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(2.0f, 0.0f, 0.0f), 2 * radius), hints.get()));
	//���һ��Բ׶
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(4.0f, 0.0f, 0.0f), radius, height), hints.get()));
	//���һ��Բ����
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(6.0f, 0.0f, 0.0f), radius, height), hints.get()));
	//���һ��̫�ղ�
	geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(8.0f, 0.0f, 0.0f), radius, height), hints.get()));

	return geode.get();
}

int main(int argc, char** argv)
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ӵ�����
	root->addChild(createShape());
	osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode;
	polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	root->getOrCreateStateSet()->setAttribute(polygonMode.get());
	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}