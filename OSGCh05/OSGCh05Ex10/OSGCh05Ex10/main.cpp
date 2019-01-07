#include <Windows.h>

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/Texture1D>
#include <osg/TexEnv>
#include <osg/StateSet>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//����һά��������
osg::ref_ptr<osg::StateSet> createTexture1DState()
{
	//������ͼ����ʵ������һ���߶�Ϊ1�Ķ�άͼ��
	osg::ref_ptr<osg::Image> image = new osg::Image;

	//Ϊimage����һ���ռ�
	image->allocateImage(1024, 1, 1, GL_RGBA, GL_FLOAT);
	//��������ͼ�����ݸ�ʽRGBA
	image->setInternalTextureFormat(GL_RGBA);

	//Ϊimage�������
	osg::Vec4* dataPtr = (osg::Vec4*)image->data();
	for (int i = 0; i < 1024; ++i)
	{
		*dataPtr++ = osg::Vec4(float(i)/1024.0, 0.5f, 0.8f, 0.5f);
	}

	//����һά����
	osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D();
	//���û���ģʽ
	texture->setWrap(osg::Texture1D::WRAP_S, osg::Texture1D::MIRROR);
	//�����˲�
	texture->setFilter(osg::Texture1D::MIN_FILTER, osg::Texture1D::LINEAR);
	//������ͼ
	texture->setImage(image.get());

	//�����Զ��������꣬��ָ����ص�ƽ��
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S, osg::Plane(0.0f, 0.0f, 1.0f, -10000));

	//�������Լ�
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

	//����һά����
	stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_1D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//��������������
	stateset->setTextureAttribute(0, texgen.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	return stateset.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");

	//ʹ��һά��������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = createTexture1DState();

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