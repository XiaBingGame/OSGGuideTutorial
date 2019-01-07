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
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");

	if (image.get())
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(image.get());

		osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
		texgen->setMode(osg::TexGen::SPHERE_MAP);

		osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		texenv->setColor(osg::Vec4(0.6, 0.6, 0.6, 0.0));

		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1, texture.get(), osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1, texgen.get(), osg::StateAttribute::ON);
		stateset->setTextureAttribute(1,texenv.get());
		node->setStateSet(stateset.get());
	}

	root->addChild(node.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}