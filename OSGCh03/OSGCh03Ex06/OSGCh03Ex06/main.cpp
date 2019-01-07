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

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

int main(int argc, char** argv)
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ���ڵ㣬��ȡһ��ţ��ģ��
	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("cow.osg");

	//����һ���ڵ㣬��ȡ�����ģ��
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	//����һ��ϸ�ڲ��LOD�ڵ�
	osg::ref_ptr<osg::LOD> lode = new osg::LOD();
	//����ӽڵ㣬��0-30��Χ����ʾţ
	lode->addChild(node1.get(), 0.0f, 30.0f);
	//����ӽڵ㣬��30-100��Χ����ʾ�����
	lode->addChild(node2.get(), 30.0f, 100.0f);

	//д��lode.osg�ļ�
	osgDB::writeNodeFile(*(lode.get()), "lode.osg");

	//��ӵ�����
	root->addChild(lode.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}