# Compile Time Tools
A set of compile time tools.

Includes a key-value map, mutable compile time variable. 


###Map example:
```cpp
#include "consteval_map.hpp"

template <auto>
struct storage; //the incomplete type that is used as the storage medium for the map

using map = consteval_map<^^storage>; // the map type

constexpr int i = 124;

consteval {
	// add key-value pairs
	map::putTV<int, 123>();
	map::putTV<char, i>();
	map::putVT<2.7, double>();
	map::putVT<str<"hello">, void>();
	map::putVV<1, str<"abc">>();
}
// get values by keys
static_assert(map::getT_v<int> == 123);
static_assert(map::getT_v<char> == 124);
static_assert(std::is_same_v<map::getV_t<2.7>, double>);
static_assert(std::is_same_v<map::getV_t<str<"hello">>, void>);

consteval { map::putVV<2, str<"abc">>(); }
static_assert(map::getV_v<1> == str<"abc">);
static_assert(map::getV_v<1> == map::getV_v<2>);
```

###Consteval mutable example:
'''cpp
template<auto>
struct storage; //the incomplete type that is used as the storage medium for the mutable
using mut = consteval_mutable<^^storage>; // the mutable type

// put a constant value
consteval{
	mut::putV<124>();
}
static_assert(mut::get_v<> == 124);

// put a constant variable
const int i = 125;
consteval{
	mut::putV<i>();
}
static_assert(mut::get_v<> == i);
static_assert(mut::get_v<> == 125);

// put a type
consteval{
	mut::putT<double>();
}
static_assert(std::is_same_v<mut::get_t<>,double>);

// put a lambda type
using func1 = decltype([](){return 1});
consteval{
	mut::putT<func1>();
}
static_assert(mut::get_t<>{}() == 1);
'''
