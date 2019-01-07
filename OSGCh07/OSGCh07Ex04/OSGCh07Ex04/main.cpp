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

class SpaceShipCallback : public osg::NodeCallback
{
public:
	SpaceShipCallback() : angle(0) {}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::MatrixTransform> mtCow = dynamic_cast<osg::MatrixTransform*>(node);
		osg::Matrix mr;
		mr.makeRotate(angle, osg::Vec3(0.0, 0.0, 1.0));
		mtCow->setMatrix(mr);

		angle += 0.01;
		traverse(node, nv);
	}

public:
	double angle;
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取奶牛模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;

	mt->setUpdateCallback(new SpaceShipCallback);
	mt->addChild(node.get());
	root->addChild(mt);

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}