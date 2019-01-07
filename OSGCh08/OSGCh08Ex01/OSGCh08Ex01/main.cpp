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

	//����һ���ü�ƽ��
	osg::ref_ptr<osg::ClipPlane> cp1 = new osg::ClipPlane();
	//���òü�ƽ��
	cp1->setClipPlane(0, 1, 1, 1);
	//ָ��ƽ������
	cp1->setClipPlaneNum(0);

	//����һ���ü�ƽ��
	osg::ref_ptr<osg::ClipPlane> cp2 = new osg::ClipPlane();
	//���òü�ƽ��
	cp2->setClipPlane(1, 0, 0, 1);
	//ָ��ƽ������
	cp2->setClipPlaneNum(1);

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());

	//���òü�ƽ��
	root->getOrCreateStateSet()->setAttributeAndModes(cp1.get(), osg::StateAttribute::ON);
	root->getOrCreateStateSet()->setAttributeAndModes(cp2.get(), osg::StateAttribute::ON);

	//�Ż���������
	osgUtil::Optimizer optimzier;
	optimzier.optimize(root.get());

	viewer->setUpViewInWindow(50, 50, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}