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

#include <osgParticle/PrecipitationEffect>

#include <osgViewer/Viewer>
#include <iostream>

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setUpViewInWindow(100, 100, 800, 600);
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osgParticle::PrecipitationEffect> pe = new osgParticle::PrecipitationEffect;
	pe->snow(0.5);
	viewer->getCamera()->setClearColor(pe->getFog()->getColor());

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");
	node->getOrCreateStateSet()->setAttributeAndModes(pe->getFog());
	//使用雾效
	root->addChild(node.get());
	root->addChild(pe.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}