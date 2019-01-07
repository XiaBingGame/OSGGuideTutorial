#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/StateSetManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

int main(int argc, char** argv)
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//�л�����ģʽ������Ƚ�ģ�͵�����
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//�����������ʣ���������Խ�󣬼�Խ��
	//��������ԽС����Խ��
	float sampleRatio = 0.3f;
	//���õ��������
	float maxError = 4.0f;

	//�����򻯶���
	osgUtil::Simplifier simplifier(sampleRatio, maxError);

	//��ȡţ��ģ��
	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("cow.osg");

	//���ţ��ģ�͵�node2�ڵ�
	osg::ref_ptr<osg::Node> node2 = (osg::Node*)(node1->clone(osg::CopyOp::DEEP_COPY_ALL));

	//����һ��λ�ñ任�ڵ�
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	//����λ��
	pat->setPosition(osg::Vec3(10.0f, 0.0f, 0.0f));
	//����ӽڵ�
	pat->addChild(node2.get());
	//�򻯴���
	pat->accept(simplifier);

	//��ӵ�����
	root->addChild(node1.get());
	root->addChild(pat.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}