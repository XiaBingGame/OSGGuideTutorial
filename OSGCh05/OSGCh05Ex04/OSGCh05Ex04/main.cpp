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

//����һ���ı���
osg::ref_ptr<osg::Geode> createQuad()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	geode->addDrawable(geom.get());

	//���ö���
	osg::ref_ptr<osg::Vec3Array> vec = new osg::Vec3Array;
	vec->push_back(osg::Vec3(-10.0f, 0.0f, -10.0f));
	vec->push_back(osg::Vec3(-10.0f, 0.0f, 10.0f));
	vec->push_back(osg::Vec3(10.0f, 0.0f, 10.0f));
	vec->push_back(osg::Vec3(10.0f, 0.0f, -10.0f));
	geom->setVertexArray(vec.get());

	//���÷���
	osg::ref_ptr<osg::Vec3Array> nor = new osg::Vec3Array;
	nor->push_back(osg::Vec3f(0.0f, -1.0f, 0.0f));

	geom->setNormalArray(nor.get());
	geom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

	//������������
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
	//��ɫ
	if (size == 1)
	{
		float r = 0.5f;
		osg::Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
		*ptr++ = (unsigned char)((color[0])*255.0f);
		*ptr++ = (unsigned char)((color[1])*255.0f);
		*ptr++ = (unsigned char)((color[2])*255.0f);
		*ptr++ = (unsigned char)((color[3])*255.0f);
	}

	//��ɫ
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

	//��ɫ
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

	//��ɫ
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

	//�ۺ�ɫ
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

	//��ɫ
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

	//����ɫ
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

	//�Ұ�ɫ
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

	//��ɫ
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

	//����һ��ƽ��
	osg::ref_ptr<osg::Geode> geode = createQuad();

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	osg::ref_ptr<osg::Image> image = new osg::Image();

	//����һ��MipmapDataType�б���������ͼƬ���ݵ�ƫ�Ƶ�ַ
	osg::Image::MipmapDataType mipmapData;

	//����ĳߴ�����ֵ������Ϊ2���ݴ�
	unsigned int s = 256;

	//����������������Ĵ�С
	unsigned int totalSize = 0;

	for (unsigned int i = 0; s > 0; s >>= 1, ++i)
	{
		if (i > 0)
		{
			mipmapData.push_back(totalSize);
		}

		totalSize += s*s * 4;
	}

	//����һ������
	unsigned char* ptr = new unsigned char[totalSize];

	//����image�ĳߴ��С�����ݼ����ݸ�ʽ
	image->setImage(256, 256, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, ptr, osg::Image::USE_NEW_DELETE, 1);

	//��ƫ�Ƶ�ַ����imge����
	image->setMipmapLevels(mipmapData);

	//��image������������
	s = 256;
	for (unsigned int i = 0; s > 0; s >>= 1, ++i)
	{
		fillImage(ptr, s);

		ptr += s*s * 4;
	}

	//����һ����ά�������
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	//������ͼ
	texture->setImage(0, image.get());
	//���ñ߽紦��ΪREPEATE
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
	//�����˲�
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST_MIPMAP_NEAREST);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	//���ö�ά�������
	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

	geode->setStateSet(stateset.get());

	root->addChild(geode.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}