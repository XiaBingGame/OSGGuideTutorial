#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/TextureRectangle>
#include <osg/TexMat>

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

//������ά����״̬����
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::ref_ptr<osg::Image> image)
{
	//����״̬������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//������ά�������
	osg::ref_ptr<osg::TextureRectangle> texture = new osg::TextureRectangle();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//������ͼ
	texture->setImage(image.get());

	//����������󣬲�����Ϊ���ݾ�������(TextureRectangle)�Ĵ�С�Զ�����
	//�Ӷ�����Ӧ��һ����������һ���������겻��0-1��
	osg::ref_ptr<osg::TexMat> texmat = new osg::TexMat;
	texmat->setScaleByTextureRectangleSize(true);

	//���������������
	stateset->setTextureAttributeAndModes(0, texmat.get(), osg::StateAttribute::ON);
	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

	//�رչ���
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return stateset.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡ��ͼ�ļ�
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");

	osg::ref_ptr<osg::Node> node = createNode();

	//����״̬������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = createTexture2DState(image.get());

	//ʹ�ö�ά����
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