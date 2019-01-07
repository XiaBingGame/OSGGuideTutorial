#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>
#include <osgUtil/TriStripVisitor>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/StateSetManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

//����һ���ı��νڵ�
osg::ref_ptr<osg::Geometry> createQuad()
{
	//����һ��Ҷ�ڵ����
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	//����һ�����������
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//�����������飬ע�ⶥ������˳������ʱ���
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	//�������
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	//���ö�������
	geom->setVertexArray(v.get());

	//������������
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	//�������
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));

	//������������
	geom->setTexCoordArray(0, vt.get());

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	//�������
	vc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	vc->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	//������ɫ����
	geom->setColorArray(vc.get());
	//������ɫ�İ󶨷�ʽΪ��������
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//������������
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//��ӷ���
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	//���÷�������
	geom->setNormalArray(nc.get());
	//���÷��ߵİ󶨷�ʽΪȫ������
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//���ͼԪ����ͼ��ԪΪ�ı���
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	return geom.get();
}

int main(int argc, char** argv)
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//����鿴�ڶ����֮���л����Բ鿴������
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ�����������
	osg::ref_ptr<osg::Geometry> geometry = createQuad();

	//�Լ��������������
	osgUtil::TriStripVisitor stripper;
	stripper.stripify(*(geometry.get()));

	//��ӵ�Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geometry.get());

	//��ӵ�����
	root->addChild(geode.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}