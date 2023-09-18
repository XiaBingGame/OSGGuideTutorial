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

class esImage : public osg::Image
{
public:
	static int s_image_count;
	static int s_delete_image_count;
	esImage() : osg::Image() {
		s_image_count++;
		osg::notify(osg::WARN) << "alloc image: " << s_image_count << std::endl;
	}
	~esImage()
	{
		s_delete_image_count++;
		osg::notify(osg::WARN) << "delete image: " << s_delete_image_count << std::endl;
	}
};

int esImage::s_image_count = 0;
int esImage::s_delete_image_count = 0;

class DrawCallback : public osg::Drawable::DrawCallback
{
	virtual void drawImplementation(osg::RenderInfo& renderInfo, const osg::Drawable* drawable) const
	{
		static double dColor = 0;//颜色

		glColor3f(dColor, 0, 0);
		glBegin(GL_TRIANGLES);//在OSG中画一个opengl三角形
		glVertex3f(0.0, 0.0, -2.0);
		glVertex3f(0.2, 0.0, -2.0);
		glVertex3f(0.0, 0.4, -2.0);
		glEnd();

		dColor += 0.0001;//颜色渐变
		if (dColor > 1.0)
		{
			dColor = 0.0;
		}
	}
};

void render_method_1()
{
	osgViewer::Viewer viewer;
	osg::Geometry* geometry = new osg::Geometry;
	//此处一定要把显示列表设置为false,

	//否则DrawCallback的drawImplementation()函数只会调用一次，而不是在画一帧时都动态更新opengl图形

	geometry->setUseDisplayList(false);
	geometry->setDrawCallback(new DrawCallback);//Drawable设置动态更新opengl图形
	

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(geometry);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osg::Group* group = new osg::Group;

	group->addChild(geode);
	viewer.setSceneData(group);

	//return viewer.run();

	osg::Matrix mt;

	mt.makeIdentity();

	while (!viewer.done())
	{
		viewer.getCamera()->setViewMatrix(mt);
		viewer.frame();
	}
}

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
	// geom->addDrawCallback(new osg::DrawC)

	return geode.get();
}

//创建二维纹理状态对象
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::Image* image)
{
	osg::ref_ptr<osg::Image> image2 = osgDB::readImageFile("Images/sunset.jpg");
	
	//创建状态集对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	
	//创建二维纹理对象
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//设置贴图
	texture->setImage(image2.get());
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
	//texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);

	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
	//启用混合
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	//关闭光照
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	
	return stateset.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取贴图文件
	//osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/screenshot.jpg");
	osg::ref_ptr<osg::Node> node = createNode();

	//创建状态集对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	// stateset = createTexture2DState(image.get());
	stateset = createTexture2DState(0);

	//使用二维纹理
	node->setStateSet(stateset.get());

	root->addChild(node.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}
