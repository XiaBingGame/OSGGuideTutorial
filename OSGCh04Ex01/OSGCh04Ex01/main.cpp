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

osg::ref_ptr<osg::Node> createQuad()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	v->push_back(osg::Vec3(0.0, 0.0, 0.0));
	v->push_back(osg::Vec3(1.0, 0.0, 0.0));
	v->push_back(osg::Vec3(1.0, 0.0, 1.0));
	v->push_back(osg::Vec3(0.0, 0.0, 1.0));

	geom->setVertexArray(v.get());

	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;
	t->push_back(osg::Vec2(0.0, 0.0));
	t->push_back(osg::Vec2(1.0, 0.0));
	t->push_back(osg::Vec2(1.0, 1.0));
	t->push_back(osg::Vec2(0.0, 1.0));

	geom->setTexCoordArray(0, t.get());
	
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array;
	vc->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	vc->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	vc->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	vc->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));

	geom->setColorArray(vc.get());

	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array;
	nc->push_back(osg::Vec3(0.0, -1.0, 0.0));
	geom->setNormalArray(nc);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	geode->addDrawable(geom.get());

	return geode.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//添加到场景
	root->addChild(createQuad());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}