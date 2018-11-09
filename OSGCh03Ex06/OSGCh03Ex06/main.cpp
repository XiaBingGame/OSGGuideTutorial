#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

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

	//创建一个节点，读取一个牛的模型
	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("cow.osg");

	//创建一个节点，读取滑翔机模型
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	//创建一个细节层次LOD节点
	osg::ref_ptr<osg::LOD> lode = new osg::LOD();
	//添加子节点，在0-30范围内显示牛
	lode->addChild(node1.get(), 0.0f, 30.0f);
	//添加子节点，在30-100范围内显示滑翔机
	lode->addChild(node2.get(), 30.0f, 100.0f);

	//写入lode.osg文件
	osgDB::writeNodeFile(*(lode.get()), "lode.osg");

	//添加到场景
	root->addChild(lode.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}