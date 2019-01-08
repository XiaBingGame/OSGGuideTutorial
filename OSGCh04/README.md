### 04.01 基本几何体的绘制
* 几何体设置顶点数组, 颜色数组, 法线数组, 绑定方式, 添加图元集合
    - osg::DrawArray
* geode 添加可绘制对象(几何体)

### 04.02 索引绑定几何体
* osg::DrawElementsUInt

### 04.03 预定义几何体
* osg::TessellationHits --- 可以设置预定义几何体的精度
* osg::ShapeDrawable
    - osg::Sphere
    - osg::Box
    - osg::Cone
    - osg::Cylinder
    - osg::Capsule

### 04.04 多边形分格化
* osgUtil::Tessellator 
    - setTessellationType
    - setBoundaryOnly
    - setWindingType
    - retessellatePolygons

### 04.05 简化几何体
* osgUtil::Simplifier: 继承自 osg::NodeVisitor 

### 04.06 Delaunay 三角网绘制
* osgUtil::DelaunayTriangulator 
    - bool triangulate() --- 生成三角网格
    - void addInputeConstraint() --- 添加限制条件
    - getTriangles()

### 04.07 三角带绘制
* osgUtil::TriStripVisitor 继承自 osgUtil::BasOptimizerVisitor
    - void stripify() --- 对几何体进行简化
    - void apply()

### 04.08 生成顶点法线
* osgUtil::SmoothingVisitor 继承自 osg::NodeVisitor 类
    - smooth()