#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Fog>

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

osg::ref_ptr<osg::Fog> createFog(bool m_Linear)
{
	osg::ref_ptr<osg::Fog> fog = new osg::Fog;
	fog->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	fog->setDensity(0.01f);
	if (!m_Linear)
	{
		fog->setMode(osg::Fog::LINEAR);
	}
	else
	{
		fog->setMode(osg::Fog::EXP);
	}
	fog->setStart(5.0f);
	fog->setEnd(2000.0f);

	return fog.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");

	root->addChild(node.get());
	root->getOrCreateStateSet()->setAttributeAndModes(createFog(true), osg::StateAttribute::ON);

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}