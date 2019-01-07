#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

//��������PagedLOD����
osg::ref_ptr<osg::Group> createPagedLOD()
{
	//����PagedLOD����
	osg::ref_ptr<osg::PagedLOD> page = new osg::PagedLOD();
	//��������λ��
	page->setCenter(osg::Vec3(0.0f, 0.0f, 0.0f));

	//��ӽڵ㣬����0��������Ϊcow.osg
	page->setFileName(0, "cow.osg");
	//���ÿ��ӱ仯��ΧΪ0.0f-50.0f
	page->setRange(0, 0.0f, 50.0f);

	//��ӽڵ㣬����0��������Ϊspaceship.osg
	page->setFileName(1, "glider.osg");
	//���ÿ��ӱ仯��Χ50.0f-100.0f
	page->setRange(1, 50.0f, 100.0f);

	//��ӽڵ㣬����0��������Ϊcessna.osg
	page->setFileName(2, "cessna.osg");
	//���ÿ��ӱ仯��Χ100.0f-200.0f
	page->setRange(2, 100.0f, 200.0f);

	return page.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��������任�ڵ�
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	//����ӽڵ�
	mt->addChild(createPagedLOD());
	//��תһ�£�����һ�º��ʵĽǶ�
	osg::Matrix m;
	m.makeRotate(60.0f, 0.0f, 0.0f, 1.0f);
	//���þ���
	mt->setMatrix(m);

	//���PagedLOD����
	root->addChild(mt.get());

	//д��PagedLOD
	osgDB::writeNodeFile(*root, "page.osg");

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();


	while (!viewer->done()) 
		viewer->frame();
	//viewer->run();
	return 0;
}