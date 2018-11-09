#include "FindNodeVisitor.h"



FindNodeVisitor::FindNodeVisitor(std::string& searchName)
	: osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
	searchForName(searchName)
{
}


FindNodeVisitor::~FindNodeVisitor()
{
}

void FindNodeVisitor::apply(osg::Node& node)
{
	if (node.getName() == searchForName)
	{
		foundNodeList.push_back(&node);
	}

	traverse(node);
}

void FindNodeVisitor::apply(osg::Geode& geode)
{
	apply((osg::Node&)geode);
	traverse((osg::Node&)geode);
}

void FindNodeVisitor::apply(osg::Transform& transform)
{
	apply((osg::Node&)transform);
	traverse(transform);
}

void FindNodeVisitor::setNameToFound(const std::string searchName)
{
	searchForName = searchName;
	foundNodeList.clear();
}

osg::Node* FindNodeVisitor::getFirst()
{
	if (foundNodeList.size() > 0)
		return *(foundNodeList.begin());
	return 0;
}
