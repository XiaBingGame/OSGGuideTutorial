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

class MyReadFileCallback : public osgDB::Registry::ReadFileCallback
{
public:
	// 重载该虚函数
	virtual osgDB::ReaderWriter::ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options)
	{
		std::cout << "Starting read node " << std::endl;
		// 得到时间
		osg::Timer_t start = osg::Timer::instance()->tick();
		osgDB::ReaderWriter::ReadResult result = osgDB::Registry::instance()->readNodeImplementation(fileName, options);
		std::cout << "Ending read node" << std::endl;

		std::cout << "The time of reading Node ===== " <<
			osg::Timer::instance()->delta_s(start, osg::Timer::instance()->tick()) << "  s" << std::endl;

		return result;
	}
};

class MyWriteFileCallback : public osgDB::Registry::WriteFileCallback
{
public:
	// 重载该虚函数
	virtual osgDB::ReaderWriter::WriteResult writeNode(const osg::Node &obj, const std::string &fileName, const osgDB::ReaderWriter::Options *options)
	{
		std::cout << "Starting write node " << std::endl;
		// 得到时间
		osg::Timer_t start = osg::Timer::instance()->tick();
		osgDB::ReaderWriter::WriteResult result = osgDB::Registry::instance()->writeNodeImplementation(obj, fileName, options);
		std::cout << "Ending write node" << std::endl;

		std::cout << "The time of writing Node ===== " <<
			osg::Timer::instance()->delta_s(start, osg::Timer::instance()->tick()) << "  s" << std::endl;

		return result;
	}
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osgDB::Registry::instance()->setReadFileCallback(new MyReadFileCallback());
	osgDB::Registry::instance()->setWriteFileCallback(new MyWriteFileCallback());

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取spaceship模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("spaceship.osgt");

	root->addChild(node.get());

	osgDB::writeNodeFile(*(root.get()), std::string("root.osg"));
	
	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();

	return 0;
}