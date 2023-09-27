#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/GLExtensions>
#include <osg/Depth>
#include <osg/AnimationPath>

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

//����������ƻص�
struct MyCameraPostDrawCallback : public osg::Camera::DrawCallback
{
public:

	MyCameraPostDrawCallback(osg::ref_ptr<osg::Image> image) :
		_image(image)
	{

	}

	virtual void operator () (const osg::Camera& /*camera*/) const
	{
		if (_image && _image->getPixelFormat() == GL_RGBA && _image->getDataType() == GL_UNSIGNED_BYTE)
		{
			//���Image������
			int column_start = _image->s() / 4;
			int column_end = 3 * column_start;

			int row_start = _image->t() / 4;
			int row_end = 3 * row_start;

			//���������ݽ��з���
			for (int r = row_start; r < row_end; ++r)
			{
				unsigned char* data = _image->data(column_start, r);
				for (int c = column_start; c < column_end; ++c)
				{
					(*data) = 255 - (*data); ++data;
					(*data) = 255 - (*data); ++data;
					(*data) = 255 - (*data); ++data;
					(*data) = 255; ++data;
				}
			}
			_image->dirty();
		}
		else if (_image && _image->getPixelFormat() == GL_RGBA && _image->getDataType() == GL_FLOAT)
		{
			//���Image������
			int column_start = _image->s() / 4;
			int column_end = 3 * column_start;

			int row_start = _image->t() / 4;
			int row_end = 3 * row_start;

			//���������ݽ��з���
			for (int r = row_start; r < row_end; ++r)
			{
				float* data = (float*)_image->data(column_start, r);
				for (int c = column_start; c < column_end; ++c)
				{
					(*data) = 1.0f - (*data); ++data;
					(*data) = 1.0f - (*data); ++data;
					(*data) = 1.0f - (*data); ++data;
					(*data) = 1.0f; ++data;
				}
			}
			_image->dirty();
		}
	}

public:

	osg::ref_ptr<osg::Image> _image;
};

//����Ԥ��Ⱦ����
osg::ref_ptr<osg::Node> createPreRenderSubGraph(osg::ref_ptr<osg::Node> subgraph,
	unsigned tex_width,
	unsigned tex_height,
	osg::Camera::RenderTargetImplementation renderImplementation,
	bool useImage)
{
	if (!subgraph) return 0;

	//����һ������Ԥ��camera�� Group �ڵ�
	osg::ref_ptr<osg::Group> parent = new osg::Group;

	//�������������������Ⱦ�Ľ��
	osg::ref_ptr<osg::Texture> texture = 0;
	{
		osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
		texture2D->setTextureSize(tex_width, tex_height);
		texture2D->setInternalFormat(GL_RGBA);
		texture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
		texture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

		texture = texture2D;
	}

	//����һ������������ı��μ�����
	{
		osg::ref_ptr<osg::Geometry> polyGeom = new osg::Geometry();
		//���øü����岻ʹ����ʾ�б�
		polyGeom->setSupportsDisplayList(false);

		float height = 100.0f;
		float width = 200.0f;

		//�����������飬���������
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
		vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
		vertices->push_back(osg::Vec3(width, 0.0f, 0.0f));
		vertices->push_back(osg::Vec3(width, 0.0f, height));
		vertices->push_back(osg::Vec3(0.0f, 0.0f, height));

		//�����������飬���������
		osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array();
		texcoords->push_back(osg::Vec2(0.0f, 0.0f));
		texcoords->push_back(osg::Vec2(1.0f, 0.0f));
		texcoords->push_back(osg::Vec2(1.0f, 1.0f));
		texcoords->push_back(osg::Vec2(0.0f, 1.0f));

		polyGeom->setVertexArray(vertices.get());

		//ʹ��vbo��չ
		{
			osg::ref_ptr<osg::VertexBufferObject> vbObject = new osg::VertexBufferObject;
			vertices->setVertexBufferObject(vbObject);

			polyGeom->setUseVertexBufferObjects(true);
		}

		polyGeom->setTexCoordArray(0, texcoords.get());

		//������ɫ���飬�����ð󶨷�ʽ���������
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		polyGeom->setColorArray(colors.get());
		polyGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

		polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size()));

		//����������Ҫ�������ӵ��ü������ϣ����Ǵ���һ�������������StateSet
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		polyGeom->setStateSet(stateset);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		geode->addDrawable(polyGeom.get());

		parent->addChild(geode.get());

	}

	// ��Ҫ����һ������ڵ㣬������Ⱦ��������(RTT)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;

		//���ñ���ɫ�������ɫ����Ȼ���
		camera->setClearColor(osg::Vec4(0.6f, 0.6f, 0.3f, 1.0f));
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//��øýڵ�ķ�Χ��
		const osg::BoundingSphere& bs = subgraph->getBound();
		if (!bs.valid())
		{
			return subgraph.get();
		}

		float znear = 1.0f*bs.radius();
		float zfar = 3.0f*bs.radius();

		float proj_top = 0.25f*znear;
		float proj_right = 0.5f*znear;

		znear *= 0.9f;
		zfar *= 1.1f;

		//����ͶӰ����.
		camera->setProjectionMatrixAsFrustum(-proj_right, proj_right, -proj_top, proj_top, znear, zfar);

		//�������׼���ӳ���
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setViewMatrixAsLookAt(bs.center() - osg::Vec3(0.0f, 2.0f, 0.0f)*bs.radius(), bs.center(), osg::Vec3(0.0f, 0.0f, 1.0f));

		//�����ӿ�
		camera->setViewport(0, 0, tex_width, tex_height);

		//�����������Ⱦ����
		camera->setRenderOrder(osg::Camera::PRE_RENDER);

		//���������Ⱦͨ�� OpenGL frame buffer object ʵ��
		camera->setRenderTargetImplementation(renderImplementation);


		if (useImage)
		{
			osg::ref_ptr<osg::Image> image = new osg::Image;
			//image->allocateImage(tex_width, tex_height, 1, GL_RGBA, GL_UNSIGNED_BYTE);
			image->allocateImage(tex_width, tex_height, 1, GL_RGBA, GL_FLOAT);

			//��Image���ӵ������COLOR_BUFFER
			camera->attach(osg::Camera::COLOR_BUFFER, image.get());

			//�������Ļ��ƺ�ص����޸�images����
			camera->setPostDrawCallback(new MyCameraPostDrawCallback(image.get()));

			//�������ǲ�ֱ�ӽ������COLOR_BUFFER���ӵ��������ϣ���Ϊ���޸���Ⱦ���ͼ������
			texture->setImage(0, image.get());
		}
		else
		{
			//ֱ�ӽ��������ӵ��������ɫ����.
			camera->attach(osg::Camera::COLOR_BUFFER, texture.get());
		}

		//���Ҫ���Ƶ��ӳ���
		camera->addChild(subgraph.get());

		parent->addChild(camera.get());

	}

	return parent.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	unsigned tex_width = 1024;
	unsigned tex_height = 512;

	osg::Camera::RenderTargetImplementation renderImplementation = osg::Camera::FRAME_BUFFER_OBJECT;

	bool useImage = false;

	//��ȡģ��
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("cessna.osg");

	//����һ��transform�ڵ�,����ѡװ���ӳ���
	osg::ref_ptr<osg::MatrixTransform> loadedModelTransform = new osg::MatrixTransform;
	loadedModelTransform->addChild(loadedModel.get());

	//���ø��»ص�
	osg::ref_ptr<osg::NodeCallback> nc = new osg::AnimationPathCallback(loadedModelTransform->getBound().center(), osg::Vec3(0.0f, 0.0f, 1.0f), osg::inDegrees(45.0f));
	loadedModelTransform->setUpdateCallback(nc);

	osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	rootNode->addChild(createPreRenderSubGraph(loadedModelTransform.get(), tex_width, tex_height, renderImplementation, useImage));

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(rootNode.get());

	viewer->setSceneData(rootNode.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();

	return 0;
}