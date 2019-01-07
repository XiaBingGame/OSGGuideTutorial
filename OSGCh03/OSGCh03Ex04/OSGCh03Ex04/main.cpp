#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/AutoTransform>

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


//�����Զ��任�ڵ�
osg::ref_ptr<osg::Node> createAutoTransform(osg::Vec3& position, float size, std::string& label,
	osg::AutoTransform::AutoRotateMode autoMode, osgText::Text::AxisAlignment axisAlignment)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	//����
	std::string font("fonts/cour.ttf");

	//����Text����
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	geode->addDrawable(text.get());

	//��������
	text->setFont(font);
	//��������ķֱ��ʣ�Ĭ��Ϊ32*32
	text->setFontResolution(128.0f, 128.0f);
	//��������Ĵ�С
	text->setCharacterSize(size);
	//���ö��뷽ʽ
	text->setAlignment(osgText::Text::CENTER_CENTER);
	//���÷���
	text->setAxisAlignment(axisAlignment);
	//��������
	text->setText(label);

	//�رչ���
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//�����Զ��任�ڵ�
	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform();
	//����ӽڵ�
	at->addChild(geode.get());

	//�����Զ��任��ʽ
	at->setAutoRotateMode(autoMode);
	//������Ļ��С�����Žڵ㣬Ĭ��Ϊfalse������Ϊtrueʱ���ڵ��޷�����
	at->setAutoScaleToScreen(false);
	//at->setAutoScaleToScreen(true) ;
	//�������ŵ�������С����
	at->setMinimumScale(0.0f);
	at->setMaximumScale(5.0f);
	//����λ��
	at->setPosition(position);

	return at.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::string text("Text 1");

	/*
	���ֱ任ģʽ��
	ROTATE_TO_SCREEN�����Զ�������Ļ
	ROTATE_TO_CAMERA	�Զ��������
	NO_ROTATION			��
	*/
	//���ROTATE_TO_SCEREENģʽ�任�ڵ�
	root->addChild(createAutoTransform(osg::Vec3(0.0f, 0.0f, 0.0f), 60.0f, text,
		osg::AutoTransform::ROTATE_TO_SCREEN, osgText::Text::XY_PLANE));

	std::string text2("Text 2");
	//���NO_ROTATIONģʽ�任�ڵ�
	root->addChild(createAutoTransform(osg::Vec3(0.0f, 0.0f, 0.0f), 60.0f, text2,
		osg::AutoTransform::NO_ROTATION, osgText::Text::XZ_PLANE));

	//���ROTATE_TO_CAMERAģʽ�任�ڵ�
	//root->addChild(createAutoTransform(osg::Vec3(0.0f,0.0f,0.0f),60.0f,text,
	//	osg::AutoTransform::ROTATE_TO_CAMERA,osgText::Text::XY_PLANE)) ;

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}