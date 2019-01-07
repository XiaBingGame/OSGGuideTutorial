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

//创建一个四边形节点
osg::ref_ptr<osg::Node> createQuad()
{
	//创建一个叶节点对象
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	//创建一个几何体对象
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//创建顶点数组
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	//添加数据
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	v->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	//设置顶点数据
	geom->setVertexArray(v.get());

	//创建四边形顶点索引数组，指定绘图基元为四边形，注意添加顺序
	osg::ref_ptr<osg::DrawElementsUInt> quad = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	//添加数据
	quad->push_back(0);
	quad->push_back(1);
	quad->push_back(2);
	quad->push_back(3);

	//添加到几何体
	geom->addPrimitiveSet(quad.get());

	//创建三角形顶点索引数组，指定绘图基元为三角形，注意添加顺序
	osg::ref_ptr<osg::DrawElementsUInt> triangle = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//添加数据
	triangle->push_back(4);
	triangle->push_back(0);
	triangle->push_back(3);

	//添加到几何体
	geom->addPrimitiveSet(triangle.get());

	//创建颜色数组
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	//添加数据
	vc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	vc->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	//创建颜色索引数组
	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>* colorIndex =
		new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>();
	//添加数据，注意添加数据顺序与顶点一一对应
	colorIndex->push_back(0);
	colorIndex->push_back(1);
	colorIndex->push_back(2);
	colorIndex->push_back(3);
	colorIndex->push_back(2);

	//设置颜色数组
	geom->setColorArray(vc.get());
	//设置颜色索引数组
	// geom->setColorIndices(colorIndex);
	//设置颜色的绑定方式为单个顶点
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//创建法线数组
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//添加法线
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	//设置法线数组
	geom->setNormalArray(nc.get());
	//设置法线的绑定方式为全部顶点
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//添加到叶节点
	geode->addDrawable(geom.get());

	return geode.get();
}

int main(int argc, char** argv)
{
	//创建Viewer对象，场景浏览器
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