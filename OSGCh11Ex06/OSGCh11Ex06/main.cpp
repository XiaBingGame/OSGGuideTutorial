#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Sequence>

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

osg::ref_ptr<osg::Node> createBillboard(osg::ref_ptr<osg::Image> image)
{
	//创建四边形
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	//设置顶点
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(-0.5f, 0.0f, -0.5f));
	v->push_back(osg::Vec3(0.5f, 0.0f, -0.5f));
	v->push_back(osg::Vec3(0.5f, 0.0f, 0.5f));
	v->push_back(osg::Vec3(-0.5f, 0.0f, 0.5f));

	geometry->setVertexArray(v.get());

	//设置法线
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	normal->push_back(osg::Vec3(1.0f, 0.0f, 0.0f) ^ osg::Vec3(0.0f, 0.0f, 1.0f));

	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));

	geometry->setTexCoordArray(0, vt.get());

	//绘制四边形
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	if (image.get())
	{
		//属性对象
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

		//创建一个Texture2D属性对象
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		//关联image
		texture->setImage(image.get());

		//关联Texture2D纹理对象，第三个参数默认为ON
		stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
		//启用混合
		stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		//关闭光照
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		geometry->setStateSet(stateset.get());
	}

	//创建Billboard对象一
	osg::ref_ptr<osg::Billboard> billboard1 = new osg::Billboard();

	//设置旋转模式为绕视点
	billboard1->setMode(osg::Billboard::POINT_ROT_EYE);
	//添加Drawable，并设置其位置，默认位置为osg::Vec3(0.0f,0.0f,0.0f) ;
	billboard1->addDrawable(geometry.get(), osg::Vec3(5.0f, 0.0f, 0.0f));

	osg::ref_ptr<osg::Billboard> billboard2 = new osg::Billboard();

	osg::ref_ptr<osg::Group> billboard = new osg::Group();
	billboard->addChild(billboard1.get());

	return billboard.get();

}
//创建帧动画
osg::ref_ptr<osg::Sequence> createSequence()
{
	//创建帧动画对象
	osg::ref_ptr<osg::Sequence> seq = new osg::Sequence;

	//文件名向量对象
	typedef std::vector<std::string> Filenames;
	Filenames filenames;

	for (int i = 0; i < 60; i++)
	{
		char name_count[10];
		sprintf(name_count, "bz%d.jpg", i);
		filenames.push_back(name_count);
	}

	for (Filenames::iterator itr = filenames.begin(); itr != filenames.end(); ++itr)
	{
		//加载模型
		osg::Image* image = osgDB::readImageFile(*itr);

		if (image)
		{
			//添加子节点
			seq->addChild(createBillboard(image));
			//设定节点的持续时间
			seq->setTime(seq->getNumChildren() - 1, 0.1f);
		}
	}

	//设置帧动画持续的时间
	seq->setInterval(osg::Sequence::LOOP, 0, -1);

	//设置播放的速度及重复的次数
	seq->setDuration(1.0f, -1);

	//开始播放
	seq->setMode(osg::Sequence::START);

	return seq.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//向场景中添加帧动画
	osg::ref_ptr<osg::Sequence> sq = new osg::Sequence();
	sq = createSequence();

	root->addChild(sq.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}