#pragma once
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgViewer/View>

namespace osgCookBook
{
	class PickHandler :
		public osgGA::GUIEventHandler
	{
	public:
		// 派生类必须实现该函数
		virtual void doUserOperations(const osgUtil::LineSegmentIntersector::Intersection&) = 0;
	
		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
		{
			if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE ||
				ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
				!(ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL))
				return false;
	
			osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
			if (viewer)
			{
				osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
					new osgUtil::LineSegmentIntersector(
						osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
				
				osgUtil::IntersectionVisitor iv(intersector.get());
				viewer->getCamera()->accept(iv);
	
				if (intersector->containsIntersections())
				{
					const osgUtil::LineSegmentIntersector::Intersection&
						result = *(intersector->getIntersections().begin());
					doUserOperations(result);
				}
			}
		}
	};
}
