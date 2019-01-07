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
	//创建Viewer对象，场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建一个节点,读取牛的模型
	osg::ref_ptr<osg::Node> node1 = new osg::Node();
	node1 = osgDB::readNodeFile("cow.osg");

	//创建一个节点，读取滑翔机模型
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	//创建一个开关节点，只渲染滑翔机而不渲染牛
	osg::ref_ptr<osg::Switch> swtich = new osg::Switch();
	//添加牛的模型，但不渲染，默认为true
	swtich->addChild(node1.get(), false);
	//添加滑翔机模型，状态为渲染
	swtich->addChild(node2.get(), true);

	root->addChild(swtich.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}