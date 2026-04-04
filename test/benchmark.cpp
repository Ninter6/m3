#include <nanobench.h>

import m3;
using namespace m3;

int main() {
    ankerl::nanobench::Bench bench;
    bench.title("m3数学库性能基准")
         .warmup(1000)
         .epochs(10)
         .batch(10000);

    // 向量性能测试
    bench.run("vec3: 加法运算", [&]() {
        vec3 a{1.0f, 2.0f, 3.0f};
        vec3 b{4.0f, 5.0f, 6.0f};
        vec3 c = a + b;
        ankerl::nanobench::doNotOptimizeAway(c);
    });

    bench.run("vec3: 点积运算", [&]() {
        vec3 a{1.0f, 2.0f, 3.0f};
        vec3 b{4.0f, 5.0f, 6.0f};
        float res = dot(a, b);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("vec3: 叉积运算", [&]() {
        vec3 a{1.0f, 2.0f, 3.0f};
        vec3 b{4.0f, 5.0f, 6.0f};
        vec3 res = cross(a, b);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("vec3: 归一化运算", [&]() {
        vec3 a{1.234f, 5.678f, 9.012f};
        vec3 res = a.normalized();
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    // 矩阵性能测试
    bench.run("mat4: 矩阵乘法", [&]() {
        mat4 a = mat4::zero();
        mat4 b = mat4::zero();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                a[i][j] = 1.23f * i + 4.56f * j;
                b[i][j] = 7.89f * i + 0.12f * j;
            }
        }
        mat4 res = a * b;
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("mat3: 行列式计算", [&]() {
        mat3 m{1.23f,4.56f,7.89f,0.12f,3.45f,6.78f,9.01f,2.34f,5.67f};
        float res = determinant(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    // 四元数性能测试
    bench.run("quat: 四元数乘法", [&]() {
        quat q1{0.707f, 0.707f, 0.0f, 0.0f};
        quat q2{0.707f, 0.0f, 0.707f, 0.0f};
        quat res = q1 * q2;
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("quat: 向量旋转", [&]() {
        quat q{0.707f, 0.707f, 0.0f, 0.0f};
        vec3 v{1.0f, 2.0f, 3.0f};
        vec3 res = q * v;
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    // ======================
    // inverse与inverse_svd性能对比
    // ======================

    // 2x2矩阵性能对比
    bench.run("mat2: 普通逆矩阵 (inverse)", [&]() {
        mat2 m{1.23f, 4.56f, 7.89f, 0.12f};
        mat2 res = inverse(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("mat2: SVD伪逆矩阵 (inverse_svd)", [&]() {
        mat2 m{1.23f, 4.56f, 7.89f, 0.12f};
        mat2 res = inverse_svd(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    // 3x3矩阵性能对比
    bench.run("mat3: 普通逆矩阵 (inverse)", [&]() {
        mat3 m{1.23f, 4.56f, 7.89f,
               0.12f, 3.45f, 6.78f,
               9.01f, 2.34f, 5.67f};
        mat3 res = inverse(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("mat3: SVD伪逆矩阵 (inverse_svd)", [&]() {
        mat3 m{1.23f, 4.56f, 7.89f,
               0.12f, 3.45f, 6.78f,
               9.01f, 2.34f, 5.67f};
        mat3 res = inverse_svd(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    // 4x4矩阵性能对比（图形学最常用）
    bench.run("mat4: 普通逆矩阵 (inverse)", [&]() {
        mat4 m = mat4::zero();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = 1.23f * i + 4.56f * j + 0.78f;
            }
        }
        mat4 res = inverse(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("mat4: SVD伪逆矩阵 (inverse_svd)", [&]() {
        mat4 m = mat4::zero();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = 1.23f * i + 4.56f * j + 0.78f;
            }
        }
        mat4 res = inverse_svd(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    // 病态矩阵性能对比（数值稳定性测试）
    bench.run("mat3: 普通逆矩阵 (病态矩阵)", [&]() {
        mat3 m = mat3::zero();
        m[0][0] = 1.0f; m[0][1] = 1.0f; m[0][2] = 1.0f;
        m[1][0] = 1.0f; m[1][1] = 1.0001f; m[1][2] = 1.0002f;
        m[2][0] = 1.0f; m[2][1] = 1.0002f; m[2][2] = 1.0004f;
        mat3 res = inverse(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    bench.run("mat3: SVD伪逆矩阵 (病态矩阵)", [&]() {
        mat3 m = mat3::zero();
        m[0][0] = 1.0f; m[0][1] = 1.0f; m[0][2] = 1.0f;
        m[1][0] = 1.0f; m[1][1] = 1.0001f; m[1][2] = 1.0002f;
        m[2][0] = 1.0f; m[2][1] = 1.0002f; m[2][2] = 1.0004f;
        mat3 res = inverse_svd(m);
        ankerl::nanobench::doNotOptimizeAway(res);
    });

    return 0;
}