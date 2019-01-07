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


//创建自动变换节点
osg::ref_ptr<osg::Node> createAutoTransform(osg::Vec3& position, float size, std::string& label,
	osg::AutoTransform::AutoRotateMode autoMode, osgText::Text::AxisAlignment axisAlignment)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	//字体
	std::string font("fonts/cour.ttf");

	//创建Text对象
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	geode->addDrawable(text.get());

	//设置字体
	text->setFont(font);
	//设置字体的分辨率，默认为32*32
	text->setFontResolution(128.0f, 128.0f);
	//设置字体的大小
	text->setCharacterSize(size);
	//设置对齐方式
	text->setAlignment(osgText::Text::CENTER_CENTER);
	//设置方向
	text->setAxisAlignment(axisAlignment);
	//设置文字
	text->setText(label);

	//关闭光照
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//创建自动变换节点
	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform();
	//添加子节点
	at->addChild(geode.get());

	//设置自动变换方式
	at->setAutoRotateMode(autoMode);
	//根据屏幕大小来缩放节点，默认为false，设置为true时，节点无法缩放
	at->setAutoScaleToScreen(false);
	//at->setAutoScaleToScreen(true) ;
	//设置缩放的最大和最小比例
	at->setMinimumScale(0.0f);
	at->setMaximumScale(5.0f);
	//设置位置
	at->setPosition(position);

	return at.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::string text("Text 1");

	/*
	三种变换模式：
	ROTATE_TO_SCREEN　　自动朝向屏幕
	ROTATE_TO_CAMERA	自动朝向相机
	NO_ROTATION			无
	*/
	//添加ROTATE_TO_SCEREEN模式变换节点
	root->addChild(createAutoTransform(osg::Vec3(0.0f, 0.0f, 0.0f), 60.0f, text,
		osg::AutoTransform::ROTATE_TO_SCREEN, osgText::Text::XY_PLANE));

	std::string text2("Text 2");
	//添加NO_ROTATION模式变换节点
	root->addChild(createAutoTransform(osg::Vec3(0.0f, 0.0f, 0.0f), 60.0f, text2,
		osg::AutoTransform::NO_ROTATION, osgText::Text::XZ_PLANE));

	//添加ROTATE_TO_CAMERA模式变换节点
	//root->addChild(createAutoTransform(osg::Vec3(0.0f,0.0f,0.0f),60.0f,text,
	//	osg::AutoTransform::ROTATE_TO_CAMERA,osgText::Text::XY_PLANE)) ;

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}