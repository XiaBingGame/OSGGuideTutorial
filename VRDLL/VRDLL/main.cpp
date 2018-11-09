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
#include <osgDB/FileNameUtils>
#include <osgDB/ReaderWriter>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

#define N 10

class ReaderWriterVR : public osgDB::ReaderWriter
{
public:
	ReaderWriterVR() {}

	const char* className() const { return "VR Reader/Writer"; }

	bool acceptsExtension(const std::string& extension) const {
		return osgDB::equalCaseInsensitive(extension, "VR");
	}

	virtual ReadResult readNode(const std::string& fileName, const Options* options) const
	{
		std::string ext(osgDB::getLowerCaseFileExtension(fileName));
		if (!acceptsExtension(ext))
		{
			return ReadResult::FILE_NOT_FOUND;
		}

		osg::notify(osg::INFO) << "ReaderWriteVR( \"" << fileName << "\" )" << std::endl;
		osg::ref_ptr<osg::Node> node = new osg::Node();
		
		std::string tempFileNmae("CopyRight.ive");

		std::ifstream in(fileName.c_str(), std::ios::in | std::ios::binary);
		if (!in)
		{
			osg::notify(osg::WARN) << "Subfile could not be loaded" << std::endl;
		}

		std::ofstream fout(tempFileNmae.c_str(), std::ios::out | std::ios::binary);

		int j = 0;
		char fileContent;
		while (!in.eof())
		{
			j++;
			if (j != N)
			{
				in.get(fileContent);
				fout.put(fileContent);
			}
			else
			{
				in.get(fileContent);
			}
			fileContent = ' ';
		}
		in.close();
		fout.close();

		node = osgDB::readNodeFile(tempFileNmae);

		std::ofstream foutCopyRight(tempFileNmae.c_str(), std::ios::out | std::ios::binary);
		foutCopyRight << "CopyRight@FlySky" << std::endl;
		foutCopyRight << "Email: zzuxp@163.com" << std::endl;
		foutCopyRight.close();
		return node.get();
	}

	virtual WriteResult writeNode(const osg::Node& node, const std::string& fileName, const Options* options) const
	{
		std::string ext(osgDB::getLowerCaseFileExtension(fileName));
		if (!acceptsExtension(ext))
		{
			return WriteResult::FILE_NOT_HANDLED;
		}

		std::string dealFileName("CopyRight.ive");
		osgDB::writeNodeFile(node, dealFileName);

		std::ifstream in(dealFileName.c_str(), std::ios::in | std::ios::binary);
		std::ofstream fout(fileName.c_str(), std::ios::out | std::ios::binary);
		if (!in)
		{
			std::cout << "请确定文件是否存在或者文件名是否正确 ～～～～" << std::endl;
			std::cout << "No Data Load" << std::endl;

			exit(-1);
		}

		int i = 0;

		char fileContent;

		while (!in.eof())
		{
			i++;

			if (i == N)
			{
				fout.put('a');
			}

			in.get(fileContent);

			fout.put(fileContent);

			fileContent = ' ';
		}

		in.close();
		fout.close();

		std::ofstream foutCopyRight(dealFileName.c_str(), std::ios::out | std::ios::binary);

		foutCopyRight << "CopyRight@FlySky" << std::endl;
		foutCopyRight << "Email:zzuxp@163.com" << std::endl;

		foutCopyRight.close();

		return WriteResult::FILE_SAVED;
	}
};

//为了实现插件注册，定义全局变量
REGISTER_OSGPLUGIN(VR, ReaderWriterVR)