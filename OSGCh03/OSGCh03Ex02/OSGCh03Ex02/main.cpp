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

int main()
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//����������ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ���ڵ�,��ȡţ��ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	//����λ�ñ任�ڵ�pat1
	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform();
	//����λ��Ϊosg::Vec3(-10.0f,0.0f,0.0f)
	pat1->setPosition(osg::Vec3(-10.0f, 0.0f, 0.0f));
	//��������,��X,Y,Z������Сһ��
	pat1->setScale(osg::Vec3(0.5f, 0.5f, 0.5f));
	//����ӽڵ�
	pat1->addChild(node.get());

	//����λ�ñ任�ڵ�pat2
	osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform();
	//����λ��Ϊosg::Vec3(10.0f,0.0f,0.0f)
	pat2->setPosition(osg::Vec3(10.0f, 0.0f, 0.0f));
	//����ӽڵ�
	pat2->addChild(node.get());

	//��ӵ�����
	root->addChild(pat1.get());
	root->addChild(pat2.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	//���ó�������
	viewer->setSceneData(root.get());
	//��ʼ������������
	viewer->realize();
	//��ʼ��Ⱦ
	viewer->run();

	return 0;
}