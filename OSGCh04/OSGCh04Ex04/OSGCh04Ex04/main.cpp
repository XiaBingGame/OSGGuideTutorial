/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgUtil/Tessellator>

//ʹ�÷ָ񻯻��ư������
osg::ref_ptr<osg::Geode> tesslatorGeometry () 
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom= new osg::Geometry();
	geode->addDrawable(geom.get()) ;

	//������һЩ��������
	//ǽ
	const float wall[5][3] =
	{ { 2200.0f, 0.0f, 1130.0f },
	{ 2600.0f, 0.0f, 1130.0f },  
	{ 2600.0f, 0.0f, 1340.0f }, 
	{ 2400.0f, 0.0f, 1440.0f },
	{ 2200.0f, 0.0f, 1340.0f } };

	//��
	const float door[4][3] =
	{ { 2360.0f, 0.0f, 1130.0f },
	{ 2440.0f, 0.0f, 1130.0f }, 
	{ 2440.0f, 0.0f, 1230.0f },
	{ 2360.0f, 0.0f, 1230.0f } };

	//���ȴ���
	const float windows[16][3] =
	{ { 2240.0f, 0.0f, 1180.0f },
	{ 2330.0f, 0.0f,1180.0f }, 
	{ 2330.0f, 0.0f, 1220.0f },
	{ 2240.0f, 0.0f, 1220.0f }, 
	{ 2460.0f, 0.0f, 1180.0f },
	{ 2560.0f, 0.0f, 1180.0f }, 
	{ 2560.0f, 0.0f, 1220.0f },
	{ 2460.0f, 0.0f, 1220.0f },
	{ 2240.0f, 0.0f, 1280.0f },
	{ 2330.0f, 0.0f, 1280.0f }, 
	{ 2330.0f, 0.0f, 1320.0f },
	{ 2240.0f, 0.0f, 1320.0f }, 
	{ 2460.0f, 0.0f, 1280.0f },
	{ 2560.0f, 0.0f, 1280.0f }, 
	{ 2560.0f, 0.0f, 1320.0f },
	{ 2460.0f, 0.0f, 1320.0f } };

	//���ö�������
	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array();
	geom->setVertexArray(coords.get());

	//���÷���
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	normal->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	geom->setNormalArray(normal.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//���ǽ
	for (int i = 0; i < 5; i++) 
	{
		coords->push_back(osg::Vec3(wall[i][0],wall[i][1],wall[i][2]));

	}
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,5));


	//�����
	for (int i = 0; i < 4; i++) 
	{
		coords->push_back(osg::Vec3(door[i][0],door[i][1],door[i][2]));
	}

	//��Ӵ�
	for (int i = 0; i < 16; i++) 
	{
		coords->push_back(osg::Vec3(windows[i][0],windows[i][1],windows[i][2]));
	}
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,5,20));

	//�����ָ񻯶���
	osg::ref_ptr<osgUtil::Tessellator> tscx=new osgUtil::Tessellator();
	//���÷ָ�����Ϊ������
	tscx->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
	//����ֻ��ʾ������Ϊfalse�����ﻹ��Ҫ���
	tscx->setBoundaryOnly(false);
	//���û��ƹ���
	tscx->setWindingType( osgUtil::Tessellator::TESS_WINDING_ODD);

	//ʹ�÷ָ�
	tscx->retessellatePolygons(*(geom.get()));

	return geode.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ӻ��ƵĶ����
	osg::ref_ptr<osg::Geode> geode = tesslatorGeometry() ;

	root->addChild(geode.get());

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0 ;
}