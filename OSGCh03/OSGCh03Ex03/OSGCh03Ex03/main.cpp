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

int main(int argc, char** argv)
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//����������ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ���ڵ�,��ȡţ��ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	//��������任�ڵ�mt1
	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform();
	//����һ������
	osg::Matrix m;
	//��X����ƽ��10����λ
	m.makeTranslate(osg::Vec3(10.0f, 0.0f, 0.0f));
	//��X����ת45��
	m.makeRotate(45.0f, 1.0f, 0.0f, 0.0f);
	//���þ���
	mt1->setMatrix(m);
	//����ӽڵ�
	mt1->addChild(node.get());

	//��������任�ڵ�mt2
	osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform();
	//����һ������
	osg::Matrix t;
	//��X��������ƽ��10����λ
	t.makeTranslate(osg::Vec3(-10.0f, 0.0f, 0.0f));
	//���þ���
	mt2->setMatrix(t);
	//����ӽڵ�
	mt2->addChild(node.get());

	//��ӵ�����
	root->addChild(mt1.get());
	root->addChild(mt2.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	//���ó�������
	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);

	//��ʼ������������
	viewer->realize();

	//��ʼ��Ⱦ
	viewer->run();
	return 0;
}