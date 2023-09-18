### 04.01 基本几何体的绘制
* 几何体设置顶点数组, 颜色数组, 法线数组, 绑定方式, 添加图元集合
    - osg::Geometry::addPrimitiveSet()
    - osg::DrawArray
* geode 添加可绘制对象(几何体)
    - addDrawable

### 04.02 索引绑定几何体
* osg::DrawElementsUInt --- 绘制元素索引列表
    - new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0)
    - osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0)
* 使用颜色索引数组 osg::TemplateIndexArray<
```
osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
//添加数据
vc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
vc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
vc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
vc->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>* colorIndex =
	new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>();
colorIndex->push_back(0);
colorIndex->push_back(1);
colorIndex->push_back(2);
colorIndex->push_back(3);
colorIndex->push_back(2);

//设置颜色数组
geom->setColorArray(vc.get());
//设置颜色索引数组
// geom->setColorIndices(colorIndex);
//设置颜色的绑定方式为单个顶点
geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
```

### 04.03 预定义几何体
* osg::TessellationHits --- 可以设置预定义可绘制对象几何体的精度
```
osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
hints->setDetailRatio(0.5f);
new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), radius), hints.get())
```
* osg::ShapeDrawable
    - osg::Sphere
    - osg::Box
    - osg::Cone
    - osg::Cylinder
    - osg::Capsule
* geode 可以通过 addDrawable 方法添加上面的就
* osg::PolygonMode 设置多边形渲染模式
```
osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode;
polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
root->getOrCreateStateSet()->setAttribute(polygonMode.get());
```

### 04.04 多边形分格化
* osgUtil::Tessellator 
    - setTessellationType
    - setBoundaryOnly
    - setWindingType
    - retessellatePolygons
```
//创建分格化对象
osg::ref_ptr<osgUtil::Tessellator> tscx=new osgUtil::Tessellator();
//设置分格类型为几何体
tscx->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
//设置只显示轮廓线为false，这里还需要填充
tscx->setBoundaryOnly(false);
//设置环绕规则
tscx->setWindingType( osgUtil::Tessellator::TESS_WINDING_ODD);

//使用分格化
tscx->retessellatePolygons(*(geom.get()));
```

### 04.05 简化几何体
* osgUtil::Simplifier: 继承自 osg::NodeVisitor 

### 04.06 Delaunay 三角网绘制
* osgUtil::DelaunayTriangulator 
    - bool triangulate() --- 生成三角网格
    - void addInputeConstraint() --- 添加限制条件
    - getTriangles() --- 获取三角形可绘制对象

### 04.07 三角带化
* osgUtil::TriStripVisitor 继承自 osgUtil::BasOptimizerVisitor
    - void stripify() --- 对几何体进行简化

### 04.08 生成顶点法线
* osgUtil::SmoothingVisitor 继承自 osg::NodeVisitor 类
    - smooth()