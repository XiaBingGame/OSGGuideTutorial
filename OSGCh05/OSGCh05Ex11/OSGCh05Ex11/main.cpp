#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture3D>

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
osg::ref_ptr<osg::Node> createNode()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//设置顶点
	osg::ref_ptr<osg::Vec3Array> vc = new osg::Vec3Array();
	vc->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vc->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	vc->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	geom->setVertexArray(vc.get());

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));

	geom->setTexCoordArray(0, vt.get());

	//设置法线
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	geom->setNormalArray(nc.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//添加图元
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//绘制
	geode->addDrawable(geom.get());

	return geode.get();
}

//初始化一个图形环境
class MyGraphicsContext
{
public:
	MyGraphicsContext()
	{
		//设置图形环境特性
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		//设置左上角坐标
		traits->x = 0;
		traits->y = 0;
		//设置宽度和高度
		traits->width = 1;
		traits->height = 1;
		//设置窗口扩展
		traits->windowDecoration = false;
		//设置双缓冲
		traits->doubleBuffer = false;
		traits->sharedContext = 0;
		//设置pbuffer
		traits->pbuffer = true;

		//创建图形环境
		_gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		//如果创建失败
		if (!_gc)
		{
			//设置pbuffer为false
			traits->pbuffer = false;
			//重新创建图形环境
			_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		}

		//是否初始化
		if (_gc.valid())
		{
			//初始化
			_gc->realize();
			_gc->makeCurrent();
		}
	}

	bool valid() const { return _gc.valid() && _gc->isRealized(); }

private:
	osg::ref_ptr<osg::GraphicsContext> _gc;
};

//创建三维纹理属性
osg::ref_ptr<osg::StateSet> createState()
{
	//创建图形环境
	/*
	MyGraphicsContext gc;
	if (!gc.valid())
	{
		//如果创建失败，则返回
		osg::notify(osg::NOTICE) << "Unable to create the graphics context required to build 3d image." << std::endl;
		return 0;
	}
	*/

	//读取四张二维纹理图像
	osg::ref_ptr<osg::Image> image_0 = osgDB::readImageFile("Images/lz.rgb");
	osg::ref_ptr<osg::Image> image_1 = osgDB::readImageFile("Images/reflect.rgb");
	osg::ref_ptr<osg::Image> image_2 = osgDB::readImageFile("Images/tank.rgb");
	osg::ref_ptr<osg::Image> image_3 = osgDB::readImageFile("Images/skymap.jpg");

	//判断是否正确读取
	if (!image_0 || !image_1 || !image_2 || !image_3)
	{
		std::cout << "Warning: could not open files." << std::endl;

		return new osg::StateSet();
	}
	else {
		std::cout << "Read images success!" << std::endl;
	}

	//判断纹理格式是否一致
	if (image_0->getPixelFormat() != image_1->getPixelFormat() || image_0->getPixelFormat() != image_2->getPixelFormat() || image_0->getPixelFormat() != image_3->getPixelFormat())
	{
		std::cout << "Warning: image pixel formats not compatible." << std::endl;

		return new osg::StateSet();
	}

	//得到支持的最大的三维纹理单元的大小
	/*
	GLint textureSize = osg::Texture3D::getExtensions(0, true)->maxTexture3DSize();
	if (textureSize > 256)
	{
		textureSize = 256;
	}
	*/
	GLuint textureSize = 256;

	//对四张二维纹理图像缩放，以达到相同的大小
	image_0->scaleImage(textureSize, textureSize, 1);
	image_1->scaleImage(textureSize, textureSize, 1);
	image_2->scaleImage(textureSize, textureSize, 1);
	image_3->scaleImage(textureSize, textureSize, 1);

	//创建一个三维纹理数据图像，注意深度为4
	osg::ref_ptr<osg::Image> image_3d = new osg::Image;
	//第一个和第二个参数是纹理的大小，第三个参数指的是三维纹理数据图像的深度
	image_3d->allocateImage(textureSize, textureSize, 4, image_0->getPixelFormat(), image_0->getDataType());

	//把四张二维纹理图像压入三维纹理数据图像
	//第1-3个参数分别是s,t,r上的偏移，当然这里只是r上的偏移
	//第四个参数是子二维纹理图像数据
	image_3d->copySubImage(0, 0, 0, image_0.get());
	image_3d->copySubImage(0, 0, 1, image_1.get());
	image_3d->copySubImage(0, 0, 2, image_2.get());
	image_3d->copySubImage(0, 0, 3, image_3.get());

	//设置纹理格式
	image_3d->setInternalTextureFormat(image_0->getInternalTextureFormat());

	//创建三维纹理对象
	osg::ref_ptr<osg::Texture3D> texture3D = new osg::Texture3D;
	//设置滤波，不支持mip map滤波
	texture3D->setFilter(osg::Texture3D::MIN_FILTER, osg::Texture3D::LINEAR);
	texture3D->setFilter(osg::Texture3D::MAG_FILTER, osg::Texture3D::LINEAR);
	//设置环绕模式
	texture3D->setWrap(osg::Texture3D::WRAP_R, osg::Texture3D::REPEAT);
	//关联三维纹理图像数据
	texture3D->setImage(image_3d.get());

	//设置自动生成纹理坐标
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
	//设置自动生成纹理坐标为视觉线性
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	//指定参考平面
	texgen->setPlane(osg::TexGen::R, osg::Plane(0.0f, 0.0f, 0.0f, 0.2f));

	//创建状态属性对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
	//设置在R上自动生成纹理坐标
	stateset->setTextureMode(0, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	//启用自动生成纹理坐标
	stateset->setTextureAttribute(0, texgen.get());
	//启用三维纹理对象
	stateset->setTextureAttributeAndModes(0, texture3D.get(), osg::StateAttribute::ON);

	return stateset.get();
}


int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = createNode();

	//创建状态属性对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = createState();

	//使用三维纹理
	node->setStateSet(stateset.get());
	root->addChild(node.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);

	viewer->realize();

	viewer->run();
	return 0;
}