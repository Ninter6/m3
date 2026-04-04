#include <catch2/catch_all.hpp>

import m3;
using namespace m3;

using namespace Catch::Matchers;
constexpr double EPS = 1e-6;
constexpr double PI = M_PI;

TEST_CASE("utils: dot 点积", "[utils]") {
    vec3 a{1,2,3};
    vec3 b{4,5,6};
    REQUIRE_THAT(dot(a, b), WithinAbs(32.0f, EPS));

    vec3 c{1,0,0};
    vec3 d{0,1,0};
    REQUIRE_THAT(dot(c, d), WithinAbs(0.0f, EPS));
}

TEST_CASE("utils: cross 叉积", "[utils]") {
    vec3 a{1,0,0};
    vec3 b{0,1,0};
    vec3 res = cross(a, b);
    REQUIRE_THAT(res.z, WithinAbs(1.0f, EPS));
    REQUIRE(cross(b, a) == -res);
}

TEST_CASE("utils: distance 距离", "[utils]") {
    vec3 a{0,0,0};
    vec3 b{3,4,0};
    REQUIRE_THAT(distance(a, b), WithinAbs(5.0f, EPS));
    REQUIRE_THAT(distance_squared(a, b), WithinAbs(25.0f, EPS));
}

TEST_CASE("utils: angle 夹角", "[utils]") {
    vec3 a{1,0,0};
    vec3 b{0,1,0};
    REQUIRE_THAT(angle(a, b), WithinAbs(PI/2.0f, EPS));
    REQUIRE_THAT(angle(a, a), WithinAbs(0.0f, EPS));
}

TEST_CASE("utils: clamp 钳制", "[utils]") {
    vec3 v{-1,5,3};
    vec3 res = clamp(v, 0.0f, 4.0f);
    REQUIRE_THAT(res[0], WithinAbs(0.0f, EPS));
    REQUIRE_THAT(res[1], WithinAbs(4.0f, EPS));
    REQUIRE_THAT(res[2], WithinAbs(3.0f, EPS));
}

TEST_CASE("utils: outer_product 外积", "[utils]") {
    vec2 a{1,2};
    vec2 b{3,4};
    mat2 res = outer_product(a, b);
    REQUIRE_THAT(res[0][0], WithinAbs(3.0f, EPS));
    REQUIRE_THAT(res[0][1], WithinAbs(6.0f, EPS));
    REQUIRE_THAT(res[1][0], WithinAbs(4.0f, EPS));
    REQUIRE_THAT(res[1][1], WithinAbs(8.0f, EPS));
}