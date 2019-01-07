/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include "PickHandler.h"

//�¼�������
bool CPickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
		//ÿһ֡
	case(osgGA::GUIEventAdapter::FRAME):
		{
			osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
			//�õ���ͼ����
			viewer ->getCamera() ->getViewMatrixAsLookAt(position, center, up) ;
			if (viewer)
			{	
				//ִ��PICK����
				pick(viewer.get(),ea);
			}
			return false;
		}    
	default:
		return false;
	}

}
//PICK����
void CPickHandler::pick(osg::ref_ptr<osgViewer::Viewer> viewer, const osgGA::GUIEventAdapter& ea)
{
	//����һ���߶ν���������
	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	//����һ����
	std::ostringstream os;
	//�õ�����λ��
	float x = ea.getX();
	float y = ea.getY();
	//���û�з����������㣬�����û�е�������
	if (viewer->computeIntersections(x,y,intersections))
	{
		//�õ��ཻ�����Ľ���
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			//������
			os<<"Mouse in World  X:"<< hitr->getWorldIntersectPoint().x()<<"     Y: "<<
				 hitr->getWorldIntersectPoint().y()<<"     Z: "<< hitr->getWorldIntersectPoint().z()<<std::endl ;
		}
	}
	//������
	os<<"Viewer Position X: "<<position[0]<<"     Y: "<<position[1]<<"     Z: "<<position[2]<<std::endl ;

	gdlist += os.str();
	
	//������ʾ����
	setLabel(gdlist);
}
