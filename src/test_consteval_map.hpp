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
static_assert(!map::check_refl(^^tag<int>));
consteval { map::put_refl<^^tag<int>, ^^int>(); }
static_assert(map::check_refl(^^tag<int>));
static_assert(map::get_refl<^^tag<int>>() == ^^int);
consteval { map::put_refl<^^tag<char>, meta::reflect_constant(22)>(); }
static_assert(map::get_refl<^^tag<char>>() == meta::reflect_constant(22));
consteval { map::put_refl<meta::reflect_constant(2.7), ^^double>(); }
static_assert(map::get_refl<meta::reflect_constant(2.7)>() == ^^double);
}  // namespace map_test1





namespace map_test2 {
template <meta::info k>
struct storage;
using map = consteval_map<^^storage>;

static_assert(!map::check<int>());
constexpr bool a = map::check<int>();

consteval { map::put<int, char>(); }

static_assert(map::check_is_type<int>());
static_assert(!map::check_is_value<int>());

constexpr bool b = map::check<int>();
static_assert(a == !b);
static_assert(std::is_same_v<map::getT_t<int>, char>);

consteval { map::put<tag<double>, tag<int>>(); }
consteval { map::put<tag<int>, char>(); }

static_assert(std::is_same_v<map::getT_t<tag<double>>, tag<int>>);
static_assert(std::is_same_v<map::getT_t<map::getT_t<tag<double>>>, char>);
}  // namespace map_test2





namespace map_test3 {
template <auto>
struct storage;
using map = consteval_map<^^storage>;

constexpr int i = 124;
consteval {
    map::put<int, 123>();
    map::put<char, i>();
    map::put<2.7, double>();
    map::put<str<"hello">, void>();
    map::put<1, str<"abc">>(); 
    map::put<void,int>();
}

static_assert(map::check_is_value<int>());
static_assert(map::check_is_type<2.7>());
static_assert(map::check_is_value<1>());
static_assert(map::check_is_type<void>());

static_assert(map::getT_v<int> == 123);
static_assert(map::getT_v<char> == 124);
static_assert(std::is_same_v<map::getV_t<2.7>, double>);
static_assert(std::is_same_v<map::getV_t<str<"hello">>, void>);

consteval { 
    map::put<2, str<"abc">>();
    map::put<3, str<"acb">>();  
}
static_assert(map::getV_v<1> == str<"abc">);
static_assert(map::getV_v<1> == map::getV_v<2>);
static_assert(map::getV_v<1> != map::getV_v<3>);
static_assert(map::getV_v<2> != map::getV_v<3>);

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
    funcMap::put<1, func1>();
    funcMap::put<2, func2{}>();
    funcMap::put<3, func3>();
    funcMap::put<4, ^^func4>();
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
