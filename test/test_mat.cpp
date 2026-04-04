#include <catch2/catch_all.hpp>

import m3;
using namespace m3;

using namespace Catch::Matchers;
constexpr double EPS = 1e-6;
constexpr double PI = M_PI;

TEST_CASE("mat: 基础构造与访问", "[mat]") {
    SECTION("2x2矩阵构造") {
        mat2 m{1.0f, 2.0f, 3.0f, 4.0f};
        REQUIRE(m[0][0] == 1.0f);
        REQUIRE(m[1][0] == 2.0f);
        REQUIRE(m[0][1] == 3.0f);
        REQUIRE(m[1][1] == 4.0f);
    }

    SECTION("3x3零矩阵") {
        mat3 m = mat3::zero();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                REQUIRE(m[i][j] == 0.0f);
            }
        }
    }
}

TEST_CASE("mat: 算术运算符", "[mat]") {
    mat2 a{1.0f, 2.0f, 3.0f, 4.0f};
    mat2 b{5.0f, 6.0f, 7.0f, 8.0f};

    SECTION("标量乘法") {
        mat2 c = a * 2.0f;
        REQUIRE_THAT(c[0][0], WithinAbs(2.0f, EPS));
        REQUIRE_THAT(c[1][1], WithinAbs(8.0f, EPS));

        mat2 d = a;
        d *= 2.0f;
        REQUIRE(d == c);
    }

    SECTION("标量除法") {
        mat2 c = a / 2.0f;
        REQUIRE_THAT(c[0][0], WithinAbs(0.5f, EPS));
        REQUIRE_THAT(c[1][1], WithinAbs(2.0f, EPS));

        mat2 d = a;
        d /= 2.0f;
        REQUIRE(d == c);
    }

    SECTION("矩阵乘法") {
        mat2 c = a * b;
        REQUIRE_THAT(c[0][0], WithinAbs(19.0f, EPS));
        REQUIRE_THAT(c[1][0], WithinAbs(22.0f, EPS));
        REQUIRE_THAT(c[0][1], WithinAbs(43.0f, EPS));
        REQUIRE_THAT(c[1][1], WithinAbs(50.0f, EPS));
    }
}

TEST_CASE("mat: 核心矩阵运算", "[mat]") {
    SECTION("cofactor 余子式") {
        mat3 m{1,2,3,4,5,6,7,8,9};
        mat2 cof = cofactor(m, 0, 0);
        REQUIRE(cof[0][0] == 5.0f);
        REQUIRE(cof[1][0] == 6.0f);
        REQUIRE(cof[0][1] == 8.0f);
        REQUIRE(cof[1][1] == 9.0f);
    }

    SECTION("transpose 转置") {
        mat2 m{1,2,3,4};
        mat2 t = transpose(m);
        REQUIRE(t[0][0] == 1.0f);
        REQUIRE(t[1][0] == 3.0f);
        REQUIRE(t[0][1] == 2.0f);
        REQUIRE(t[1][1] == 4.0f);
    }

    SECTION("determinant 行列式") {
        mat2 m2{1,2,3,4};
        REQUIRE_THAT(determinant(m2), WithinAbs(-2.0f, EPS));

        mat3 m3 = mat3::zero();
        m3[0][0] = m3[1][1] = m3[2][2] = 1.0f;
        REQUIRE_THAT(determinant(m3), WithinAbs(1.0f, EPS));
    }

    SECTION("inverse 逆矩阵") {
        mat2 m{1,2,3,4};
        mat2 inv = inverse(m);
        mat2 res = m * inv;
        REQUIRE_THAT(res[0][0], WithinAbs(1.0f, EPS));
        REQUIRE_THAT(res[1][0], WithinAbs(0.0f, EPS));
        REQUIRE_THAT(res[0][1], WithinAbs(0.0f, EPS));
        REQUIRE_THAT(res[1][1], WithinAbs(1.0f, EPS));
    }

    SECTION("矩阵乘向量") {
        mat3 m = mat3::zero();
        m[0][0] = m[1][1] = m[2][2] = 2.0f;
        vec3 v{1,2,3};
        vec3 res = m * v;
        REQUIRE_THAT(res[0], WithinAbs(2.0f, EPS));
        REQUIRE_THAT(res[1], WithinAbs(4.0f, EPS));
        REQUIRE_THAT(res[2], WithinAbs(6.0f, EPS));
    }
}

TEST_CASE("mat: 变换矩阵", "[mat]") {
    SECTION("translate 平移矩阵") {
        vec2 offset{2,3};
        mat3 m = translate(offset);
        vec3 v{1,1,1};
        vec3 res = m * v;
        REQUIRE_THAT(res[0], WithinAbs(3.0f, EPS));
        REQUIRE_THAT(res[1], WithinAbs(4.0f, EPS));
    }

    SECTION("rotate 2D旋转矩阵") {
        mat3 m = rotate<float>(PI / 2.0f);
        vec3 v{1,0,1};
        vec3 res = m * v;
        REQUIRE_THAT(res[0], WithinAbs(0.0f, EPS));
        REQUIRE_THAT(res[1], WithinAbs(1.0f, EPS));
    }
}

// ======================
// SVD相关函数测试
// ======================

TEST_CASE("mat: SVD奇异值分解 - 基础正确性", "[mat][svd]") {
    SECTION("2x2对角矩阵SVD") {
        mat2 m = mat2::zero();
        m[0][0] = 3.0f;
        m[1][1] = 2.0f;

        auto result = svd(m);
        auto& U = result.U;
        auto& S = result.S;
        auto& V = result.V;

        // 验证奇异值
        REQUIRE_THAT(S[0][0], WithinAbs(3.0f, EPS));
        REQUIRE_THAT(S[1][1], WithinAbs(2.0f, EPS));

        // 验证重构: A ≈ U * S * V^T
        mat2 reconstructed = U * S * V.T();
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                REQUIRE_THAT(reconstructed[i][j], WithinAbs(m[i][j], EPS));
            }
        }
    }

    SECTION("3x3随机矩阵SVD") {
        mat3 m = mat3::zero();
        // 构造一个简单的可预测矩阵
        m[0][0] = 1.0f; m[1][0] = 2.0f; m[0][2] = 3.0f;
        m[0][1] = 4.0f; m[1][1] = 5.0f; m[1][2] = 6.0f;
        m[2][0] = 7.0f; m[2][1] = 8.0f; m[2][2] = 9.0f;

        auto result = svd(m);
        auto& U = result.U;
        auto& S = result.S;
        auto& V = result.V;

        // 验证重构: A ≈ U * S * V^T
        mat3 reconstructed = U * S * V.T();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                REQUIRE_THAT(reconstructed[i][j], WithinAbs(m[i][j], EPS));
            }
        }
    }
}

TEST_CASE("mat: inverse_svd伪逆矩阵", "[mat][svd]") {
    SECTION("2x2可逆矩阵的伪逆应等于普通逆") {
        mat2 m{1.0f, 2.0f, 3.0f, 4.0f};

        mat2 inv_normal = inverse(m);
        mat2 inv_svd = inverse_svd(m);

        // 验证两种方法结果一致
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                REQUIRE_THAT(inv_svd[i][j], WithinAbs(inv_normal[i][j], 1e-4));
            }
        }

        // 验证 A * A⁻¹ ≈ I
        mat2 identity_test = m * inv_svd;
        REQUIRE_THAT(identity_test[0][0], WithinAbs(1.0f, 1e-4));
        REQUIRE_THAT(identity_test[1][0], WithinAbs(0.0f, 1e-4));
        REQUIRE_THAT(identity_test[0][1], WithinAbs(0.0f, 1e-4));
        REQUIRE_THAT(identity_test[1][1], WithinAbs(1.0f, 1e-4));
    }

    SECTION("3x3可逆矩阵的伪逆验证") {
        mat3 m = mat3::zero();
        m[0][0] = 2.0f; m[0][1] = 1.0f; m[0][2] = 1.0f;
        m[1][0] = 1.0f; m[1][1] = 2.0f; m[1][2] = 1.0f;
        m[2][0] = 1.0f; m[2][1] = 1.0f; m[2][2] = 2.0f;

        mat3 inv_normal = inverse(m);
        mat3 inv_svd = inverse_svd(m);

        // 验证 A * A⁻¹ ≈ I
        mat3 identity_test = m * inv_normal;
        mat3 identity_test_svd = m * inv_svd;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                float expected = (i == j) ? 1.0f : 0.0f;
                REQUIRE_THAT(identity_test[i][j], WithinAbs(expected, EPS));
                REQUIRE_THAT(identity_test_svd[i][j], WithinAbs(expected, EPS));
            }
        }

        // 验证两种方法结果一致
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                REQUIRE_THAT(inv_svd[i][j], WithinAbs(inv_normal[i][j], EPS));
            }
        }
    }
}

TEST_CASE("mat: SVD边界条件测试", "[mat][svd]") {
    SECTION("单位矩阵的SVD") {
        mat3 m = mat3::zero();
        m[0][0] = m[1][1] = m[2][2] = 1.0f;

        auto result = svd(m);

        // 单位矩阵的奇异值应该都是1
        REQUIRE_THAT(result.S[0][0], WithinAbs(1.0f, EPS));
        REQUIRE_THAT(result.S[1][1], WithinAbs(1.0f, EPS));
        REQUIRE_THAT(result.S[2][2], WithinAbs(1.0f, EPS));
    }

    SECTION("零矩阵的SVD") {
        mat2 m = mat2::zero();

        auto result = svd(m);

        // 零矩阵的奇异值应该都是0
        REQUIRE_THAT(result.S[0][0], WithinAbs(0.0f, EPS));
        REQUIRE_THAT(result.S[1][1], WithinAbs(0.0f, EPS));
    }
}