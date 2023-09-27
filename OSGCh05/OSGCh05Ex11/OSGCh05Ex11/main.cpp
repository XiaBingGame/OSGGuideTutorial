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

	return geode.get();
}

//��ʼ��һ��ͼ�λ���
class MyGraphicsContext
{
public:
	MyGraphicsContext()
	{
		//����ͼ�λ�������
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		//�������Ͻ�����
		traits->x = 0;
		traits->y = 0;
		//���ÿ�Ⱥ͸߶�
		traits->width = 1;
		traits->height = 1;
		//���ô�����չ
		traits->windowDecoration = false;
		//����˫����
		traits->doubleBuffer = false;
		traits->sharedContext = 0;
		//����pbuffer
		traits->pbuffer = true;

		//����ͼ�λ���
		_gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		//�������ʧ��
		if (!_gc)
		{
			//����pbufferΪfalse
			traits->pbuffer = false;
			//���´���ͼ�λ���
			_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		}

		//�Ƿ��ʼ��
		if (_gc.valid())
		{
			//��ʼ��
			_gc->realize();
			_gc->makeCurrent();
		}
	}

	bool valid() const { return _gc.valid() && _gc->isRealized(); }

private:
	osg::ref_ptr<osg::GraphicsContext> _gc;
};

//������ά��������
osg::ref_ptr<osg::StateSet> createState()
{
	//����ͼ�λ���
	/*
	MyGraphicsContext gc;
	if (!gc.valid())
	{
		//�������ʧ�ܣ��򷵻�
		osg::notify(osg::NOTICE) << "Unable to create the graphics context required to build 3d image." << std::endl;
		return 0;
	}
	*/

	//��ȡ���Ŷ�ά����ͼ��
	osg::ref_ptr<osg::Image> image_0 = osgDB::readImageFile("Images/lz.rgb");
	osg::ref_ptr<osg::Image> image_1 = osgDB::readImageFile("Images/reflect.rgb");
	osg::ref_ptr<osg::Image> image_2 = osgDB::readImageFile("Images/tank.rgb");
	osg::ref_ptr<osg::Image> image_3 = osgDB::readImageFile("Images/skymap.jpg");

	//�ж��Ƿ���ȷ��ȡ
	if (!image_0 || !image_1 || !image_2 || !image_3)
	{
		std::cout << "Warning: could not open files." << std::endl;

		return new osg::StateSet();
	}
	else {
		std::cout << "Read images success!" << std::endl;
	}

	//�ж������ʽ�Ƿ�һ��
	if (image_0->getPixelFormat() != image_1->getPixelFormat() || image_0->getPixelFormat() != image_2->getPixelFormat() || image_0->getPixelFormat() != image_3->getPixelFormat())
	{
		std::cout << "Warning: image pixel formats not compatible." << std::endl;

		return new osg::StateSet();
	}

	//�õ�֧�ֵ�������ά����Ԫ�Ĵ�С
	/*
	GLint textureSize = osg::Texture3D::getExtensions(0, true)->maxTexture3DSize();
	if (textureSize > 256)
	{
		textureSize = 256;
	}
	*/
	GLuint textureSize = 256;

	//�����Ŷ�ά����ͼ�����ţ��Դﵽ��ͬ�Ĵ�С
	image_0->scaleImage(textureSize, textureSize, 1);
	image_1->scaleImage(textureSize, textureSize, 1);
	image_2->scaleImage(textureSize, textureSize, 1);
	image_3->scaleImage(textureSize, textureSize, 1);

	//����һ����ά��������ͼ��ע�����Ϊ4
	osg::ref_ptr<osg::Image> image_3d = new osg::Image;
	//��һ���͵ڶ�������������Ĵ�С������������ָ������ά��������ͼ������
	image_3d->allocateImage(textureSize, textureSize, 4, image_0->getPixelFormat(), image_0->getDataType());

	//�����Ŷ�ά����ͼ��ѹ����ά��������ͼ��
	//��1-3�������ֱ���s,t,r�ϵ�ƫ�ƣ���Ȼ����ֻ��r�ϵ�ƫ��
	//���ĸ��������Ӷ�ά����ͼ������
	image_3d->copySubImage(0, 0, 0, image_0.get());
	image_3d->copySubImage(0, 0, 1, image_1.get());
	image_3d->copySubImage(0, 0, 2, image_2.get());
	image_3d->copySubImage(0, 0, 3, image_3.get());

	//���������ʽ
	image_3d->setInternalTextureFormat(image_0->getInternalTextureFormat());

	//������ά�������
	osg::ref_ptr<osg::Texture3D> texture3D = new osg::Texture3D;
	//�����˲�����֧��mip map�˲�
	texture3D->setFilter(osg::Texture3D::MIN_FILTER, osg::Texture3D::LINEAR);
	texture3D->setFilter(osg::Texture3D::MAG_FILTER, osg::Texture3D::LINEAR);
	//���û���ģʽ
	texture3D->setWrap(osg::Texture3D::WRAP_R, osg::Texture3D::REPEAT);
	//������ά����ͼ������
	texture3D->setImage(image_3d.get());

	//�����Զ�������������
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
	//�����Զ�������������Ϊ�Ӿ�����
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	//ָ���ο�ƽ��
	texgen->setPlane(osg::TexGen::R, osg::Plane(0.0f, 0.0f, 0.0f, 0.2f));

	//����״̬���Զ���
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
	//������R���Զ�������������
	stateset->setTextureMode(0, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	//�����Զ�������������
	stateset->setTextureAttribute(0, texgen.get());
	//������ά�������
	stateset->setTextureAttributeAndModes(0, texture3D.get(), osg::StateAttribute::ON);

	return stateset.get();
}


int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = createNode();

	//����״̬���Զ���
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = createState();

	//ʹ����ά����
	node->setStateSet(stateset.get());
	root->addChild(node.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);

	viewer->realize();

	viewer->run();
	return 0;
}