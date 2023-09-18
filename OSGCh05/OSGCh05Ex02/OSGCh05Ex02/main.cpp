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
		static double dColor = 0;//��ɫ

		glColor3f(dColor, 0, 0);
		glBegin(GL_TRIANGLES);//��OSG�л�һ��opengl������
		glVertex3f(0.0, 0.0, -2.0);
		glVertex3f(0.2, 0.0, -2.0);
		glVertex3f(0.0, 0.4, -2.0);
		glEnd();

		dColor += 0.0001;//��ɫ����
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
	//�˴�һ��Ҫ����ʾ�б�����Ϊfalse,

	//����DrawCallback��drawImplementation()����ֻ�����һ�Σ��������ڻ�һ֡ʱ����̬����openglͼ��

	geometry->setUseDisplayList(false);
	geometry->setDrawCallback(new DrawCallback);//Drawable���ö�̬����openglͼ��
	

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

//����һ���ı��νڵ�
osg::ref_ptr<osg::Node> createNode()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//���ö���
	osg::ref_ptr<osg::Vec3Array> vc = new osg::Vec3Array();
	vc->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vc->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	vc->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	geom->setVertexArray(vc.get());

	//������������
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));

	geom->setTexCoordArray(0, vt.get());

	//���÷���
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	geom->setNormalArray(nc.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//���ͼԪ
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//����
	geode->addDrawable(geom.get());
	// geom->addDrawCallback(new osg::DrawC)

	return geode.get();
}

//������ά����״̬����
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::Image* image)
{
	osg::ref_ptr<osg::Image> image2 = osgDB::readImageFile("Images/sunset.jpg");
	
	//����״̬������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	
	//������ά�������
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//������ͼ
	texture->setImage(image2.get());
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
	//texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);

	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
	//���û��
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	//�رչ���
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	
	return stateset.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡ��ͼ�ļ�
	//osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/screenshot.jpg");
	osg::ref_ptr<osg::Node> node = createNode();

	//����״̬������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	// stateset = createTexture2DState(image.get());
	stateset = createTexture2DState(0);

	//ʹ�ö�ά����
	node->setStateSet(stateset.get());

	root->addChild(node.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}
