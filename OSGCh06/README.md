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