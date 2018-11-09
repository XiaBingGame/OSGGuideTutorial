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

	//创建场景组节点
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建一个节点,读取牛的模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	//创建矩阵变换节点mt1
	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform();
	//创建一个矩阵
	osg::Matrix m;
	//在X方向平移10个单位
	m.makeTranslate(osg::Vec3(10.0f, 0.0f, 0.0f));
	//绕X轴旋转45度
	m.makeRotate(45.0f, 1.0f, 0.0f, 0.0f);
	//设置矩阵
	mt1->setMatrix(m);
	//添加子节点
	mt1->addChild(node.get());

	//创建矩阵变换节点mt2
	osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform();
	//创建一个矩阵
	osg::Matrix t;
	//在X负方向上平移10个单位
	t.makeTranslate(osg::Vec3(-10.0f, 0.0f, 0.0f));
	//设置矩阵
	mt2->setMatrix(t);
	//添加子节点
	mt2->addChild(node.get());

	//添加到场景
	root->addChild(mt1.get());
	root->addChild(mt2.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	//设置场景数据
	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);

	//初始化并创建窗口
	viewer->realize();

	//开始渲染
	viewer->run();
	return 0;
}