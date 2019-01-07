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


osg::ref_ptr<osg::Node> createBillboardTree(osg::ref_ptr<osg::Image> image)
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
		//״̬���Զ���
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

		//����һ��Texture2D���Զ���
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		//����image
		texture->setImage(image.get());
		//����Texture2D������󣬵���������Ĭ��ΪON
		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
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
	//������תģʽΪ����ת����˻���Ҫ����ת��
	billboard2->setMode(osg::Billboard::AXIAL_ROT);
	//������ת��
	billboard2->setAxis(osg::Vec3(0.0f, 0.0f, 1.0f));
	billboard2->addDrawable(geometry.get(), osg::Vec3(10.0f, 0.0f, 0.0f));

	osg::ref_ptr<osg::Group> billboard = new osg::Group();
	billboard->addChild(billboard1.get());
	billboard->addChild(billboard2.get());

	return billboard.get();

}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡͼ��
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/tree0.rgba");

	//����һ�£��Դﵽ���ʵĴ�С
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	pat->setScale(osg::Vec3(5.0f, 5.0f, 5.0f));
	pat->addChild(createBillboardTree(image.get()));

	root->addChild(pat.get());

	//��ȡcow��ģ�ͣ��ԶԱ�
	root->addChild(osgDB::readNodeFile("cow.osg"));

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}