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

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

#include <osgViewer/Viewer>
#include <iostream>

osg::ref_ptr<osg::Group> createMyParticleScene()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	// 创建粒子系统模板
	osgParticle::Particle ptemplate;
	ptemplate.setLifeTime(2);
	ptemplate.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, 1.0f));
	ptemplate.setColorRange(osgParticle::rangev4(
		osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	ptemplate.setRadius(0.05f);
	ptemplate.setMass(0.05f);

	// 粒子系统
	osg::ref_ptr<osgParticle::ParticleSystem> ps = new osgParticle::ParticleSystem;
	// 设置材质
	// ps->setDefaultAttributes("Images/smoke.rgb", false, false);
	ps->setDefaultAttributes("", false, false);
	ps->setDefaultParticleTemplate(ptemplate);

	// 放射器
	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
	emitter->setParticleSystem(ps.get());

	// 计数器
	osg::ref_ptr<osgParticle::RandomRateCounter> counter = new osgParticle::RandomRateCounter;
	// 每秒粒子添加的数量
	counter->setRateRange(100.0f, 100.0f);
	emitter->setCounter(counter.get());

	// 放置器
	osg::ref_ptr<osgParticle::PointPlacer> placer = new osgParticle::PointPlacer;
	placer->setCenter(osg::Vec3(0.0, 0.0, 0.0));
	emitter->setPlacer(placer.get());

	// 发射器
	osg::ref_ptr<osgParticle::RadialShooter> shooter = new osgParticle::RadialShooter;
	shooter->setInitialSpeedRange(100, 0);
	emitter->setShooter(shooter.get());

	root->addChild(emitter.get());

	// 标准编程对象
	osg::ref_ptr<osgParticle::ModularProgram> program = new osgParticle::ModularProgram;
	program->setParticleSystem(ps.get());

	// 操作器
	osg::ref_ptr<osgParticle::AccelOperator> ap = new osgParticle::AccelOperator;
	ap->setToGravity(1.0f);
	// program->addOperator(ap.get());
	osg::ref_ptr<osgParticle::FluidFrictionOperator> ffo = new osgParticle::FluidFrictionOperator;
	ffo->setFluidToAir();
	// program->addOperator(ffo.get());

	// root->addChild(program.get());

	osg::ref_ptr<osgParticle::ParticleSystemUpdater> psu = new osgParticle::ParticleSystemUpdater;
	psu->addParticleSystem(ps.get());

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(ps.get());

	root->addChild(geode.get());
	root->addChild(psu.get());

	return root.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setUpViewInWindow(100, 100, 800, 600);
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//自定义粒子系统加入场景
	root->addChild(createMyParticleScene());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}