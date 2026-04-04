# 集成Catch2 v3（功能测试框架）
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.6.0
        SYSTEM
)
FetchContent_MakeAvailable(Catch2)

# 集成nanobench（性能测试框架）
FetchContent_Declare(
        nanobench
        GIT_REPOSITORY https://github.com/martinus/nanobench.git
        GIT_TAG        v4.3.11
        SYSTEM
)
FetchContent_MakeAvailable(nanobench)

# ======================
# 1. 单元测试可执行文件
# ======================
add_executable(m3_unit_tests
        test_vec.cpp
        test_mat.cpp
        test_qua.cpp
        test_utils.cpp
)
# 链接核心库与Catch2
target_link_libraries(m3_unit_tests PRIVATE m3 Catch2::Catch2WithMain)

# Debug模式开启ASAN内存检测（C++数学库必做）
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(m3_unit_tests PRIVATE -fsanitize=address -g -O1)
        target_link_options(m3_unit_tests PRIVATE -fsanitize=address)
    endif()
endif()

# 自动注册所有测试用例到CTest
include(Catch)
catch_discover_tests(m3_unit_tests)

# ======================
# 2. 性能测试可执行文件
# ======================
add_executable(m3_benchmark benchmark.cpp)
target_link_libraries(m3_benchmark PRIVATE m3 nanobench::nanobench)
# 性能测试强制开启O2优化，禁用调试信息
target_compile_options(m3_benchmark PRIVATE -O2 -DNDEBUG)

add_executable(svd_demo svd_demo/main.cpp)
target_link_libraries(svd_demo PRIVATE m3)
