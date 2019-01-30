#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/TexGen>
#include <osg/ShapeDrawable>

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

#include "Tex.h"


//创建聚光灯纹理的mipmap贴图
osg::ref_ptr<osg::Image> createSpotLightImage(const osg::Vec4& centerColour, const osg::Vec4& backgroudColour, unsigned int size, float power)
{
	//创建Image对象
	osg::ref_ptr<osg::Image> image = new osg::Image;
	//动态分配一个size*size大小的image
	image->allocateImage(size, size, 1, GL_RGBA, GL_UNSIGNED_BYTE);

	//填充image
	//以中心为原点，颜色逐渐向四周衰减
	float mid = (float(size) - 1)*0.5f;
	float div = 2.0f / float(size);
	for (unsigned int r = 0; r < size; ++r)
	{
		unsigned char* ptr = image->data(0, r, 0);
		for (unsigned int c = 0; c < size; ++c)
		{
			float dx = (float(c) - mid)*div;
			float dy = (float(r) - mid)*div;
			float r = powf(1.0f - sqrtf(dx*dx + dy*dy), power);
			if (r < 0.0f) r = 0.0f;
			osg::Vec4 color = centerColour*r + backgroudColour*(1.0f - r);
			*ptr++ = (unsigned char)((color[0])*255.0f);
			*ptr++ = (unsigned char)((color[1])*255.0f);
			*ptr++ = (unsigned char)((color[2])*255.0f);
			*ptr++ = (unsigned char)((color[3])*255.0f);
		}
	}
	return image.get();
}

//创建聚光灯状态属性
osg::ref_ptr<osg::StateSet> createSpotLightDecoratorState(unsigned int lightNum, unsigned int textureUnit)
{
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

	//开启ID为lightNum的光照
	stateset->setMode(GL_LIGHT0 + lightNum, osg::StateAttribute::ON);

	//设置中心的颜色和环境光的颜色
	osg::Vec4 centerColour(1.0f, 1.0f, 1.0f, 1.0f);
	osg::Vec4 ambientColour(0.5f, 0.5f, 0.5f, 1.0f);

	//创建聚光灯纹理
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setImage(createSpotLightImage(centerColour, ambientColour, 64, 1.0));
	texture->setBorderColor(osg::Vec4(ambientColour));
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_BORDER);

	//打开纹理单元
	stateset->setTextureAttributeAndModes(textureUnit, texture.get(), osg::StateAttribute::ON);

	//设置自动生成纹理坐标
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_Q, osg::StateAttribute::ON);

	return stateset.get();
}

//创建聚光灯节点
osg::ref_ptr<osg::Node> createSpotLightNode(const osg::Vec3& position, const osg::Vec3& direction, float angle, unsigned int lightNum, unsigned int textureUnit)
{
	osg::ref_ptr<osg::Group> group = new osg::Group;

	//创建光源
	osg::ref_ptr<osg::LightSource> lightsource = new osg::LightSource;
	osg::ref_ptr<osg::Light> light = lightsource->getLight();
	light->setLightNum(lightNum);
	light->setPosition(osg::Vec4(position, 1.0f));
	light->setAmbient(osg::Vec4(0.00f, 0.00f, 0.05f, 1.0f));
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	group->addChild(lightsource);

	//计算法向量
	osg::Vec3 up(0.0f, 0.0f, 1.0f);
	up = (direction ^ up) ^ direction;
	up.normalize();

	//创建自动生成纹理坐标节点
	osg::ref_ptr<osg::TexGenNode> texgenNode = new osg::TexGenNode;
	//关联纹理单元
	texgenNode->setTextureUnit(textureUnit);
	//设置纹理坐标生成器
	osg::ref_ptr<osg::TexGen> texgen = texgenNode->getTexGen();
	//设置模式为视觉线性
	texgen->setMode(osg::TexGen::EYE_LINEAR);
	//从视图中指定参考平面
	texgen->setPlanesFromMatrix(osg::Matrixd::lookAt(position, position + direction, up)*
		osg::Matrixd::perspective(angle, 1.0, 0.1, 100));

	// group->addChild(texgenNode.get());

	return group.get();
}

//创建动画路径(请参看后面章节的OSG动画)
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

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/lz.rgb");
	if (image.get())
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(image.get());
		stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
	}

	geode->setStateSet(stateset.get());

	osg::ref_ptr<osg::HeightField> grid = new osg::HeightField;
	grid->allocate(38, 39);
	grid->setOrigin(center + osg::Vec3(-radius, -radius, 0.0f));
	grid->setXInterval(radius*2.0f / (float)(38 - 1));
	grid->setYInterval(radius*2.0f / (float)(39 - 1));

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

//创建动画模型
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

		//添加聚光灯节点
		xform->addChild(createSpotLightNode(osg::Vec3(0.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, -1.0f), 60.0f, 0, 1));

		model->addChild(xform.get());
	}

	return model.get();
}

//创建场景
osg::ref_ptr<osg::Node> createModel()
{
	osg::Vec3 center(0.0f, 0.0f, 0.0f);
	float radius = 100.0f;

	//创建动画模型
	osg::ref_ptr<osg::Node> shadower = createMovingModel(center, radius*0.5f);

	//创建地形平面
	osg::ref_ptr<osg::Node> shadowed = createBase(center - osg::Vec3(0.0f, 0.0f, radius*0.1), radius);

	//创建场景组节点
	osg::ref_ptr<osg::Group> root = new osg::Group;

	//设置状态属性
	// root->setStateSet(createSpotLightDecoratorState(0, 1));

	//添加子节点
	root->addChild(shadower.get());
	root->addChild(shadowed.get());

	return root.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//添加场景
	root->addChild(createModel());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();

	return 0;
}