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

//创建三级PagedLOD场景
osg::ref_ptr<osg::Group> createPagedLOD()
{
	//创建PagedLOD对象
	osg::ref_ptr<osg::PagedLOD> page = new osg::PagedLOD();
	//设置中心位置
	page->setCenter(osg::Vec3(0.0f, 0.0f, 0.0f));

	//添加节点，设置0级的内容为cow.osg
	page->setFileName(0, "cow.osg");
	//设置可视变化范围为0.0f-50.0f
	page->setRange(0, 0.0f, 50.0f);

	//添加节点，设置0级的内容为spaceship.osg
	page->setFileName(1, "glider.osg");
	//设置可视变化范围50.0f-100.0f
	page->setRange(1, 50.0f, 100.0f);

	//添加节点，设置0级的内容为cessna.osg
	page->setFileName(2, "cessna.osg");
	//设置可视变化范围100.0f-200.0f
	page->setRange(2, 100.0f, 200.0f);

	return page.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建矩阵变换节点
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	//添加子节点
	mt->addChild(createPagedLOD());
	//旋转一下，调整一下合适的角度
	osg::Matrix m;
	m.makeRotate(60.0f, 0.0f, 0.0f, 1.0f);
	//设置矩阵
	mt->setMatrix(m);

	//添加PagedLOD场景
	root->addChild(mt.get());

	//写入PagedLOD
	osgDB::writeNodeFile(*root, "page.osg");

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();
	return 0;
}