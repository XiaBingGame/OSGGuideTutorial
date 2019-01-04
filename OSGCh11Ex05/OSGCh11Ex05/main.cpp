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
#include <osgParticle/SinkOperator>
#include <osgParticle/BounceOperator>

#include <osgViewer/Viewer>
#include <iostream>

osg::ref_ptr<osg::Group> createMyParticleScene()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	// 创建粒子系统模板
	osgParticle::Particle ptemplate;
	ptemplate.setLifeTime(8);
	ptemplate.setSizeRange(osgParticle::rangef(1.0f, 1.0f));
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, 1.0f));
	ptemplate.setColorRange(osgParticle::rangev4(
		osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	ptemplate.setRadius(0.01f);
	ptemplate.setMass(0.02f);
	ptemplate.setShape(osgParticle::Particle::POINT);

	// 粒子系统
	osg::ref_ptr<osgParticle::ParticleSystem> ps = new osgParticle::ParticleSystem;
	// 设置材质
	// ps->setDefaultAttributes("Images/smoke.rgb", false, false);
	ps->setDefaultAttributes("", false, false);
	ps->setDefaultParticleTemplate(ptemplate);

	// 放射器
	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
	emitter->setParticleSystem(ps.get());

	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform();
	//创建一个矩阵
	osg::Matrix m;
	m.makeRotate(3.1415 * 0.5, 0.0f, 1.0f, 0.0f);
	//设置矩阵
	mt1->setMatrix(m);
	//添加子节点
	mt1->addChild(emitter);

	// 计数器
	osg::ref_ptr<osgParticle::RandomRateCounter> counter = new osgParticle::RandomRateCounter;
	// 每秒粒子添加的数量
	counter->setRateRange(1600.0f, 1600.0f);
	emitter->setCounter(counter.get());

	// 放置器
	osg::ref_ptr<osgParticle::PointPlacer> placer = new osgParticle::PointPlacer;
	placer->setCenter(osg::Vec3(0.0, 0.0, 0.0));
	emitter->setPlacer(placer.get());

	// 发射器
	osg::ref_ptr<osgParticle::RadialShooter> shooter = new osgParticle::RadialShooter;
	shooter->setInitialSpeedRange(60, 60);
	
	emitter->setShooter(shooter.get());

	// root->addChild(emitter.get());
	root->addChild(mt1);

	// 标准编程对象
	osg::ref_ptr<osgParticle::ModularProgram> program = new osgParticle::ModularProgram;
	program->setParticleSystem(ps.get());

	// 操作器
	osg::ref_ptr<osgParticle::AccelOperator> ap = new osgParticle::AccelOperator;
	ap->setToGravity(5.0f);
	program->addOperator(ap.get());
	osg::ref_ptr<osgParticle::FluidFrictionOperator> ffo = new osgParticle::FluidFrictionOperator;
	ffo->setFluidToAir();
	program->addOperator(ffo.get());

	osg::ref_ptr<osgParticle::BounceOperator> bounce = new osgParticle::BounceOperator;
	bounce->setFriction(-0.001);
	bounce->setResilience(0.35);
	//bounce->addDiskDomain(osg::Vec3(0.0f, 0.0f, -2.0f), osg::Z_AXIS, 8.0f);
	bounce->addPlaneDomain(osg::Plane(osg::Z_AXIS, 50.0f));


	osg::ref_ptr<osgParticle::SinkOperator> sink = new osgParticle::SinkOperator;
	sink->setSinkStrategy(osgParticle::SinkOperator::SINK_OUTSIDE);
	// sink->addSphereDomain(osg::Vec3(), 20.0f);
	sink->addPlaneDomain(osg::Plane(0.0, 0.0, 1.0, 50.0));
	program->addOperator(sink.get());
	program->addOperator(bounce.get());

	root->addChild(program.get());

	osg::ref_ptr<osgParticle::ParticleSystemUpdater> psu = new osgParticle::ParticleSystemUpdater;
	psu->addParticleSystem(ps.get());

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(ps.get());

	root->addChild(geode.get());
	root->addChild(psu.get());

	root->getOrCreateStateSet()->setMode(GL_POINT_SMOOTH, osg::StateAttribute::ON);

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

	viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	viewer->getCameraManipulator()->setHomePosition(osg::Vec3(0.0, -800.0, 0.0), osg::Vec3(0.0, 0.0, 0.0),
		osg::Vec3(0.0, 0.0, 1.0));

	viewer->realize();

	viewer->run();
	return 0;
}