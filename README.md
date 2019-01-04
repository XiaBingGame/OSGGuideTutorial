# OpenSceneGraph 三维渲染编程指南学习代码
====================================================

03.01
公告板
    - 创建几何体, 设置纹理
    - 设置公告板
    - osg::StateSet::setMode(): 设置混合, 光照
    - osg::Billboard

03.02
位置变换节点
    - osg::PositionAttitudeTransform
        - setPosition
        - setScale
        - addChild

03.03
矩阵变换节点
    - osg::MatrixTransform: 继承自 osg::Transform
        - setMatrix
        - addChild
    - osg::Matrix
        - makeTranslate
        - makeRotate

03.04
自动对齐节点
    - osg::AutoTransform: 朝向屏幕, 朝向相机
        - setAutoRotateMode
        - setAutoScaleToScreen
        - setMinimumScale
        - setMaximumScale
        - setPosition
    - osgText::Text 为 osg::Drawable
        - setFont()
        - setFontResolution()
        - setCharacterSize()
        - setAlignment()
        - setAxisAlignment()

03.05
开关节点
    - osg::Switch: 继承自 osg::Group
        - addChild()

03.06
细节层次节点
    - osg::LOD: 继承自 osg::Group, 两种中心模式
        - addChild

03.07
分页细节层次节点
    - osg::PagedLOD: 继承自 osg::LOD, osg::LOD 节点存在于一个文件中, osg::PagedLOD 每个节点都是磁盘的文件, 节点只提供索引的作用
        - setCenter
        - setFileName
        - setRange

03.08
替代节点
    - osgSim::Impostor

03.11
浅拷贝

08.01
* ClipPlane 裁剪平面

08.02
* Scissor 裁剪

08.03
* 宽屏变形问题

08.04
* 单视图多相机

08.05
* 单窗口多视图渲染
    - 多视图就可以使用不同的相机不同的内容

08.06
* 多窗口多视图

08.07
* 实现抓图功能
    - GUIEventHandler 捕捉按键事件
    - osg::Camera::DrawCallback 派生类实现读取当前图像的功能

08.08
* 自定义场景漫游操作器
    - 实现简单的碰撞检测

08.09
* 动画路径操作器读取动画从而实现路径漫游

08.10
* 鼠标拾取并用 HUD 显示拾取信息

08.11
* 鼠标拾取并实现高亮效果

11.01
* 状态属性中的雾属性

11.02
* 预定义的雪粒子

11.03
* 预定义的雨粒子

11.04
* 预定义的爆炸粒子, 烟雾粒子, 火粒子, 爆炸碎片粒子

11.05
* 自定义粒子系统
    - 自定义粒子的一切内容

11.06
* 公告板和序列动画实现爆炸动画

11.07
* 粒子系统的读取和保存
    - 主要为用绝对矩阵来保存
    - 代码里的回调似乎有问题

11.08
* 阴影计数
    - 实现飞机飞行动画
    - 创建 grid 几何体实现高程数据
    - 阴影实现
