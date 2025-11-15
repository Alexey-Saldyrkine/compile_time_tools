# Compile Time Tools
A set of compile time tools, uses c++26 reflection

[godbolt Link](https://godbolt.org/z/zz9348xT9)

Includes compile time variant of:
- mutable variable
- key-value map
- mutable key-value map 
- universal enum
- counter
- random number generator

See docs for details, primarily map

### Consteval mutable example:
```cpp
template<auto>
struct storage; //the incomplete type that is used as the storage medium for the mutable
using mut = consteval_mutable<^^storage>; // the mutable type

// put a constant value
consteval{
	mut::put<124>();
}
static_assert(mut::get_v<> == 124);

// put a constant variable
const int i = 125;
consteval{
	mut::put<i>();
}
static_assert(mut::get_v<> == i);
static_assert(mut::get_v<> == 125);

// put a type
consteval{
	mut::put<double>();
}
static_assert(std::is_same_v<mut::get_t<>,double>);

// put a lambda type
using func1 = decltype([](){return 1});
consteval{
	mut::put<func1>();
}
static_assert(mut::get_t<>{}() == 1);
```

### Map example:
```cpp
#include "consteval_map.hpp"

template <auto>
struct storage; //the incomplete type that is used as the storage medium for the map

using map = consteval_map<^^storage>; // the map type

constexpr int i = 124;

consteval {
	// add key-value pairs
	map::put<int, 123>();
	map::put<char, i>();
	map::put<2.7, double>();
	map::put<str<"hello">, void>();
	map::put<1, str<"abc">>();
}
// get values by keys
static_assert(map::getT_v<int> == 123);
static_assert(map::getT_v<char> == 124);
static_assert(std::is_same_v<map::getV_t<2.7>, double>);
static_assert(std::is_same_v<map::getV_t<str<"hello">>, void>);

consteval { map::put<2, str<"abc">>(); }
static_assert(map::getV_v<1> == str<"abc">);
static_assert(map::getV_v<1> == map::getV_v<2>);
```

### Mutable consteval map example:
```cpp
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
```

### Universal enum example:
```cpp
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
```

### Counter example:
```cpp
template<auto>
struct storage;

using counter = consteval_counter<^^storage>;

// all counters begin at 0
static_assert(counter::get()==0);

consteval{
    counter::increment();// increment counter by 1, default
}

static_assert(counter::get()==1);

consteval{
    counter::increment(1); // increment by 1
}

static_assert(counter::get()==2);

consteval{
    counter::increment(5); // increment by 5;
}

static_assert(counter::get()==7);
```
### Random value generator example:
```cpp
template<auto>
struct storage;

// rng with seed set to 1984
using rng = consteval_rng<^^storage,1984>;

static_assert(rng::getInRange(1,6) == 3);

consteval{
    rng::next();
}

static_assert(rng::getInRange(1,6) == 2);
consteval{
    rng::next();
}
static_assert(rng::getInRange(1,6) == 4);

consteval{
       rng::next();
}
static_assert(rng::getInRange(1,6) == 3);

consteval{
    rng::next();
}
static_assert(rng::getInRange(1,6) == 5);

```

