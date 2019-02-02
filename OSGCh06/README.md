### 06.01 文件读写示例
* osgDB 库
    - osgDB 库允许用户程序加载, 使用和写入 3D 数据库. osgDB 负责维护插件的注册信息表
* 文件的读取
    - osgDB::readNodeFile
    - osgDB::readImageFile
    - 相对路径的读取, 在数据路径列表中搜索文件, 如会在环境变量 OSG_FILE_PATH 定义的路径中查找文件
    - 手动添加数据文件路径列表
```
// 方法一
osgDB::FilePathList pathList = osgDB::getDataFilePathList();
pathList.push_back("...");
pathList.push_back("...");
osgDB::setDataFilePathList(pathList);
// 方法二
osgDB::Registry::instance()->getDataFilePathList().push_back(newpath);
```
* 文件的保存
    - osgDB::writeNodeFile

### 06.02 文件读写进度
* C++ 流缓冲的知识
    - C++ 的 iostream 提供了 3 种形式的缓冲
        - 不带缓冲区, 和 fwrite 操作一致
        - 空间自动管理的缓冲区, 下一个可写位置总是为末边界+1
        - 外置的缓冲区, 如定义局部字符串数组, 并将此区域传递给 iostream
    - basic_streambuf 及其派生类负责缓冲区, 如 basic_filebuf, basic_stringbuf, basic_istream 和 basic_ostream 都是 basic_streambuf 的友元类, basic_iostream 包含一个 basic_streambuf 的派生类的实例, 可以通过 basic_iostream 的成员函数 rdbuf() 获取, 类似"<<"的流操作符实际上是通过调用 basic_streambuf 的相关操作实现的.
    - 要控制标准输入/输出流的缓冲区行为, 必须生成自己的 basic_streambuf 派生类, 并生成该类的对象, 传递给标准输入/输出流. 由于基类 basic_streambuf 的功能较少, 我们从 basic_filebuf 或 basic_stringbuf 派生
    - osgDB 通过 osgDB::readNodeFile(), osgDB::writeNodeFile(), osgDB::readImageFile() 之类的函数来执行文件的读取和写入操作.
    - 编写自定义的数据流缓存类来跟踪 uflow() 方法, 从而获取当前读入的字节数
    - 步骤如下:
      - 给出文件名, 搜索用于读取该文件的插件, osgDB::Registry::instance()->getReaderWriterForExtension(), 传递文件扩展名参数, 返回 osgDB::ReaderWriter 对象
      - 创建自定义数据流缓存类的实例, 使用文件名构造对象, 使用类似 std::basic_filebuf<> 的方法打开文件
      - 通过自定义类的实例创建 std::istream 对象
      - 使用第一步的 ReaderWriter 对象的 readNode() 方法, 将第三步创建的 istream 对象作为其输入参数. 此时, OSG 将读入文件, 同时调用自定义类的 uflow() 方法, 该方法将负责显示读入字节的进度信息.
* 从 std::basic_filebuf 派生, 重写 uflow() 函数
* osgDB::ReaderWriter::readNode()

### 06.03 自定义文件插件
* 自定义一个插件读写类, 继承自 osgDB::ReaderWriter 类, 然后根据需要重写下面的一些函数
    - openArchive, readObject, readImage, readHeightField, readNode, writeObject, writeImage, writeHeightField, writeNode ...
    - 状态: ReadStatus, WriteStatus, ArchiveStatus
* 自定义文件插件的写法:
    - 需要建立一个 dll 项目, 输出的 dll 必须为 osgdb_扩展名.dll 或者 osgdb_扩展名d.dll
    - 插件注册, 定义全局变量
```
REGISTER_OSGPLUGIN(VR, ReaderWriterVR)
```
    - 应用程序使用中, 需要注册插件
```
osgDB::Registry::instance()->addFileExtensionAlias("VR","VR");
```

### 06.04 中文文件名和中文路径