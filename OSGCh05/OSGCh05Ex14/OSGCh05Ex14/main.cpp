#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Material>
#include <osg/CullFace>

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

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = createNode();

	//�õ�״̬����
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = node->getOrCreateStateSet();

	//�������ʶ���
	osg::ref_ptr<osg::Material> mat = new osg::Material();
	//��������ɢ����ɫ
	mat->setDiffuse(osg::Material::FRONT, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//�������澵����ɫ
	mat->setSpecular(osg::Material::FRONT, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//��������ָ��
	mat->setShininess(osg::Material::FRONT, 90.0f);
	stateset->setAttribute(mat.get());

	//���ñ����޳�
	osg::ref_ptr<osg::CullFace> cullface = new osg::CullFace(osg::CullFace::BACK);
	stateset->setAttribute(cullface.get());
	stateset->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	// stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	root->addChild(node.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);
	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

	return 0;
}