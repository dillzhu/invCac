cmake_minimum_required(VERSION 3.10)
project(invCac)

# 设置C++标准为17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # 关闭非标准的C++拓展

# 下载和添加Eigen库
include(FetchContent)
FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0  # 选择您想要的Eigen版本
)
FetchContent_MakeAvailable(Eigen)

# 添加可执行文件
add_executable(invCac main.cpp)

# 链接Eigen库
target_link_libraries(invCac Eigen3::Eigen)