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
#include <iostream>

//�򳡾�����ӹ�Դ
osg::ref_ptr<osg::Group> createLight(osg::ref_ptr<osg::Node> node)
{
	osg::ref_ptr<osg::Group> lightRoot = new osg::Group();
	lightRoot->addChild(node);

	//��������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = lightRoot->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON);

	//�����Χ��
	osg::BoundingSphere bs;
	node->computeBound();
	bs = node->getBound();

	//����һ��Light����
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(0);
	//���÷���
	light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	//����λ��
	light->setPosition(osg::Vec4(bs.center().x(), bs.center().y(), bs.center().z() + bs.radius(), 1.0f));
	//���û��������ɫ
	light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//����ɢ������ɫ
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//���ú�˥��ָ��
	light->setConstantAttenuation(1.0f);
	//��������˥��ָ��
	light->setLinearAttenuation(0.0f);
	//���ö��η�˥��ָ��
	light->setQuadraticAttenuation(0.0f);

	//������Դ
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());

	lightRoot->addChild(lightSource.get());

	return lightRoot.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	//�򳡾�����ӹ�Դ
	root->addChild(createLight(node.get()));

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();
	return 0;
}