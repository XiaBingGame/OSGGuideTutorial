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

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ���ڵ�,��ȡţ��ģ��
	osg::ref_ptr<osg::Node> node1 = new osg::Node();
	node1 = osgDB::readNodeFile("cow.osg");

	//����һ���ڵ㣬��ȡ�����ģ��
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	//����һ�����ؽڵ㣬ֻ��Ⱦ�����������Ⱦţ
	osg::ref_ptr<osg::Switch> swtich = new osg::Switch();
	//���ţ��ģ�ͣ�������Ⱦ��Ĭ��Ϊtrue
	swtich->addChild(node1.get(), false);
	//��ӻ����ģ�ͣ�״̬Ϊ��Ⱦ
	swtich->addChild(node2.get(), true);

	root->addChild(swtich.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}