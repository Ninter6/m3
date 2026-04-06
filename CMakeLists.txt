cmake_minimum_required(VERSION 3.29)
project(m3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译警告配置
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()

add_library(m3)
target_sources(m3 PUBLIC
    FILE_SET CXX_MODULES
    FILES m3.ccm
)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(test)
endif()