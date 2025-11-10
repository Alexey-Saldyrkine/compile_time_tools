# consteval map

Consteval map is compile time key-value map, that can be extended after its declaration.

Consteval map can use anything reflectable as its key or value, meaning types, templates and values of structural types and be used as keys and values.

Consteval map is a templated class that only has static consteval member functions, that allow to get, put and check key-value pairs.

Consteval map has a single non-type template parameter of type std::meta::info named storageR, meaning reflection of storage. 

storageR needs to be a reflection of an incomplete templated class with a single non-type template parameter of type std::meta::info or auto.

Example of storage classes:
```cpp
template<std::meta::info> // template parameter doesn't need an identifier
struct storageA; // class is incomplete 

template<auto>
class storageB; // either class or struct can be used

constexpr auto reflA = ^^storageA;
using mapA = consteval_map<reflA>; 

using mapB = consteval_map<^^storageB>;

```

### how it works

To store information on key-value pairs during compile time, consteval map uses meta::define_aggregate to create a complete template specialized type of storageR.

The template argument for the specialized class will be 'keyR', the reflection of the key.

The specialized will have on data member 'value' of type 'valueTag<valueR>', where 'valueR' is the reflection of the value, and valueTag is is a templated class defined as:

```cpp
template <auto v>
struct value_tag {
	static constexpr auto value = v;
};
```

So when we store a key-value pair in the map, we effectively create a specialization of the class 'storage' defined as:
```cpp
struct storage<^^key>{
	valueTag<^^value> value;
};
```

To extract a value using a key consteval map effectively does:
```cpp
meta::info valueRefl = decltype(storage<^^key>::value)::value
``` 
# functions
Consteval map has three main types of functions: out, get and check. They are all static consteval members of consteval_map.

## check
check functions are uses to check, whether a key exists and whether the value of a key is a type or a value.

### check_refl
```cpp
static consteval bool check_refl(meta::info key);
```
check_refl checks whether a there is a key-value pair for the key exists. It accepts a reflection of key. Returns true if there is a pair, and false if there isn't.

Example:
```cpp
tempalte<auto>
struct storage;

using map = consteval_map<^^storage>;

// check whether a key-value pair with the type 'int' as key exists
static_assert(map::check_refl(^^int) == false); // no such key exists

consteval{
	map::put<int,99>();// put a pair into the map;
}

//check again
static_assert(map::check_refl(^^int) == true); // a pair for int exists 

```

### check
```cpp
tempalte<typename key>
static consteval bool check();

template<auto key>
static consteval bool check();
```
Check checks whether a key-value pair for the given type or value as the key exists. Accepts a type or value as its template parameter. Returns true if there is a pair, and false if there isn't.

Example:
```cpp
tempalte<auto>
struct storage;

using map = consteval_map<^^storage>;

// check whether a key-value pair with the type 'int' as key exists
static_assert(map::check<int>() == false); // no such key exists
// check whether a key-value pair with the value '55' as key exists
static_assert(map::check<55>() == false); // no such key exists

consteval{
	map::put<int,99>();// put a pair into the map;
	map::put<55,100>();// put a pair into the map;
}

//check again
static_assert(map::check<int>() == true); // a pair for int exists
static_assert(map::check<55>() == true); // a pair for 55 exists 
```

### check_is_value
Same as check, but if check<>() passes, then it also checks whether the value of the pair is a value, and not a type.

### check_is_type
Same as check_is_value, but check if the value is a type and not a value.
Example:
```cpp
tempalte<auto>
struct storage;

using map = consteval_map<^^storage>;

consetval{
	map:put<1,0>();
	map::put<2,int>();
}

static_assert(map::check_if_value<1>() == true);
static_assert(map::check_if_type<1>() == false);

static_assert(map::check_if_value<2>() == false);
static_assert(map::check_if_type<2>() == true);

static_assert(map::check_if_value<3>() == false);
static_assert(map::check_if_type<3>() == false);
``` 

## put
Put functions insert key-value pairs into the map. 

### put_refl
```cpp
template<meta::info key, meta::info value>
static consteval void put_refl();
```
put_refl takes two meta::info template parameters key and value. It inserts the pair into the map.

### put
```cpp
template<typename k, typename v>
static consteval void put();

template<typename k, auto v>
static consteval void put();

template<auto k, typename v>
static consteval void put();

template<auto k, auto v>
static consteval void put();
```
put takes two of either type or value as its template parameter. It inserts the pair into the map.

Example:
```cpp
tempalte<auto>
struct storage;

using map = consteval_map<^^storage>;

consetval{
	map::put_refl<^^int,^^long>(); // put reflections 
	map:put<1,0>(); // put value and value
	map::put<2,int>(); // put value and type
	map::put<char,3>(); // put type and value
	map::put<double,float>(); // put type and type
}
```

## get
For getting the value of a key-value pair there is a function get_refl and tempalted aliases getT_v and getV_t, and static data members getV_v and getT_v.

If a key-value pair for a given key does not exist when get is used, then a compilation error occurs.

get_refl takes a reflection as its template parameter and returns a reflection.
```cpp
template<meta::info key>
static consteval meta::info get_refl();

template<auto key>
static cosnstexpr auto getV_v;

tempalte<auto key>
using getV_t;

template<typename key>
static constexpr auto getT_t;

template<typeanme key>
using getT_t;
````

Example:
```cpp
tempalte<auto>
struct storage;

using map = consteval_map<^^storage>;

consetval{
	map::put_refl<^^int,^^long>(); 
	map:put<1,0>(); 
	map::put<2,int>(); 
	map::put<char,3>(); 
	map::put<double,float>();
}

static_assert(map::get_refl<^^int>() == ^^long); // get reflection by reflection
static_assert(map::getV_v<1> == 0); // get value by value
static_assert(std::is_same_v<map::getV_t<2>, int>); // get type by value
static_assert(map::getT_v<char> == 3); // get value by type
static_assert(std::is_same_v<map::getT_t<double>, float>); // get type by type

```