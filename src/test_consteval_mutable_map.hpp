#include "consteval_mutable_map.hpp"

namespace mutable_map_test1{
template<auto>
struct storage; // storage type

using mut_map = consteval_mutable_map<^^storage>; // the mutable map

// insert values
consteval{
    mut_map::put<1,2>();
    mut_map::put<35,7>();
    mut_map::put<66,int>();
    mut_map::put<char,99>();
    mut_map::put<long,void>();
}

static_assert(mut_map::check_is_value<1>());
static_assert(mut_map::check_is_type<66>());

static_assert(mut_map::getV_v<1> == 2);
static_assert(mut_map::getV_v<35> == 7);
static_assert(std::is_same_v<mut_map::getV_t<66>,int>);
static_assert(mut_map::getT_v<char> == 99);
    
// override the values with new ones
consteval{
    mut_map::put<1,246>();
    mut_map::put<35,void>();
    mut_map::put<66,321>();
    mut_map::put<char,int>();
    mut_map::put<long,4.321>();
}

static_assert(mut_map::getV_v<1> == 246);
static_assert(std::is_same_v<mut_map::getV_t<35>,void>);
static_assert(mut_map::getV_v<66> == 321);
static_assert(std::is_same_v<mut_map::getT_t<char>,int>);
static_assert(mut_map::getT_v<long> == 4.321);

// it is possible to get previously stored values
static_assert(mut_map::get_mutable_typeV_t<1>::getV<0> == 2);

}
