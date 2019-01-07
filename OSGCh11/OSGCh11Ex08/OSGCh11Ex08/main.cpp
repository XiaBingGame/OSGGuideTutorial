#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/ComputeBoundsVisitor>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/AnimationPath>

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

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>

#include <osgViewer/Viewer>
#include <iostream>
#include "Tex.h"

//标识阴影接收对象
const int ReceivesShadowTraversalMask = 0x1;
//标识阴影投影对象
const int CastsShadowTraversalMask = 0x2;

//创建动画路径
osg::ref_ptr<osg::AnimationPath> createAnimationPath(const osg::Vec3& center, float radius, double looptime)
{
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;
	float yaw = 0.0f;
	float yaw_delta = 2.0f*osg::PI / ((float)numSamples - 1.0f);
	float roll = osg::inDegrees(30.0f);

	double time = 0.0f;
	double time_delta = looptime / (double)numSamples;
	for (int i = 0; i < numSamples; ++i)
	{
		osg::Vec3 position(center + osg::Vec3(sinf(yaw)*radius, cosf(yaw)*radius, 0.0f));
		osg::Quat rotation(osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0))*osg::Quat(-(yaw + osg::inDegrees(90.0f)), osg::Vec3(0.0, 0.0, 1.0)));

		animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));

		yaw += yaw_delta;
		time += time_delta;

	}
	return animationPath.get();
}

//创建地形平面
osg::ref_ptr<osg::Node> createBase(const osg::Vec3& center, float radius)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	//设置纹理属性
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/lz.rgb");
	if (image.get())
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(image.get());
		stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
	}

	geode->setStateSet(stateset.get());

	//申请一个高程类
	osg::ref_ptr<osg::HeightField> grid = new osg::HeightField;
	grid->allocate(38, 39);
	grid->setOrigin(center + osg::Vec3(-radius, -radius, 0.0f));
	grid->setXInterval(radius*2.0f / (float)(38 - 1));
	grid->setYInterval(radius*2.0f / (float)(39 - 1));

	//得到最高值和最低值，以确定合适的高程缩放和
	//高程偏移
	float minHeight = FLT_MAX;
	float maxHeight = -FLT_MAX;
	unsigned int r;
	for (r = 0; r < 39; ++r)
	{
		for (unsigned int c = 0; c < 38; ++c)
		{
			float h = vertex[r + c * 39][2];
			if (h > maxHeight) maxHeight = h;
			if (h < minHeight) minHeight = h;
		}
	}

	float hieghtScale = radius*0.5f / (maxHeight - minHeight);
	float hieghtOffset = -(minHeight + maxHeight)*0.5f;

	//设置行列数据
	for (r = 0; r < 39; ++r)
	{
		for (unsigned int c = 0; c < 38; ++c)
		{
			float h = vertex[r + c * 39][2];
			grid->setHeight(c, r, (h + hieghtOffset)*hieghtScale);
		}
	}
	geode->addDrawable(new osg::ShapeDrawable(grid.get()));

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(geode.get());

	return group.get();
}

//创建飞机动画模型
osg::ref_ptr<osg::Node> createMovingModel(const osg::Vec3& center, float radius)
{
	float animationLength = 10.0f;

	osg::ref_ptr<osg::AnimationPath> animationPath = createAnimationPath(center, radius, animationLength);

	osg::ref_ptr<osg::Group> model = new osg::Group;

	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");
	if (cessna.get())
	{
		const osg::BoundingSphere& bs = cessna->getBound();

		float size = radius / bs.radius()*0.3f;
		osg::ref_ptr<osg::MatrixTransform> positioned = new osg::MatrixTransform;
		positioned->setDataVariance(osg::Object::STATIC);
		positioned->setMatrix(osg::Matrix::translate(-bs.center())*
			osg::Matrix::scale(size, size, size)*
			osg::Matrix::rotate(osg::inDegrees(180.0f), 0.0f, 0.0f, 2.0f));

		positioned->addChild(cessna.get());

		osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform;
		xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath, 0.0f, 2.0));
		xform->addChild(positioned);

		model->addChild(xform.get());
	}

	return model.get();
}

//创建场景数据
osg::ref_ptr<osg::Node> createModel()
{
	osg::Vec3 center(0.0f, 0.0f, 0.0f);
	float radius = 100.0f;
	osg::Vec3 lightPosition(center + osg::Vec3(0.0f, 0.0f, radius));

	osg::ref_ptr<osg::Node> shadower = createMovingModel(center, radius*0.5f);
	shadower->setNodeMask(CastsShadowTraversalMask);

	osg::ref_ptr<osg::Node> shadowed = createBase(center - osg::Vec3(0.0f, 0.0f, radius*0.25), radius);
	shadowed->setNodeMask(ReceivesShadowTraversalMask);

	osg::ref_ptr<osg::Group> group = new osg::Group;

	group->addChild(shadowed.get());
	group->addChild(shadower.get());

	return group.get();
}

//创建一个光照
osg::ref_ptr<osg::Node> createLight(osg::ref_ptr<osg::Node> model)
{
	osg::ComputeBoundsVisitor cbbv;
	model->accept(cbbv);
	osg::BoundingBox bb = cbbv.getBoundingBox();

	osg::Vec4 lightpos;

	lightpos.set(bb.center().x(), bb.center().y(), bb.zMax() + bb.radius()*2.0f, 1.0f);

	osg::ref_ptr<osg::LightSource> ls = new osg::LightSource();
	ls->getLight()->setPosition(lightpos);

	ls->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
	ls->getLight()->setDiffuse(osg::Vec4(0.8, 0.8, 0.8, 1.0));

	return ls.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建一个阴影节点，并标识接收对象和投影对象
	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene();
	shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);

	//创建阴影纹理
	osg::ref_ptr<osgShadow::ShadowTexture> st = new osgShadow::ShadowTexture();
	//关联阴影纹理
	shadowedScene->setShadowTechnique(st);

	osg::ref_ptr<osg::Node> node = new osg::Node();
	node = createModel();

	//添加场景数据并添加光源
	shadowedScene->addChild(createLight(node.get()));
	shadowedScene->addChild(node.get());

	root->addChild(shadowedScene.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}