# universal enum

The name 'universal_enum' may be misleading as it is not a runtime type, like an enum, but is a compile time construct.

Universal_enum is a consteval_map where all the keys are character strings giving them a name. It is universal in the way, that anything reflectable can be the stored value, including types and templates, which normal enums can't use. universal_enum can also be extended after its declaration, unlike enums.

Universal_enum can be used a dictionary for a code base.

## functions 

Universal enum has all the same functions as consteval_map (check, check_is_value, check_is_type, put, get_t, get_v) except get_refl and put_refl.

The only difference being that the key is always a character string.

### example:
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