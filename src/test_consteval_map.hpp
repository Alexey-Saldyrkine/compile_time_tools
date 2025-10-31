#include "consteval_map.hpp"

template <typename T>
struct tag {};

template <std::size_t N>
struct fixed_string {
    char data[N] = {};

    consteval fixed_string(const char (&str)[N]) {
        for (int i = 0; i < N; i++) {
            data[i] = str[i];
        }
    }

    constexpr char operator[](std::size_t i) const { return data[i]; }

    constexpr const char* to_cstr() const { return data; }
};

template <std::size_t N1, std::size_t N2>
constexpr bool operator==(fixed_string<N1> a, fixed_string<N2> b) {
    if constexpr (N1 == N2) {
        bool flag = true;
        for (int i = 0; i < N1; i++) {
            if (a[i] != b[i]) {
                flag = false;
                break;
            }
        }
        return flag;
    } else {
        return false;
    }
}

template <fixed_string strIn>
constexpr auto str = strIn;





namespace map_test1 {
template <meta::info k>
struct storage;
using map = consteval_map<^^storage>;
static_assert(!map::check(^^tag<int>));
consteval { map::put<^^tag<int>, ^^int>(); }
static_assert(map::check(^^tag<int>));
static_assert(map::get<^^tag<int>>() == ^^int);
consteval { map::put<^^tag<char>, meta::reflect_constant(22)>(); }
static_assert(map::get<^^tag<char>>() == meta::reflect_constant(22));
consteval { map::put<meta::reflect_constant(2.7), ^^double>(); }
static_assert(map::get<meta::reflect_constant(2.7)>() == ^^double);
}  // namespace map_test1





namespace map_test2 {
template <meta::info k>
struct storage;
using map = consteval_map<^^storage>;

static_assert(!map::checkT<int>());
constexpr bool a = map::checkT<int>();

consteval { map::putTT<int, char>(); }

constexpr bool b = map::checkT<int>();
static_assert(a == !b);
static_assert(std::is_same_v<map::getT_t<int>, char>);

consteval { map::putTT<tag<double>, tag<int>>(); }
consteval { map::putTT<tag<int>, char>(); }

static_assert(std::is_same_v<map::getT_t<tag<double>>, tag<int>>);
static_assert(std::is_same_v<map::getT_t<map::getT_t<tag<double>>>, char>);
}  // namespace map_test2





namespace map_test3 {
template <auto>
struct storage;
using map = consteval_map<^^storage>;

constexpr int i = 124;
consteval {
    map::putTV<int, 123>();
    map::putTV<char, i>();
    map::putVT<2.7, double>();
    map::putVT<str<"hello">, void>();
    map::putVV<1, str<"abc">>();
}

static_assert(map::getT_v<int> == 123);
static_assert(map::getT_v<char> == 124);
static_assert(std::is_same_v<map::getV_t<2.7>, double>);
static_assert(std::is_same_v<map::getV_t<str<"hello">>, void>);

consteval { map::putVV<2, str<"abc">>(); }
static_assert(map::getV_v<1> == str<"abc">);
static_assert(map::getV_v<1> == map::getV_v<2>);

}  // namespace map_test3




namespace map_test4 {
template <auto k>
struct funcStorage;
using funcMap = consteval_map<^^funcStorage>;

using func1 = decltype([]() { return 1; });
using func2 = decltype([]() { return 2; });
constexpr int func3() { return 3; }
consteval int func4() { return 4; }

consteval {
    funcMap::putVT<1, func1>();
    funcMap::putVV<2, func2{}>();
    funcMap::putVV<3, func3>();
    funcMap::putVV<4, ^^func4>();
}
using Tfunc1 = funcMap::getV_t<1>;
constexpr auto Tfunc2 = funcMap::getV_v<2>;
constexpr auto Tfunc3 = funcMap::getV_v<3>;
constexpr auto Tfunc4 = funcMap::getV_v<4>;
static_assert(Tfunc1{}() == 1);
static_assert(Tfunc2() == 2);
static_assert(Tfunc3() == 3);
static_assert([:Tfunc4:]() == 4);

}  // namespace map_test4
