#include "ManipulatorTravel.h"

//构造函数
TravelManipulator::TravelManipulator() : m_fMoveSpeed(1.0f)
, m_bLeftButtonDown(false)
, m_fpushX(0)
, m_fAngle(2.5)
, m_bPeng(true)
, m_fpushY(0)
{
	m_vPosition = osg::Vec3(-22.0f, -274.0f, 100.0f);

	m_vRotation = osg::Vec3(osg::PI_2, 0.0f, 0.0f);

}
//析构函数
TravelManipulator::~TravelManipulator()
{
	//
}
// 把漫游加入到场景之中
TravelManipulator * TravelManipulator::TravelToScene(osg::ref_ptr <osgViewer::Viewer> viewer)
{
	TravelManipulator* camera = new TravelManipulator;

	viewer->setCameraManipulator(camera);

	camera->m_pHostViewer = viewer;

	return camera;
}

// 设置矩阵
void TravelManipulator::setByMatrix(const osg::Matrixd& matrix)
{
	//
}
//设置逆矩阵
void TravelManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{
	//
}
//得到矩阵
osg::Matrixd TravelManipulator::getMatrix(void) const
{
	osg::Matrixd mat;

	mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f),

		m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f),

		m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));

	return mat * osg::Matrixd::translate(m_vPosition);
}
//得到逆矩阵
osg::Matrixd TravelManipulator::getInverseMatrix(void) const
{
	osg::Matrixd mat;

	mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f),

		m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f),

		m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));

	return osg::Matrixd::inverse(mat * osg::Matrixd::translate(m_vPosition));
}
//事件处理函数
bool TravelManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	//得到鼠标的位置
	float mouseX = ea.getX();
	float mouseY = ea.getY();

	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN):
	{
		//空格键
		if (ea.getKey() == 0x20)
		{
			us.requestRedraw();

			us.requestContinuousUpdate(false);

			return true;
		}
		//上移动
		if (ea.getKey() == 0xFF50)
		{
			ChangePosition(osg::Vec3(0, 0, m_fMoveSpeed));

			return true;
		}
		//下移动 
		if (ea.getKey() == 0xFF57)
		{
			ChangePosition(osg::Vec3(0, 0, -m_fMoveSpeed));

			return true;
		}
		//增加速度
		if (ea.getKey() == 0x2B)
		{
			m_fMoveSpeed += 1.0f;

			return true;
		}
		//减少速度
		if (ea.getKey() == 0x2D)
		{
			m_fMoveSpeed -= 1.0f;

			if (m_fMoveSpeed < 1.0f)
			{
				m_fMoveSpeed = 1.0f;
			}
			return true;
		}
		//前进
		if (ea.getKey() == 0xFF52 || ea.getKey() == 0x57 || ea.getKey() == 0x77)//up
		{

			ChangePosition(osg::Vec3(0, m_fMoveSpeed * sinf(osg::PI_2 + m_vRotation._v[2]), 0));

			ChangePosition(osg::Vec3(m_fMoveSpeed * cosf(osg::PI_2 + m_vRotation._v[2]), 0, 0));

			return true;
		}
		//后退
		if (ea.getKey() == 0xFF54 || ea.getKey() == 0x53 || ea.getKey() == 0x73)//down
		{
			ChangePosition(osg::Vec3(0, -m_fMoveSpeed * sinf(osg::PI_2 + m_vRotation._v[2]), 0));

			ChangePosition(osg::Vec3(-m_fMoveSpeed * cosf(osg::PI_2 + m_vRotation._v[2]), 0, 0));

			return true;
		}
		//向左
		if (ea.getKey() == 0x41 || ea.getKey() == 0x61)
		{
			ChangePosition(osg::Vec3(0, m_fMoveSpeed * cosf(osg::PI_2 + m_vRotation._v[2]), 0));

			ChangePosition(osg::Vec3(-m_fMoveSpeed * sinf(osg::PI_2 + m_vRotation._v[2]), 0, 0));

			return true;
		}
		//向右
		if (ea.getKey() == 0x44 || ea.getKey() == 0x64)
		{
			ChangePosition(osg::Vec3(0, -m_fMoveSpeed * cosf(osg::PI_2 + m_vRotation._v[2]), 0));

			ChangePosition(osg::Vec3(m_fMoveSpeed * sinf(osg::PI_2 + m_vRotation._v[2]), 0, 0));

			return true;
		}
		//Right
		if (ea.getKey() == 0xFF53)
		{
			m_vRotation._v[2] -= osg::DegreesToRadians(m_fAngle);
		}
		//Left
		if (ea.getKey() == 0xFF51)
		{
			m_vRotation._v[2] += osg::DegreesToRadians(m_fAngle);
		}
		//改变屏角
		if (ea.getKey() == 0x46 || ea.getKey() == 0x66)//F
		{
			m_fAngle -= 0.2;

			return true;
		}

		if (ea.getKey() == 0x47 || ea.getKey() == 0x67)//G
		{
			m_fAngle += 0.2;

			return true;
		}

		return false;
	}
	//鼠标按下
	case (osgGA::GUIEventAdapter::PUSH):

		if (ea.getButton() == 1)
		{
			m_fpushX = mouseX;
			m_fpushY = mouseY;

			m_bLeftButtonDown = true;
		}

		return false;

		//拖动
	case (osgGA::GUIEventAdapter::DRAG):

		if (m_bLeftButtonDown)

		{
			m_vRotation._v[2] -= osg::DegreesToRadians(m_fAngle * (mouseX - m_fpushX));

			m_vRotation._v[0] += osg::DegreesToRadians(1.1*(mouseY - m_fpushY));

			if (m_vRotation._v[0] >= 3.14)
			{
				m_vRotation._v[0] = 3.14;
			}

			if (m_vRotation._v[0] <= 0)
			{
				m_vRotation._v[0] = 0;
			}
		}

		return false;
		//鼠标释放
	case (osgGA::GUIEventAdapter::RELEASE):

		if (ea.getButton() == 1)
		{
			m_bLeftButtonDown = false;
		}

		return false;

	default:
		return false;
	}
}

// 位置变换函数
void TravelManipulator::ChangePosition(osg::Vec3& delta)
{
	//碰撞检测
	if (m_bPeng)
	{
		//得到新的位置
		osg::Vec3 newPos1 = m_vPosition + delta;

		osgUtil::IntersectVisitor ivXY;
		//根据新的位置得到两条线段检测
		osg::ref_ptr<osg::LineSegment> lineXY = new osg::LineSegment(newPos1,
			m_vPosition);

		osg::ref_ptr<osg::LineSegment> lineZ = new osg::LineSegment(newPos1 + osg::Vec3(0.0f, 0.0f, 10.0f),
			newPos1 - osg::Vec3(0.0f, 0.0f, -10.0f));

		ivXY.addLineSegment(lineZ.get());

		ivXY.addLineSegment(lineXY.get());
		//结构交集检测
		m_pHostViewer->getSceneData()->accept(ivXY);
		//如果没有碰撞检测
		if (!ivXY.hits())
		{
			m_vPosition += delta;
		}
	}
	else
	{
		m_vPosition += delta;
	}

}

//设置速度
void TravelManipulator::setSpeed(float &sp)
{
	m_fMoveSpeed = sp;
}

//得到当前速度
float TravelManipulator::getSpeed()
{
	return m_fMoveSpeed;
}

//设置起始的位置
void TravelManipulator::SetPosition(osg::Vec3 &position)
{
	m_vPosition = position;
}

//得到当前的所在位置
osg::Vec3 TravelManipulator::GetPosition()
{
	return m_vPosition;
}
