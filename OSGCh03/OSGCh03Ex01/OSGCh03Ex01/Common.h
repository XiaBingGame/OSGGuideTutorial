#pragma once
#include <string>
#include <osg/Vec3>
#include <osgText/Font>
#include <osgText/Text>

namespace osg
{
	class Camera;
}
namespace osgText
{
	class Text;
}

namespace osgCookBook
{
	osg::Camera* createHUDCamera(double left, double right, double bottom, double top);
	osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size);
}
