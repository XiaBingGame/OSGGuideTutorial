#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ClipNode>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>

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

osg::ref_ptr<osg::Node> createClipNode(osg::ref_ptr<osg::Node> subgraph)
{
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//��������λ���ģʽ������ͷ��涼����
	osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode();
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);

	//���ö�������λ���ģʽ����ָ��״̬�̳�����ΪOVERRIDE
	stateset->setAttributeAndModes(polymode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

	//��������λ��ƽڵ�
	osg::ref_ptr<osg::Group> wireframe_subgraph = new osg::Group;
	//������Ⱦ״̬
	wireframe_subgraph->setStateSet(stateset.get());
	wireframe_subgraph->addChild(subgraph.get());

	root->addChild(wireframe_subgraph.get());
	
	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;
	//���»ص���ʵ�ֶ�̬�ü�
	// ����һ������, ���ϸı� transform
	osg::ref_ptr<osg::NodeCallback> nc = new osg::AnimationPathCallback(subgraph->getBound().center(), osg::Vec3(0.0f, 0.0f, 1.0f), osg::inDegrees(45.0f));
	transform->setUpdateCallback(nc.get());
	
	//�����ü��ڵ�
	osg::ref_ptr<osg::ClipNode> clipnode = new osg::ClipNode;
	osg::BoundingSphere bs = subgraph->getBound();
	bs.radius() *= 0.4f;

	//���òü��ڵ�İ�Χ��
	osg::BoundingBox bb;
	bb.expandBy(bs);

	//����ǰ��ָ���İ�Χ�д��������ü�ƽ��
	clipnode->createClipBox(bb);
	//���ü�ѡ
	//clipnode->setCullingActive(false);

	transform->addChild(clipnode.get());

	root->addChild(transform.get());

	// clipNode ���ϵط����任.
	//����δ���ü��Ľڵ�
	osg::ref_ptr<osg::Group> clippedNode = new osg::Group;
	clippedNode->setStateSet(clipnode->getStateSet());
	clippedNode->addChild(subgraph.get());

	root->addChild(clippedNode.get());
	
	return root.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Node> root = new osg::Node();

	//����ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");

	root = createClipNode(node.get());

	//�Ż���������
	osgUtil::Optimizer optimzer;
	optimzer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}