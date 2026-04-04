#include <iostream>
#include <numeric>
#include <vector>
#include <thread>

import m3;

using namespace m3;

constexpr int WIDTH = 80, HEIGHT = 24;

constexpr vec2 g{0, 50};
constexpr float dt = .02f;
constexpr float w = .05f, v = .5f;

std::vector<vec2> rest_vertices;
std::vector<vec2> curr_vertices;
std::vector<vec2> last_vertices;

vec2 rest_center;
vec2 curr_center;

void calcu_center() {
    curr_center = std::accumulate(curr_vertices.begin(), curr_vertices.end(), vec2()) / (float)curr_vertices.size();
}

void solve_constraints() {
    calcu_center();

    auto H = mat2::zero();
    for (std::size_t i = 0; i < curr_vertices.size(); ++i) {
        auto qi = rest_vertices[i] - rest_center;
        auto pi = curr_vertices[i] - curr_center;
        H += outer_product(qi, pi);
    }

    auto [U, S, V] = svd(H);

    auto R = V * U.T();
    auto t = curr_center - R * rest_center;

    for (std::size_t i = 0; i < curr_vertices.size(); ++i) {
        auto target = R * rest_vertices[i] + t;
        auto correct = (target - curr_vertices[i]) * .5f;
        curr_vertices[i] += correct;
    }
}

void collision_response() {
    for (auto& i : curr_vertices)
        i = {std::min(i.x, WIDTH*.5f-.5f), std::min(i.y, HEIGHT-.9f)};
}

void move_vertices() {
    for (std::size_t i = 0; i < curr_vertices.size(); ++i) {
        auto t = curr_vertices[i];
        curr_vertices[i] += t - last_vertices[i] + g*dt*dt;
        last_vertices[i] = t;
    }
}

void print() {
    char img[HEIGHT*(WIDTH+1)]{};
    for (auto& i : curr_vertices)
        if (auto [x, y] = i.as_array; x > 0  && y > 0)
            img[int(y)*(WIDTH+1) + int(x*2)]++;

    constexpr auto tb = " .-:+=*#";
    for (int i=0; i < (WIDTH+1)*HEIGHT-1; i++)
        img[i] = (i+1)%(WIDTH+1) ? tb[int(sqrt(min((int)img[i], 9)/9.f)*7)] : '\n';

    std::cout << "\033[H" << img;
}

void init(int len) {
    curr_vertices.resize(len*len*8);
    rest_vertices.resize(len*len*8);
    last_vertices.resize(len*len*8);

    for (int i=0; i < len; i++)
        for (int j=0; j < len; j++)
            for (int k=0; k < 4; k++)
                for (int l=0; l < 2; l++)
                    curr_vertices[(i*len+j)*8+k*2+l] =
                    last_vertices[(i*len+j)*8+k*2+l] =
                    rest_vertices[(i*len+j)*8+k*2+l] = {1+i+.25f*k, 1+j+.5f*l};
    calcu_center();
    rest_center = curr_center;
}

int main(int argc, char** argv) {
    init(argc > 1 ? min(atoi(argv[1]), 20) : 8);

    for (auto& i : last_vertices) {
        auto r = i - curr_center;
        i.x -= r.y * w + v;
        i.y += r.x * w;
    }

    print();
    {std::string s; getline(std::cin, s);}

    while (true) {
        solve_constraints();
        move_vertices();
        collision_response();

        print();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
