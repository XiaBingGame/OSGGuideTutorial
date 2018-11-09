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


//自定义Copy类，用于输出拷贝信息
class GraphCopyOp : public osg::CopyOp
{
public:

	inline GraphCopyOp
		(CopyFlags flags = SHALLOW_COPY) :
		osg::CopyOp(flags)
	{
		_nodeCopyMap.clear();
	}

	//节点的copy
	virtual osg::Node* operator() (const osg::Node* node) const
	{
		//判断是否为深拷贝
		if (node && _flags&DEEP_COPY_NODES)
		{
			//判断节点的父节点是否有多个，如果只有一个，则直接深拷贝
			if (node->getNumParents() > 1)
			{
				//判断是否已经拷贝
				if (_nodeCopyMap.find(node) != _nodeCopyMap.end())
				{
					std::cout << "Copy of node " << node << " " << node->getName() << ", "
						<< _nodeCopyMap[node] << ", will be reused" << std::endl;
					//直接返回拷贝对象的地址
					return (osg::Node*)(_nodeCopyMap[node]);
				}
				else
				{
					//进行拷贝并在映射表中保存拷贝对象
					osg::Node* newNode = dynamic_cast<osg::Node*>(node->clone(*this));
					_nodeCopyMap[node] = newNode;
					//返回地址
					return newNode;
				}
			}
			else
			{
				//直接拷贝
				return dynamic_cast<osg::Node*>(node->clone(*this));
			}
		}
		else
		{
			//直接返回地址
			return const_cast<osg::Node*>(node);
		}
	}

protected:

	//节点映射表，用来保存child和拷贝，分别作为原象和象
	mutable std::map<const osg::Node*, osg::Node*> _nodeCopyMap;
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//创建一个多父节点共享一个子节点的场景
	osg::ref_ptr<osg::Group> rootnode = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	//子节点pat，共享node
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	pat->setPosition(osg::Vec3(5.0f, 0.0f, 0.0f));
	pat->addChild(node.get());

	//子节点mt，共享node
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	osg::Matrix m;
	m.makeScale(2.0f, 2.0f, 2.0f);
	mt->setMatrix(m);
	mt->addChild(node.get());

	//添加子节点
	rootnode->addChild(pat.get());
	rootnode->addChild(mt.get());

	//优化场景数据
	osgUtil::Optimizer optimzer;
	optimzer.optimize(rootnode.get());

	//一个多父节点一个子节点的场景的默认深拷贝
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(rootnode->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout << std::endl << "完成一个多父节点一个子节点的场景的默认深拷贝" << std::endl << std::endl;
	osgDB::writeNodeFile(*(deepCopy.get()), "deepCopy.osg");

	//完成一个多父节点一个子节点的场景的浅拷贝
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(rootnode->clone(GraphCopyOp(osg::CopyOp::SHALLOW_COPY)));
	std::cout << std::endl << "完成一个多父节点一个子节点的场景的浅拷贝" << std::endl << std::endl;
	osgDB::writeNodeFile(*(myShallowCopy.get()), "myShallowCopy.osg");

	//完成一个多父节点一个子节点的场景的深拷贝
	osg::ref_ptr<osg::Node> myDeepCopy = dynamic_cast<osg::Node*>(rootnode->clone(GraphCopyOp(osg::CopyOp::DEEP_COPY_ALL)));
	std::cout << std::endl << "完成一个多父节点一个子节点的场景的深拷贝" << std::endl << std::endl;
	osgDB::writeNodeFile(*(myDeepCopy.get()), "myDeepCopy.osg");

	viewer->setSceneData(rootnode.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();
	return 0;
}