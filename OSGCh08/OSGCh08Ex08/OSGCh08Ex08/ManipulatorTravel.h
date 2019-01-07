#pragma once
#include <windows.h>
#include <osgViewer/Viewer>

#include <osg/LineSegment>
#include <osg/Point>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

// #include <osgGA/MatrixManipulator>
#include <osgGA/OrbitManipulator>

#include <osgUtil/IntersectVisitor>

#include <vector>

class TravelManipulator :
	public osgGA::OrbitManipulator

{
public:

	//���캯��
	TravelManipulator();

	//��������
	~TravelManipulator(void);

	// �����μ��뵽����֮��
	static TravelManipulator* TravelToScene(osg::ref_ptr <osgViewer::Viewer> viewer);

private:
	osg::ref_ptr <osgViewer::Viewer>	m_pHostViewer;

	//�ƶ��ٶ�
	float m_fMoveSpeed;
	//
	osg::Vec3 m_vPosition;
	//
	osg::Vec3 m_vRotation;

public:

	//�������Ƿ���
	bool m_bLeftButtonDown;

	//���X,Y
	float m_fpushY;

	float m_fpushX;

	//���þ���
	virtual void setByMatrix(const osg::Matrixd& matrix);
	//���������
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	//�õ�����
	virtual osg::Matrixd getMatrix(void) const;
	//�õ������
	virtual osg::Matrixd getInverseMatrix(void)const;

	//�¼�������
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	// ��Ŀ�Ƕ�
	float m_fAngle;

	// λ�ñ任����
	void ChangePosition(osg::Vec3& delta);

	//��ײ����Ƿ���
	bool m_bPeng;

	//�����ٶ�
	float getSpeed();

	void  setSpeed(float &);

	//������ʼλ��
	void SetPosition(osg::Vec3 &position);

	osg::Vec3 GetPosition();

};