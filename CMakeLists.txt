cmake_minimum_required(VERSION 3.11)
project(yflow)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

#添加source子目录
add_subdirectory(source)
#指定生成目标
add_executable(yflow ${DIR_SRCS})
# 添加链接库
target_link_libraries(yflow source)

# debug 模式
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
# 生成可执行的文件
