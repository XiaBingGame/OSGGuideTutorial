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
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>
#include <fstream>

template<typename Elem, typename Tr = std::char_traits<Elem> >
class progress_streambuf : public std::basic_filebuf<Elem, Tr>
{
public:
	typedef std::basic_filebuf<Elem, Tr> base_type;

	explicit progress_streambuf(const std::string &filename) :
		base_type(),
		count_(0),
		prev_perc_(0)
	{
		if (open(filename.c_str(), std::ios_base::in | std::ios_base::binary))
		{
			size_ = static_cast<int>(pubseekoff(0, std::ios_base::end, std::ios_base::in));
			pubseekoff(0, std::ios_base::beg, std::ios_base::in);
		}
	}
protected:
	// ���ļ������ݶ��뻺��
	virtual int_type uflow()
	{
		// ���뻺��
		int_type v = base_type::uflow();

		// ��ǰ��ȡ�Ĵ�С
		// egptr() ��ǰ���������ָ��
		// gptr() �����嵱ǰָ��
		count_ += egptr() - gptr();
		int p = count_ * 100 / size_;
		if (p > prev_perc_)
		{
			std::cout << p << " ";
			prev_perc_ = p;
		}
		return v;
	}

private:
	int count_; // ����
	int size_; // �ļ���С
	int prev_perc_; // ��ǰ����
};

typedef progress_streambuf<char> progbuf;

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::string filename = "cow.osg";

	osg::ref_ptr<osgDB::ReaderWriter> rw =
		osgDB::Registry::instance()->getReaderWriterForExtension(osgDB::getLowerCaseFileExtension(filename));
	
	if (!rw.get())
	{
		std::cout << "Could not find a suitable read/write to load the specified file" << std::endl;
		return 1;
	}

	std::auto_ptr<progbuf> pb(new progbuf(osgDB::findDataFile(filename)));
	if (!pb->is_open())
	{
		std::cout << "could not open file " << filename << std::endl;
		return 1;
	}

	std::cout << "��ȡ����" << std::endl;

	std::istream mis(pb.get());
	osgDB::ReaderWriter::ReadResult rr = rw->readNode(mis);

	osg::ref_ptr<osg::Node> node = rr.getNode();

	if (!node)
	{
		std::cout << "Could not load file " << filename << std::endl;
	}

	root->addChild(node.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setUpViewInWindow(100, 100, 800, 600);
	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}