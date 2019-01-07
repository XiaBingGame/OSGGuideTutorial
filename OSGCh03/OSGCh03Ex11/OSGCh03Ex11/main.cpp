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


//�Զ���Copy�࣬�������������Ϣ
class MyCopyOp : public osg::CopyOp
{
public:

	inline MyCopyOp(CopyFlags flags = SHALLOW_COPY) :
		osg::CopyOp(flags),
		_blank(2),
		_number(5)
	{
		//
	}

	//�����������ڿ��������Ϣ��ʽ,ʵ�������ǹ������
	inline void moveIn() const
	{
		_blank += _number;
	}

	inline void moveOut() const
	{
		_blank -= _number;
	}

	inline void writeIndent() const
	{
		for (int i = 0; i < _blank; ++i)
		{
			std::cout << " ";
		}
	}

	//���ü�����copy
	virtual osg::Referenced* operator() (const osg::Referenced* ref) const
	{
		writeIndent();
		std::cout << "copying Referenced " << ref << std::endl;
		moveIn();
		osg::Referenced* ret_ref = CopyOp::operator()(ref);
		moveOut();
		return ret_ref;
	}

	//����copy
	virtual osg::Object* operator() (const osg::Object* obj) const
	{
		writeIndent();
		std::cout << "copying Object " << obj;
		if (obj)
		{
			std::cout << " " << obj->className();
		}
		std::cout << std::endl;
		moveIn();
		osg::Object* ret_obj = CopyOp::operator()(obj);
		moveOut();
		return ret_obj;
	}

	//�ڵ�copy
	virtual osg::Node* operator() (const osg::Node* node) const
	{
		writeIndent();
		std::cout << "copying Node " << node;
		if (node)
		{
			std::cout << " " << node->className() << " '" << node->getName() << "'";
		}
		std::cout << std::endl;
		moveIn();
		osg::Node* ret_node = CopyOp::operator()(node);
		moveOut();
		return ret_node;
	}

	//Drawable copy
	virtual osg::Drawable* operator() (const osg::Drawable* drawable) const
	{
		writeIndent();
		std::cout << "copying Drawable " << drawable;
		if (drawable)
		{
			std::cout << " " << drawable->className();
		}
		std::cout << std::endl;
		moveIn();
		osg::Drawable* ret_drawable = CopyOp::operator()(drawable);
		moveOut();
		return ret_drawable;
	}

	//״̬��copy
	virtual osg::StateSet* operator() (const osg::StateSet* stateset) const
	{
		writeIndent();
		std::cout << "copying StateSet " << stateset;
		if (stateset)
		{
			std::cout << " " << stateset->className();
		}
		std::cout << std::endl;
		moveIn();
		osg::StateSet* ret_stateset = CopyOp::operator()(stateset);
		moveOut();
		return ret_stateset;
	}

	//״̬����copy
	virtual osg::StateAttribute* operator() (const osg::StateAttribute* attr) const
	{
		writeIndent();
		std::cout << "copying StateAttribute " << attr;
		if (attr)
		{
			std::cout << " " << attr->className();
		}
		std::cout << std::endl;
		moveIn();
		osg::StateAttribute* ret_attr = CopyOp::operator()(attr);
		moveOut();
		return ret_attr;
	}

	//������Ϣcopy
	virtual osg::Texture* operator() (const osg::Texture* text) const
	{
		writeIndent();
		std::cout << "copying Texture " << text;
		if (text)
		{
			std::cout << " " << text->className();
		}
		std::cout << std::endl;
		moveIn();
		osg::Texture* ret_text = CopyOp::operator()(text);
		moveOut();
		return ret_text;
	}

	//��ͼcopy
	virtual osg::Image* operator() (const osg::Image* image) const
	{
		writeIndent();
		std::cout << "copying Image " << image;
		if (image)
		{
			std::cout << " " << image->className();
		}
		std::cout << std::endl;
		moveIn();
		osg::Image* ret_image = CopyOp::operator()(image);
		moveOut();
		return ret_image;
	}

protected:

	//�˴����������ǿɱ��ͱ�������Ϊ��const������ʹ�ã���Ҫͻ��const������
	//�ո���������
	mutable int _number;
	//�ո�������
	mutable int _blank;
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Node> rootnode = osgDB::readNodeFile("glider.osg");

	//�Ż���������
	osgUtil::Optimizer optimzer;
	optimzer.optimize(rootnode.get());

	//ǳ����һ������
	osg::ref_ptr<osg::Node> shallowCopy = dynamic_cast<osg::Node*>(rootnode->clone(osg::CopyOp::SHALLOW_COPY));
	std::cout << std::endl << "���ǳ����һ������" << std::endl << std::endl;

	//���һ������
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(rootnode->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout << std::endl << "������һ������" << std::endl << std::endl;

	//�Զ��������Ϣǳ��������
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(rootnode->clone(MyCopyOp(osg::CopyOp::SHALLOW_COPY)));
	std::cout << std::endl << "���һ���Զ��������Ϣǳ��������" << std::endl << std::endl;

	//�Զ��������Ϣ�������
	osg::ref_ptr<osg::Node> myDeepCopy = dynamic_cast<osg::Node*>(rootnode->clone(MyCopyOp(osg::CopyOp::DEEP_COPY_ALL)));
	std::cout << std::endl << "���һ���Զ��������Ϣ�������" << std::endl << std::endl;

	viewer->setSceneData(rootnode.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->realize();

	viewer->run();
	return 0;
}