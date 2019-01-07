#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>
#include <osgUtil/DelaunayTriangulator>
#include <osgGA/StateSetManipulator>

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
#include "Tex.h"

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//方便查看在多边形之间切换，以查看三角网
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建顶点数组
	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array();

	//计算顶点数组的大小
	unsigned int n = sizeof(vertex) / sizeof(float[3]);

	//添加顶点数据
	for (unsigned int i = 0; i < n; i++)
	{
		coords->push_back(osg::Vec3(vertex[i][0], vertex[i][1], vertex[i][2]));
	}

	//创建Delaunay三角网对象
	osg::ref_ptr<osgUtil::DelaunayTriangulator> dt = new osgUtil::DelaunayTriangulator(coords.get());
	//生成三角网
	dt->triangulate();

	//创建几何体
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	//设置顶点数组
	geometry->setVertexArray(coords.get());

	//加入到绘图基元
	geometry->addPrimitiveSet(dt->getTriangles());

	//添加到叶节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geometry.get());

	root->addChild(geode.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}