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
#include <map>

class TextureVisitor : public osg::NodeVisitor
{
public:
	// 构造函数, 遍历所有子节点
	TextureVisitor() : 
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{}

	virtual void apply(osg::Node& node);
	virtual void apply(osg::Geode& geode);
	
	void apply(osg::StateSet* state);

	std::map<std::string, osg::Image*>& getImages()	
	{ return _imageList; }

protected:
	std::map<std::string, osg::Image*> _imageList;
};

// 重载 apply() 方法
void TextureVisitor::apply(osg::Node& node)
{
	if (node.getStateSet()) {
		apply(node.getStateSet());
	}

	traverse(node);
}

void TextureVisitor::apply(osg::Geode& geode)
{
	if (geode.getStateSet())
	{
		apply(geode.getStateSet());
	}

	unsigned int cnt = geode.getNumDrawables();

	for (unsigned int i = 0; i < cnt; i++)
	{
		apply(geode.getDrawable(i)->getStateSet());
	}

	traverse(geode);
}

// 得到贴图列表
void TextureVisitor::apply(osg::StateSet* state)
{
	osg::StateSet::TextureAttributeList& texAttribList = state->getTextureAttributeList();
	for (size_t i = 0; i < texAttribList.size(); i++)
	{
		osg::Texture2D* tex2D = NULL;
		tex2D = dynamic_cast<osg::Texture2D*>(state->getTextureAttribute(i, osg::StateAttribute::TEXTURE));
		if (tex2D)
		{
			if (tex2D->getImage())
			{
				_imageList.insert(std::make_pair(tex2D->getImage()->getFileName(), tex2D->getImage()));
			}
		}
	}
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	TextureVisitor tv;
	node->accept(tv);

	std::map<std::string, osg::Image*> imageList = tv.getImages();
	std::map<std::string, osg::Image*>::iterator it = imageList.begin();
	unsigned int cnt = 0;

	char* buffer = new char[2000];
	for (; it != imageList.end(); it++)
	{
		sprintf(buffer, "TextureImage%d.jpg", cnt++);
		osgDB::writeImageFile(*(it->second), buffer);
		
	}

	delete[] buffer;

	return 0;
}