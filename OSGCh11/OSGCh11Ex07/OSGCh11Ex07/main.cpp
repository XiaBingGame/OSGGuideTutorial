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

//����һ���µı任��
class ParticleTransform : public osg::MatrixTransform
{
public:
	class ParticleTransformCallback : public osg::NodeCallback
	{
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			//�õ��ڵ�
			if (ParticleTransform* ps = dynamic_cast<ParticleTransform*>(node))
			{
				osg::NodePath& fullNodePath = nv->getNodePath();
				
				if (!fullNodePath.empty())
				{
					std::cout << fullNodePath.size() << std::endl;
					// fullNodePath.pop_back();
					//��ת���־���任
					osg::Matrix localCoordMat = osg::computeLocalToWorld(fullNodePath);
					osg::Matrix inverseOfAccum = osg::Matrix::inverse(localCoordMat);
					//���þ���
					ps->setMatrix(inverseOfAccum);
				}
				
			}
			traverse(node, nv);
		}
	};

	ParticleTransform()
	{
		//���ø��»ص�
		setUpdateCallback(new ParticleTransformCallback());
	}

};
//���»ص�
class orbit : public osg::NodeCallback
{
public:
	//���캯��
	orbit() : _angle(0.0) {}
	//���ؾֲ�����
	osg::Matrix getWCMatrix() { return m; }

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::MatrixTransform> tx = dynamic_cast<osg::MatrixTransform *>(node);
		if (tx != NULL)
		{
			_angle += 0.01;
			//���þ�����ת
			tx->setMatrix(osg::Matrix::rotate(_angle, 0.0f, 0.0f, 1.0f));
			//�������������굽�ֲ�����
			m = osg::computeWorldToLocal(nv->getNodePath());
		}
		traverse(node, nv);
	}
private:
	//����
	osg::Matrix m;
	//�Ƕ�
	float _angle;
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	//��ȡ��ţģ��
	osg::ref_ptr<osg::Node> tankNode = osgDB::readNodeFile("cow.osg");
	//��ȡ����ϵͳģ��
	osg::ref_ptr<osg::Node> particleNode = osgDB::readNodeFile("myvr.osg");
	//��Ӿ���任�ڵ�
	osg::ref_ptr<osg::MatrixTransform> tankTransform = new osg::MatrixTransform();
	//���ø��»ص�
	tankTransform->setUpdateCallback(new orbit());
	//����ӽڵ�
	rootNode->addChild(tankTransform.get());
	tankTransform->addChild(tankNode.get());
	//��������ϵͳ�任�ڵ�
	ParticleTransform* my = new ParticleTransform();
	//����ӽڵ�
	my->addChild(particleNode.get());

	tankTransform->addChild(my);
	//�Ż���������
	viewer->setUpViewInWindow(100, 100, 800, 600);
	osgUtil::Optimizer optimizer;
	optimizer.optimize(rootNode.get());

	viewer->setSceneData(rootNode.get());

	viewer->realize();

	viewer->run();

	return 0;
}