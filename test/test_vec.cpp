#include <catch2/catch_all.hpp>

import m3;
using namespace m3;

using namespace Catch::Matchers;
constexpr double EPS = 1e-6;

// 编译期constexpr验证（确保数学库支持编译期计算）
static_assert(vec<int, 3>{1,2,3}.size() == 3, "vec size constexpr failed");
static_assert(vec<float, 2>{1.0f, 2.0f}[0] == 1.0f, "vec operator[] constexpr failed");
static_assert(vec<int, 4>{1,2,3,4}.sum() == 10, "vec sum constexpr failed");
static_assert(vec<float, 2>{3.0f, 4.0f}.length_squared() == 25.0f, "vec length_squared constexpr failed");

TEST_CASE("vec: 基础构造与访问", "[vec]") {
    // 默认构造
    vec3 v0;
    REQUIRE(v0.size() == 3);

    // 列表初始化
    vec2 v1{1.0f, 2.0f};
    REQUIRE(v1[0] == 1.0f);
    REQUIRE(v1[1] == 2.0f);

    // 单值填充构造
    vec4 v2{5.0f};
    for (int i = 0; i < 4; ++i) {
        REQUIRE(v2[i] == 5.0f);
    }

    // 迭代器与指针访问
    ivec3 v3{1,2,3};
    int sum = 0;
    for (auto x : v3) sum += x;
    REQUIRE(sum == 6);

    dvec3 v4{1.0, 2.0, 3.0};
    const double* ptr = v4.value_ptr();
    REQUIRE(ptr[0] == 1.0);
    REQUIRE(ptr[2] == 3.0);
}

TEST_CASE("vec: 算术运算符重载", "[vec]") {
    vec3 a{1.0f, 2.0f, 3.0f};
    vec3 b{4.0f, 5.0f, 6.0f};

    SECTION("加法运算") {
        vec3 c = a + b;
        REQUIRE_THAT(c[0], WithinAbs(5.0f, EPS));
        REQUIRE_THAT(c[1], WithinAbs(7.0f, EPS));
        REQUIRE_THAT(c[2], WithinAbs(9.0f, EPS));

        vec3 d = a;
        d += b;
        REQUIRE(d == c);
    }

    SECTION("减法运算") {
        vec3 c = b - a;
        REQUIRE_THAT(c[0], WithinAbs(3.0f, EPS));
        REQUIRE_THAT(c[1], WithinAbs(3.0f, EPS));
        REQUIRE_THAT(c[2], WithinAbs(3.0f, EPS));

        vec3 d = b;
        d -= a;
        REQUIRE(d == c);
    }

    SECTION("标量乘法") {
        vec3 c = a * 2.0f;
        REQUIRE_THAT(c[0], WithinAbs(2.0f, EPS));
        REQUIRE_THAT(c[1], WithinAbs(4.0f, EPS));
        REQUIRE_THAT(c[2], WithinAbs(6.0f, EPS));

        vec3 d = a;
        d *= 2.0f;
        REQUIRE(d == c);

        vec3 e = 3.0f * a;
        REQUIRE_THAT(e[0], WithinAbs(3.0f, EPS));
    }

    SECTION("标量除法") {
        vec3 c = a / 2.0f;
        REQUIRE_THAT(c[0], WithinAbs(0.5f, EPS));
        REQUIRE_THAT(c[1], WithinAbs(1.0f, EPS));
        REQUIRE_THAT(c[2], WithinAbs(1.5f, EPS));

        vec3 d = a;
        d /= 2.0f;
        REQUIRE(d == c);
    }
}

TEST_CASE("vec: 核心数学函数", "[vec]") {
    SECTION("sum 求和") {
        vec4 v{1.0f, 2.0f, 3.0f, 4.0f};
        REQUIRE_THAT(v.sum(), WithinAbs(10.0f, EPS));
    }

    SECTION("length_squared 长度平方") {
        vec3 v{3.0f, 4.0f, 0.0f};
        REQUIRE_THAT(v.length_squared(), WithinAbs(25.0f, EPS));
    }

    SECTION("length 长度") {
        vec3 v{3.0f, 4.0f, 0.0f};
        REQUIRE_THAT(v.length(), WithinAbs(5.0f, EPS));
    }

    SECTION("normalize 归一化") {
        vec3 v{3.0f, 4.0f, 0.0f};
        vec3 n = v.normalized();
        REQUIRE_THAT(n.length(), WithinAbs(1.0f, EPS));
        REQUIRE_THAT(n[0], WithinAbs(0.6f, EPS));
        REQUIRE_THAT(n[1], WithinAbs(0.8f, EPS));

        vec3 m = v;
        m.normalize();
        REQUIRE(m == n);
    }

    SECTION("renorm 重归一化") {
        vec3 v{3.0f, 4.0f, 0.0f};
        vec3 r = renorm(v, 10.0f);
        REQUIRE_THAT(r.length(), WithinAbs(10.0f, EPS));
        REQUIRE_THAT(r[0], WithinAbs(6.0f, EPS));
        REQUIRE_THAT(r[1], WithinAbs(8.0f, EPS));
    }
}

TEST_CASE("vec: 边界与特化测试", "[vec]") {
    SECTION("零向量处理") {
        vec3 zero{0.0f, 0.0f, 0.0f};
        REQUIRE(zero.sum() == 0.0f);
        REQUIRE(zero.length_squared() == 0.0f);
    }

    SECTION("整数向量运算") {
        ivec2 a{1, 2};
        ivec2 b{3, 4};
        REQUIRE((a + b) == ivec2{4, 6});
        REQUIRE((a * 2) == ivec2{2, 4});
    }

    SECTION("无符号向量边界") {
        uvec3 v{1, 2, 3};
        REQUIRE(v[0] == 1u);
        REQUIRE(v.size() == 3);
    }
}
