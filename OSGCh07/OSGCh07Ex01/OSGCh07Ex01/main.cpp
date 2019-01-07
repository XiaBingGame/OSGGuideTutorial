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
#include <fstream>

class VertexVisitor : public osg::NodeVisitor
{
public:
	VertexVisitor();
	virtual void apply(osg::Geode& geode);

	osg::ref_ptr<osg::Vec3Array> extracted_verts;
};

VertexVisitor::VertexVisitor() :
	osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	extracted_verts = new osg::Vec3Array;
}

void VertexVisitor::apply(osg::Geode& geode)
{
	for (size_t i = 0; i < geode.getNumDrawables(); i++)
	{
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
		if (!geom)
		{
			std::cout << "几何体错误!" << std::endl;
			continue;
		}

		osg::Vec3Array* verts = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		if (!verts)
		{
			std::cout << "顶点数组错误!" << std::endl;
			continue;
		}

		extracted_verts->insert(extracted_verts->end(), verts->begin(), verts->end());
	}
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	root->addChild(node.get());
	
	VertexVisitor vv;
	node->accept(vv);

	std::ofstream output_file("glider.vertexs");
	int size = vv.extracted_verts->size();
	output_file << "Vertex size: " << size << std::endl;

	std::vector<osg::Vec3>::iterator iter = vv.extracted_verts->begin();
	while (iter != vv.extracted_verts->end())
	{
		output_file << (*iter).x() << " " << (*iter).y() << " " << (*iter).z() << std::endl;
		iter++;
	}

	system("pause");
	return 0;
}