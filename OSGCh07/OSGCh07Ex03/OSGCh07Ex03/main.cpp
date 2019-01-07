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

#include "FindNodeVisitor.h"

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//����һ���ڵ�,��ȡţ��ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	FindNodeVisitor fv(std::string("cow.osg"));
	node->accept(fv);

	if (fv.getFirst())
	{
		std::cout << "���ҽڵ�ɹ�" << std::endl;
	}
	else
	{
		std::cout << "���ҽڵ㲻�ɹ�" << std::endl;
	}

	system("pause");
	return 0;
}