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

//创建一个四边形
osg::ref_ptr<osg::Geode> createQuad()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	geode->addDrawable(geom.get());

	//设置顶点
	osg::ref_ptr<osg::Vec3Array> vec = new osg::Vec3Array;
	vec->push_back(osg::Vec3(-10.0f, 0.0f, -10.0f));
	vec->push_back(osg::Vec3(-10.0f, 0.0f, 10.0f));
	vec->push_back(osg::Vec3(10.0f, 0.0f, 10.0f));
	vec->push_back(osg::Vec3(10.0f, 0.0f, -10.0f));
	geom->setVertexArray(vec.get());

	//设置法线
	osg::ref_ptr<osg::Vec3Array> nor = new osg::Vec3Array;
	nor->push_back(osg::Vec3f(0.0f, -1.0f, 0.0f));

	geom->setNormalArray(nor.get());
	geom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> tex = new osg::Vec2Array;
	tex->push_back(osg::Vec2f(0.0f, 0.0f));
	tex->push_back(osg::Vec2f(0.0f, 1.0f));
	tex->push_back(osg::Vec2f(1.0f, 1.0f));
	tex->push_back(osg::Vec2f(1.0f, 0.0f));

	geom->setTexCoordArray(0, tex.get());

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	return geode.get();
}

static void fillImage(unsigned char* ptr, unsigned int size)
{
	//白色
	if (size == 1)
	{
		float r = 0.5f;
		osg::Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
		*ptr++ = (unsigned char)((color[0])*255.0f);
		*ptr++ = (unsigned char)((color[1])*255.0f);
		*ptr++ = (unsigned char)((color[2])*255.0f);
		*ptr++ = (unsigned char)((color[3])*255.0f);
	}

	//蓝色
	if (size == 2)
	{
		osg::Vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//黄色
	if (size == 4)
	{
		osg::Vec4 color(0.0f, 1.0f, 0.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//红色
	if (size == 8)
	{
		osg::Vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//粉红色
	if (size == 16)
	{
		osg::Vec4 color(1.0f, 0.0f, 1.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//黄色
	if (size == 32)
	{
		osg::Vec4 color(1.0f, 1.0f, 0.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//蓝绿色
	if (size == 64)
	{
		osg::Vec4 color(0.0f, 1.0f, 1.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//灰白色
	if (size == 128)
	{
		osg::Vec4 color(0.5f, 0.5f, 0.5f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}

	//黑色
	if (size == 256)
	{
		osg::Vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
		for (unsigned int r = 0; r < size; ++r)
		{
			for (unsigned int c = 0; c < size; ++c)
			{
				*ptr++ = (unsigned char)((color[0])*255.0f);
				*ptr++ = (unsigned char)((color[1])*255.0f);
				*ptr++ = (unsigned char)((color[2])*255.0f);
				*ptr++ = (unsigned char)((color[3])*255.0f);
			}
		}
	}
}


int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//创建一个平面
	osg::ref_ptr<osg::Geode> geode = createQuad();

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	osg::ref_ptr<osg::Image> image = new osg::Image();

	//创建一个MipmapDataType列表，用来各层图片数据的偏移地址
	osg::Image::MipmapDataType mipmapData;

	//纹理的尺寸的最大值，必须为2的幂次
	unsigned int s = 256;

	//计算所需分配的数组的大小
	unsigned int totalSize = 0;

	for (unsigned int i = 0; s > 0; s >>= 1, ++i)
	{
		if (i > 0)
		{
			mipmapData.push_back(totalSize);
		}

		totalSize += s*s * 4;
	}

	//申请一个数据
	unsigned char* ptr = new unsigned char[totalSize];

	//设置image的尺寸大小，数据及数据格式
	image->setImage(256, 256, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, ptr, osg::Image::USE_NEW_DELETE, 1);

	//将偏移地址传入imge对象
	image->setMipmapLevels(mipmapData);

	//向image中填充各层数据
	s = 256;
	for (unsigned int i = 0; s > 0; s >>= 1, ++i)
	{
		fillImage(ptr, s);

		ptr += s*s * 4;
	}

	//创建一个二维纹理对象
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	//设置贴图
	texture->setImage(0, image.get());
	//设置边界处理为REPEATE
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
	//设置滤波
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST_MIPMAP_NEAREST);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	//启用二维纹理对象
	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

	geode->setStateSet(stateset.get());

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