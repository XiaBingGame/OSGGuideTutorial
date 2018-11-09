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

//定义相机绘制回调
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
			//获得Image的中心
			int column_start = _image->s() / 4;
			int column_end = 3 * column_start;

			int row_start = _image->t() / 4;
			int row_end = 3 * row_start;

			//将像素数据进行反向
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
			//获得Image的中心
			int column_start = _image->s() / 4;
			int column_end = 3 * column_start;

			int row_start = _image->t() / 4;
			int row_end = 3 * row_start;

			//将像素数据进行反向
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

//创建预渲染场景
osg::ref_ptr<osg::Node> createPreRenderSubGraph(osg::ref_ptr<osg::Node> subgraph,
	unsigned tex_width,
	unsigned tex_height,
	osg::Camera::RenderTargetImplementation renderImplementation,
	bool useImage)
{
	if (!subgraph) return 0;

	//创建一个包含预渲camera的 Group 节点
	osg::ref_ptr<osg::Group> parent = new osg::Group;

	//创建纹理，用来绑定相机渲染的结果
	osg::ref_ptr<osg::Texture> texture = 0;
	{
		osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
		texture2D->setTextureSize(tex_width, tex_height);
		texture2D->setInternalFormat(GL_RGBA);
		texture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
		texture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

		texture = texture2D;
	}

	//创建一个用来浏览的四边形几何体
	{
		osg::ref_ptr<osg::Geometry> polyGeom = new osg::Geometry();
		//设置该几何体不使用显示列表
		polyGeom->setSupportsDisplayList(false);

		float height = 100.0f;
		float width = 200.0f;

		//创建顶点数组，并添加数据
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
		vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
		vertices->push_back(osg::Vec3(width, 0.0f, 0.0f));
		vertices->push_back(osg::Vec3(width, 0.0f, height));
		vertices->push_back(osg::Vec3(0.0f, 0.0f, height));

		//创建纹理数组，并添加数据
		osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array();
		texcoords->push_back(osg::Vec2(0.0f, 0.0f));
		texcoords->push_back(osg::Vec2(1.0f, 0.0f));
		texcoords->push_back(osg::Vec2(1.0f, 1.0f));
		texcoords->push_back(osg::Vec2(0.0f, 1.0f));

		polyGeom->setVertexArray(vertices.get());

		//使用vbo扩展
		{
			osg::ref_ptr<osg::VertexBufferObject> vbObject = new osg::VertexBufferObject;
			vertices->setVertexBufferObject(vbObject);

			polyGeom->setUseVertexBufferObjects(true);
		}

		polyGeom->setTexCoordArray(0, texcoords.get());

		//创建颜色数组，并设置绑定方式及添加数据
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		polyGeom->setColorArray(colors.get());
		polyGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

		polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size()));

		//现在我们需要将纹理附加到该几何体上，我们创建一个包含该纹理的StateSet
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

		polyGeom->setStateSet(stateset);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		geode->addDrawable(polyGeom.get());

		parent->addChild(geode.get());

	}

	// 需要创建一个相机节点，用来渲染到该纹理(RTT)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;

		//设置背景色及清除颜色和深度缓存
		camera->setClearColor(osg::Vec4(0.1f, 0.1f, 0.3f, 1.0f));
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//获得该节点的范围盒
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

		//设置投影矩阵.
		camera->setProjectionMatrixAsFrustum(-proj_right, proj_right, -proj_top, proj_top, znear, zfar);

		//将相机对准该子场景
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setViewMatrixAsLookAt(bs.center() - osg::Vec3(0.0f, 2.0f, 0.0f)*bs.radius(), bs.center(), osg::Vec3(0.0f, 0.0f, 1.0f));

		//设置视口
		camera->setViewport(0, 0, tex_width, tex_height);

		//设置相机的渲染序列
		camera->setRenderOrder(osg::Camera::PRE_RENDER);

		//设置相机渲染通过 OpenGL frame buffer object 实现
		camera->setRenderTargetImplementation(renderImplementation);


		if (useImage)
		{
			osg::ref_ptr<osg::Image> image = new osg::Image;
			//image->allocateImage(tex_width, tex_height, 1, GL_RGBA, GL_UNSIGNED_BYTE);
			image->allocateImage(tex_width, tex_height, 1, GL_RGBA, GL_FLOAT);

			//将Image附加到相机的COLOR_BUFFER
			camera->attach(osg::Camera::COLOR_BUFFER, image.get());

			//添加相机的绘制后回调，修改images数据
			camera->setPostDrawCallback(new MyCameraPostDrawCallback(image.get()));

			//这里我们不直接将相机的COLOR_BUFFER附加到该纹理上，是为了修改渲染后的图像数据
			texture->setImage(0, image.get());
		}
		else
		{
			//直接将该纹理附加到相机的颜色缓存.
			camera->attach(osg::Camera::COLOR_BUFFER, texture.get());
		}

		//添加要绘制的子场景
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

	//读取模型
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("cessna.osg");

	//创建一个transform节点,用来选装该子场景
	osg::ref_ptr<osg::MatrixTransform> loadedModelTransform = new osg::MatrixTransform;
	loadedModelTransform->addChild(loadedModel.get());

	//设置更新回调
	osg::ref_ptr<osg::NodeCallback> nc = new osg::AnimationPathCallback(loadedModelTransform->getBound().center(), osg::Vec3(0.0f, 0.0f, 1.0f), osg::inDegrees(45.0f));
	loadedModelTransform->setUpdateCallback(nc);

	osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	rootNode->addChild(createPreRenderSubGraph(loadedModelTransform.get(), tex_width, tex_height, renderImplementation, useImage));

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(rootNode.get());

	viewer->setSceneData(rootNode.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();

	return 0;
}