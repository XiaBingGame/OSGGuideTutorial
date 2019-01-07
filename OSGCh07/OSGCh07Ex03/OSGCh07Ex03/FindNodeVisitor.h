#ifndef __FIND_NODE_VISITOR_H__
#define __FIND_NODE_VISITOR_H__

#include <osg/NodeVisitor>
#include <osg/Node>

#include <osgSim/DOFTransform>

#include <iostream>
#include <string>
#include <vector>

class FindNodeVisitor : public osg::NodeVisitor
{
public:
	typedef std::vector<osg::Node*> nodelistType;

	FindNodeVisitor(std::string& searchName);
	~FindNodeVisitor();

	virtual void apply(osg::Node& node);
	virtual void apply(osg::Geode& geode);
	virtual void apply(osg::Transform& transform);

	void setNameToFound(const std::string searchName);
	osg::Node* getFirst();

	nodelistType& getNodeList() { return foundNodeList; }
private:
	std::string searchForName;
	nodelistType foundNodeList;
};

#endif
