cmake_minimum_required(VERSION 3.10)  
  
# 设置项目名称和版本  
project(MyProject VERSION 1.0)  
  
set(CMAKE_CXX_VERSION 11)
# 添加编译规则和源文件  
add_executable(MyProject main.cpp)  
  
# 指定需要链接的库，如果你的程序不需要链接任何库，可以忽略这一步  
# target_link_libraries(MyProject <库名>)