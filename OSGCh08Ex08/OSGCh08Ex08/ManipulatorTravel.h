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

	//构造函数
	TravelManipulator();

	//析构函数
	~TravelManipulator(void);

	// 把漫游加入到场景之中
	static TravelManipulator* TravelToScene(osg::ref_ptr <osgViewer::Viewer> viewer);

private:
	osg::ref_ptr <osgViewer::Viewer>	m_pHostViewer;

	//移动速度
	float m_fMoveSpeed;
	//
	osg::Vec3 m_vPosition;
	//
	osg::Vec3 m_vRotation;

public:

	//鼠标左键是否按下
	bool m_bLeftButtonDown;

	//鼠标X,Y
	float m_fpushY;

	float m_fpushX;

	//设置矩阵
	virtual void setByMatrix(const osg::Matrixd& matrix);
	//设置逆矩阵
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	//得到矩阵
	virtual osg::Matrixd getMatrix(void) const;
	//得到逆矩阵
	virtual osg::Matrixd getInverseMatrix(void)const;

	//事件处理函数
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	// 屏目角度
	float m_fAngle;

	// 位置变换函数
	void ChangePosition(osg::Vec3& delta);

	//碰撞检测是否开启
	bool m_bPeng;

	//设置速度
	float getSpeed();

	void  setSpeed(float &);

	//设置起始位置
	void SetPosition(osg::Vec3 &position);

	osg::Vec3 GetPosition();

};