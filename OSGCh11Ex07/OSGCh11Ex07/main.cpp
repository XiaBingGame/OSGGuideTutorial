#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/NodeCallback>

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

//创建一个新的变换类
class ParticleTransform : public osg::MatrixTransform
{
public:
	class ParticleTransformCallback : public osg::NodeCallback
	{
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			//得到节点
			if (ParticleTransform* ps = dynamic_cast<ParticleTransform*>(node))
			{
				osg::NodePath& fullNodePath = nv->getNodePath();
				
				if (!fullNodePath.empty())
				{
					std::cout << fullNodePath.size() << std::endl;
					// fullNodePath.pop_back();
					//反转各种矩阵变换
					osg::Matrix localCoordMat = osg::computeLocalToWorld(fullNodePath);
					osg::Matrix inverseOfAccum = osg::Matrix::inverse(localCoordMat);
					//设置矩阵
					ps->setMatrix(inverseOfAccum);
				}
				
			}
			traverse(node, nv);
		}
	};

	ParticleTransform()
	{
		//设置更新回调
		setUpdateCallback(new ParticleTransformCallback());
	}

};
//更新回调
class orbit : public osg::NodeCallback
{
public:
	//构造函数
	orbit() : _angle(0.0) {}
	//返回局部矩阵
	osg::Matrix getWCMatrix() { return m; }

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::MatrixTransform> tx = dynamic_cast<osg::MatrixTransform *>(node);
		if (tx != NULL)
		{
			_angle += 0.01;
			//设置矩阵旋转
			tx->setMatrix(osg::Matrix::rotate(_angle, 0.0f, 0.0f, 1.0f));
			//计算由世界坐标到局部坐标
			m = osg::computeWorldToLocal(nv->getNodePath());
		}
		traverse(node, nv);
	}
private:
	//矩阵
	osg::Matrix m;
	//角度
	float _angle;
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	//读取奶牛模型
	osg::ref_ptr<osg::Node> tankNode = osgDB::readNodeFile("cow.osg");
	//读取粒子系统模型
	osg::ref_ptr<osg::Node> particleNode = osgDB::readNodeFile("myvr.osg");
	//添加矩阵变换节点
	osg::ref_ptr<osg::MatrixTransform> tankTransform = new osg::MatrixTransform();
	//设置更新回调
	tankTransform->setUpdateCallback(new orbit());
	//添加子节点
	rootNode->addChild(tankTransform.get());
	tankTransform->addChild(tankNode.get());
	//创建粒子系统变换节点
	ParticleTransform* my = new ParticleTransform();
	//添加子节点
	my->addChild(particleNode.get());

	tankTransform->addChild(my);
	//优化场景数据
	viewer->setUpViewInWindow(100, 100, 800, 600);
	osgUtil::Optimizer optimizer;
	optimizer.optimize(rootNode.get());

	viewer->setSceneData(rootNode.get());

	viewer->realize();

	viewer->run();

	return 0;
}