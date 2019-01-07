#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Sequence>

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

osg::ref_ptr<osg::Node> createBillboard(osg::ref_ptr<osg::Image> image)
{
	//�����ı���
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	//���ö���
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(-0.5f, 0.0f, -0.5f));
	v->push_back(osg::Vec3(0.5f, 0.0f, -0.5f));
	v->push_back(osg::Vec3(0.5f, 0.0f, 0.5f));
	v->push_back(osg::Vec3(-0.5f, 0.0f, 0.5f));

	geometry->setVertexArray(v.get());

	//���÷���
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	normal->push_back(osg::Vec3(1.0f, 0.0f, 0.0f) ^ osg::Vec3(0.0f, 0.0f, 1.0f));

	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//������������
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));

	geometry->setTexCoordArray(0, vt.get());

	//�����ı���
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	if (image.get())
	{
		//���Զ���
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

		//����һ��Texture2D���Զ���
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		//����image
		texture->setImage(image.get());

		//����Texture2D������󣬵���������Ĭ��ΪON
		stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
		//���û��
		stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		//�رչ���
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		geometry->setStateSet(stateset.get());
	}

	//����Billboard����һ
	osg::ref_ptr<osg::Billboard> billboard1 = new osg::Billboard();

	//������תģʽΪ���ӵ�
	billboard1->setMode(osg::Billboard::POINT_ROT_EYE);
	//���Drawable����������λ�ã�Ĭ��λ��Ϊosg::Vec3(0.0f,0.0f,0.0f) ;
	billboard1->addDrawable(geometry.get(), osg::Vec3(5.0f, 0.0f, 0.0f));

	osg::ref_ptr<osg::Billboard> billboard2 = new osg::Billboard();

	osg::ref_ptr<osg::Group> billboard = new osg::Group();
	billboard->addChild(billboard1.get());

	return billboard.get();

}
//����֡����
osg::ref_ptr<osg::Sequence> createSequence()
{
	//����֡��������
	osg::ref_ptr<osg::Sequence> seq = new osg::Sequence;

	//�ļ�����������
	typedef std::vector<std::string> Filenames;
	Filenames filenames;

	for (int i = 0; i < 60; i++)
	{
		char name_count[10];
		sprintf(name_count, "bz%d.jpg", i);
		filenames.push_back(name_count);
	}

	for (Filenames::iterator itr = filenames.begin(); itr != filenames.end(); ++itr)
	{
		//����ģ��
		osg::Image* image = osgDB::readImageFile(*itr);

		if (image)
		{
			//����ӽڵ�
			seq->addChild(createBillboard(image));
			//�趨�ڵ�ĳ���ʱ��
			seq->setTime(seq->getNumChildren() - 1, 0.1f);
		}
	}

	//����֡����������ʱ��
	seq->setInterval(osg::Sequence::LOOP, 0, -1);

	//���ò��ŵ��ٶȼ��ظ��Ĵ���
	seq->setDuration(1.0f, -1);

	//��ʼ����
	seq->setMode(osg::Sequence::START);

	return seq.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//�򳡾������֡����
	osg::ref_ptr<osg::Sequence> sq = new osg::Sequence();
	sq = createSequence();

	root->addChild(sq.get());

	viewer->setUpViewInWindow(100, 100, 800, 600);
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}