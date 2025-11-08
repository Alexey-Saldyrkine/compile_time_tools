#include "universal_enum.hpp"

namespace uni_enum_test1{
    template<auto>
    struct storage;

    using uni_enum = universal_enum<^^storage>;

    consteval{
        uni_enum::put<"alpha">();
        uni_enum::put<"a",123>();
        uni_enum::put<"a-T",int>();
    }
    static_assert(std::is_same_v<uni_enum::get_t<"a-T">,int>);
    static_assert(uni_enum::get_v<"a"> == 123);
    consteval{
        uni_enum::put<"beta">();
    }
    consteval{
        uni_enum::put<"gamma">();
    }

    static_assert(uni_enum::get_v<"alpha"> == 0);
    static_assert(uni_enum::get_v<"beta"> == 1);
    static_assert(uni_enum::get_v<"gamma"> == 2);
}

namespace uni_enum_test2{

template<auto>
struct storage; //the incomplete type that is used as the storage medium for the mutable

using uni_enum = universal_enum<^^storage>; // the universal enum

struct helper_type{
    int g;
    constexpr helper_type(int i):g(i){}

    constexpr int f() const{
        return g;
    }
};

constexpr auto object = helper_type(33);

using lambda_t = decltype([](){return 121;});

// put elements into the enum
consteval{
    uni_enum::put<"pie",3.14>();
    uni_enum::put<"constexpr_object",object>();
    uni_enum::put<"lambda_type",lambda_t>();
    uni_enum::put<"type",long long int>();
}

//check wheter an enum element exists
static_assert(uni_enum::check<"pie">());
static_assert(!uni_enum::check<"e">());

// get and check the enums
static_assert(uni_enum::get_v<"pie"> == 3.14);
static_assert(uni_enum::get_v<"constexpr_object">.f() == 33);
static_assert(uni_enum::get_t<"lambda_type">{}() == 121);
static_assert(std::is_same_v<long long int, uni_enum::get_t<"type">>);

// add additional elements to the enum
consteval{
    uni_enum::put<"value",'g'>();
}

// test whether the enum is a value or type
static_assert(uni_enum::check_is_value<"value">());
static_assert(!uni_enum::check_is_type<"value">());

static_assert(!uni_enum::check_is_value<"type">());
static_assert(uni_enum::check_is_type<"type">());


}


