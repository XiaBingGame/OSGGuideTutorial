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

// ÀÈ∆¨
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/FireEffect>

#include <osgViewer/Viewer>
#include <iostream>

osg::ref_ptr<osg::Node> createExplode()
{
	osg::ref_ptr<osg::Group> explode = new osg::Group();

	osg::Vec3 wind(1.0, 0.0, 0.0);
	osg::Vec3 position(0.0, 0.0, -10.0);

	osg::ref_ptr<osgParticle::ExplosionEffect> explosion = new osgParticle::ExplosionEffect(position, 10.0f);
	osg::ref_ptr<osgParticle::ExplosionDebrisEffect> explosionDebris = new osgParticle::ExplosionDebrisEffect(position, 10.0f);
	osg::ref_ptr<osgParticle::SmokeEffect> smoke = new osgParticle::SmokeEffect(position, 10.0f);
	osg::ref_ptr<osgParticle::FireEffect> fire = new osgParticle::FireEffect(position, 10.0f);

	explosion->setWind(wind);
	explosionDebris->setWind(wind);
	smoke->setWind(wind);
	fire->setWind(wind);

	explode->addChild(explosion);
	explode->addChild(explosionDebris);
	explode->addChild(fire);
	explode->addChild(smoke);

	return explode.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(createExplode());

	osgUtil::Optimizer optimizer;
	viewer->setUpViewInWindow(100, 100, 800, 600);
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}