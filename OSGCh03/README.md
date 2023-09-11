### 03.01 公告板
    - 创建几何体, 设置纹理
    - osg::Geometry
        - 设置顶点数组
        - 设置法线数组
        - 设置纹理坐标数组
        - addPrimitiveSet
    - 设置公告板
    - osg::StateSet::setMode(): 设置混合, 光照
        - GL_BLEND
        - GL_LIGHTING
    - osg::Billboard
        - setMode
        - setAxis
        - addDrawable

### 03.02 位置变换节点
    - osg::PositionAttitudeTransform
        - setPosition
        - setScale
        - addChild

### 03.03 矩阵变换节点
    - osg::MatrixTransform: 继承自 osg::Transform
        - setMatrix
        - addChild
    - osg::Matrix
        - makeTranslate
        - makeRotate

### 03.04 自动对齐节点
    - osg::AutoTransform: 朝向屏幕, 朝向相机
        - setAutoRotateMode
        - setAutoScaleToScreen
        - setMinimumScale
        - setMaximumScale
        - setPosition
    - osgText::Text 为 osg::Drawable, 可以通过 geometry 添加文本 drawable
        - setFont()
        - setFontResolution()
        - setCharacterSize()
        - setAlignment()
        - setAxisAlignment()

### 03.05 开关节点
    - osg::Switch: 继承自 osg::Group
        - addChild()

### 03.06 细节层次节点
    - osg::LOD: 继承自 osg::Group, 两种中心模式
        - addChild, 包含范围参数

### 03.07 分页细节层次节点
    - osg::PagedLOD: 继承自 osg::LOD, osg::LOD 节点存在于一个文件中, osg::PagedLOD 每个节点都是磁盘的文件, 节点只提供索引的作用
        - setCenter
        - setFileName
        - setRange

### 03.08 替代节点
    - osgSim::Impostor: 继承自 osg::LOD 节点, 替代节点也是一种布告板, 通过从当前视点将一个复杂物体对象绘制到一幅图像纹理上创建的, 其中渲染的图像纹理映射到布告板上. 替代节点可用于物体的一些实例或图形的一些画面, 加速了绘制和渲染. 替代节点的另一个优点是可以对纹理图像进行低通滤波(后期处理)
      - 图形纹理的分辨率绝对不能超过屏幕当前的分辨率
      - setImposterThreashold()
      - addChild()

### 03.09 遮挡裁剪节点
- osg::OccluderNode: 继承自 osg::Group. 该节点的主要作用是裁剪掉被遮挡的物体, 主要采用基于点的遮挡算法, 其需要指定一个遮挡面
    - setOccluder() 设置 osg::ConvexPlanarOccluder 对象, 该平面应该为一个凸多边形
    - addChild()
- osg::ConvexPlanarOccluder
    - getOccluder() 获取 osg::ConvexPlanarPolygon
- osg::ConvexPlanarPolygon 可以用来设置遮挡平面
    - void add(): 添加顶点
- osg::Node::getBound(): 得到包围球 osg::BoundingSphere, 从模型上得到包围球
- osg::BoundingBox::expandBy(osg::BoundingSphere): 由包围球扩展为包围盒
- osg::BoundingBox::corner() 可以得到一个角

### 03.10 坐标系节点
- osg::CoordinateSystemNode: 继承自 osg::Group 节点, 使一个场景的对象关联一个坐标系统. 通常的坐标系统类型有 WKT, PROJ4, USGS, 与 osg::EllipsoidModel 节点一起使用
    - setEllipsoidModel: 设置椭圆模型
    - addChild: 添加一个 geode, 里面一般为球形物体, 带全球图像纹理
```
	//创建坐标系节点
	osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
	//设置椭圆体模型，默认的坐标系WGS_84
	csn->setEllipsoidModel(new osg::EllipsoidModel());
	//添加子节点
```
- WKT 坐标系

### 03.11 自定义拷贝(1)
- 派生自 osg::CopyOp 类
    - 重写 operator() 方法
- 克隆调用各自节点的 clone() 方法, 参数使用 osg::CopyOp 或其派生类
```
	//浅拷贝一个场景
	osg::ref_ptr<osg::Node> shallowCopy = dynamic_cast<osg::Node*>(rootnode->clone(osg::CopyOp::SHALLOW_COPY));
	std::cout << std::endl << "完成浅拷贝一个场景" << std::endl << std::endl;

	//深拷贝一个场景
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(rootnode->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout << std::endl << "完成深拷贝一个场景" << std::endl << std::endl;

	//自定义输出信息浅拷贝场景
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(rootnode->clone(MyCopyOp(osg::CopyOp::SHALLOW_COPY)));
	std::cout << std::endl << "完成一个自定义输出信息浅拷贝场景" << std::endl << std::endl;
```

### 03.12 自定义拷贝(2)
- 重写 osg::CopyOp 的派生类, 解决一个节点多个父节点的深度拷贝问题, 减少内存消耗