#include <catch2/catch_all.hpp>
#include <random>

import m3;
using namespace m3;

using namespace Catch::Matchers;
constexpr double EPS = 1e-6;
constexpr double PI = M_PI;

static_assert(quat::unit().size() == 4, "qua size constexpr failed");

TEST_CASE("qua: 基础构造与访问", "[qua]") {
    SECTION("默认构造（单位四元数）") {
        quat q;
        REQUIRE(q.w == 1.0f);
        REQUIRE(q.x == 0.0f);
        REQUIRE(q.y == 0.0f);
        REQUIRE(q.z == 0.0f);
    }

    SECTION("显式构造") {
        quat q{1.0f, 2.0f, 3.0f, 4.0f};
        REQUIRE(q.w == 1.0f);
        REQUIRE(q.x == 2.0f);
        REQUIRE(q.y == 3.0f);
        REQUIRE(q.z == 4.0f);
    }

    SECTION("unit() 单位四元数") {
        quat q = quat::unit();
        REQUIRE((q == quat{1,0,0,0}));
    }
}

TEST_CASE("qua: 算术运算符", "[qua]") {
    quat q1{1,2,3,4};
    quat q2{5,6,7,8};

    SECTION("加法/减法") {
        quat add = q1 + q2;
        REQUIRE_THAT(add.w, WithinAbs(6.0f, EPS));
        REQUIRE_THAT(add.x, WithinAbs(8.0f, EPS));

        quat sub = q2 - q1;
        REQUIRE_THAT(sub.w, WithinAbs(4.0f, EPS));
        REQUIRE_THAT(sub.x, WithinAbs(4.0f, EPS));
    }

    SECTION("标量乘除") {
        quat mul = q1 * 2.0f;
        REQUIRE_THAT(mul.w, WithinAbs(2.0f, EPS));
        REQUIRE_THAT(mul.x, WithinAbs(4.0f, EPS));

        quat div = q1 / 2.0f;
        REQUIRE_THAT(div.w, WithinAbs(0.5f, EPS));
        REQUIRE_THAT(div.x, WithinAbs(1.0f, EPS));
    }

    SECTION("四元数乘法") {
        quat a = quat::unit();
        quat b{0,1,0,0};
        quat res = a * b;
        REQUIRE(res == b);
    }
}

TEST_CASE("qua: 核心数学函数", "[qua]") {
    SECTION("模长与归一化") {
        quat q{1,1,1,1};
        REQUIRE_THAT(q.length_squared(), WithinAbs(4.0f, EPS));
        REQUIRE_THAT(q.length(), WithinAbs(2.0f, EPS));
        REQUIRE(!q.is_unit());

        quat n = q.normalized();
        REQUIRE_THAT(n.length(), WithinAbs(1.0f, EPS));
        REQUIRE(n.is_unit());
    }

    SECTION("共轭与逆") {
        quat q{1,2,3,4};
        quat conj = q.conjugate();
        REQUIRE(conj.w == 1.0f);
        REQUIRE(conj.x == -2.0f);

        quat unit = quat::unit();
        REQUIRE(unit.inverse() == unit);
    }
}

TEST_CASE("qua: 旋转功能验证", "[qua]") {
    SECTION("轴角构造与向量旋转") {
        vec3 axis{1,0,1};
        float angle = PI / 2.0f;
        quat q = quat::from_rotation(axis, angle);

        vec3 v{0,1,0};
        vec3 res = q * v;
        REQUIRE_THAT(res.x, WithinAbs(-sqrt(.5f), EPS));
        REQUIRE_THAT(res.y, WithinAbs(0.0f, EPS));
        REQUIRE_THAT(res.z, WithinAbs(sqrt(.5f), EPS));
    }

    std::mt19937 eng{114514};
    auto R = rotate(rand_unit_vec3(eng), eng() / 1919810.f);

    SECTION("矩阵转换") {
        auto q = quat_cast(R);
        vec3 v = rand_unit_vec3(eng);

        auto test_m_v = mat3(R)*v;
        auto test_q_v = q*v;
        for (std::size_t i = 0; i < 3; ++i) {
            // 验证旋转效果相同
            REQUIRE_THAT(test_m_v[i], WithinAbs(test_q_v[i], EPS));
        }

        auto test_r = rotate(q);
        // 验证矩阵闭环转换
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                REQUIRE_THAT(test_r[i][j], WithinAbs(R[i][j], EPS));
            }
        }
    }

    SECTION("欧拉角转换") {
        {
            auto ev = rand_unit_vec3(eng) * pi<>;
            euler e{ev.x, ev.y, ev.z};
            quat q = quat::from_euler(e, euler_seq::xyz);
            vec3 v = rand_unit_vec3(eng);
            auto rot_e =
                  rotate({1, 0, 0}, ev.x)
                * rotate({0, 1, 0}, ev.y)
                * rotate({0, 0, 1}, ev.z);
            vec3 test_q_v = q * v;
            vec3 test_e_v = vec3(rot_e * vec4(v, 1));
            REQUIRE_THAT(test_q_v.x, WithinAbs(test_e_v.x, EPS));
            REQUIRE_THAT(test_q_v.y, WithinAbs(test_e_v.y, EPS));
            REQUIRE_THAT(test_q_v.z, WithinAbs(test_e_v.z, EPS));
        }
        {
            auto q = quat_cast(R);
            auto eu = quat_to_euler(q, euler_seq::zxy);

            auto test_q = quat::from_euler(eu, euler_seq::zxy);
            for (int i = 0; i < 4; ++i) {
                // 验证欧拉角闭环转换
                REQUIRE_THAT(test_q[i], WithinAbs(q[i], EPS));
            }
        }
    }

    SECTION("旋转向量转换") {
        auto q = quat_cast(R);
        auto rv = q.to_rot_vec();

        auto test_q = quat::from_rot_vec(rv);
        for (int i = 0; i < 4; ++i) {
            REQUIRE_THAT(test_q[i], WithinAbs(q[i], EPS));
        }
    }
}
