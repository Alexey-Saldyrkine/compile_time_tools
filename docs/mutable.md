# consteval mutable

consteval_mutable is a non-constructible, compile time only type, that is interacted with using its static consteval member functions.

consteval_mutable is built on top of consteval_map.

consteval_mutable can store any reflectable value or type.

consteval_mutable is a type that can store a value, get the stored value, swap the stored value for a new one, while referring to the same type.

consteval_mutable has three template parameters, storageR (see consteval_map), search_hint and key.         
```cpp
template<meta::info storageR, std::size_t search_hint = 100, meta::info key = ^^void>
struct consteval_mutable;
```

storageR is the incomplete type that will be used to store information about the mutable

search_hint is used when searching for the latest value of the mutable

key is a reflection used to uniquely identify a consteval_mutable within the storage type.

Note: if two different consteval_mutable types have the same storage type and key value, then
The two different consteval_mutables act as if they are same type.

## how it works

The main issue with having mutable types in compile time is the fact that we can only add new class template specialization, never redefine or delete them. This means that when we want to store a new value to the mutable, we create a new class specialization and, when getting it, we find the latest specialization and return it. Consteval_mutable must work in a stateless manner.

To do this we index all the stored values staring from 0 and incrementing by one. We can't store the latest index, but we can search for it.

The search is done using the function get_last_index of consteval_mutable:
```cpp
static consteval std::size_t get_last_index(); 
```
This function will return the index of the current stored element. When called on a mutable that has no elements stored, the value '(std::size_t) -1' will be returned, meaning there is nothing stored in the mutable.

The function first searches for the value N, where the index (search_hint)^(N) is the smallest unused index and N>0.

Here search_hint determines how fast the search index grows.

Ideally search_hint is 1 more than the maximum amount of elements that will be stored in the mutable.
The default value for search_hint is 100, this number was chosen a bit arbitrarily, as in most cases you are unlikely to store more than 100 or 10000 elements.

Once the value N has been found, the function performs a binary search on the range [0;search_hint^N], searching for the smallest unused index I.

Once I is found, the function returns the value I-1.

This works in the same way as consteval_counter.

### storing values
When storing a value in a mutable, we are actually storing it in a consteval_map.

The key will the ^^(std::pair<meta::info, std::size_t>{key,Index}).

Meaning the key is the reflection of a pair object, where the first value is the template argument key of consteval_mutable, and the second is the index of the element.

Index will be get_last_index()+1.


# functions
There are three main types of functions: check, out, get. Same as consteval_map, the only difference begin that they have only one argument, the value, the key is made automatically.

## check functions

### check
```cpp
static consteval bool check();
```
The function 'check' simply returns 'get_last_index()!=static_cast<std::size_t>(-1);'. Meaning that if the condition is true, then the consteval_mutable is not empty and has a value to get, and if the condition is false, then the consteval_mutable is empty and has no value to get.

### check_is_value
```cpp
template<std::size_t index = get_last_index()>
static consteval bool check_is_value();
```
The function 'check_is_value' checks whether the value at index 'index' is a value and not a type. By default 'index' is the last element put into the mutable. If the mutable is empty then the function returns false.

### check_is_type
```cpp
template<std::size_t index = get_last_index()>
static consteval bool check_is_type();
```
The function 'check_is_type' checks whether the value at index 'index' is a type and not a value. By default 'index' is the last element put into the mutable. If the mutable is empty then the function returns false.

## put functions

### put_refl
```cpp
template<meta::info v>
static consteval void put_refl();
```
The function 'put_refl' puts the given reflection 'v' at the index get_last_index()+1.

### put
```cpp
template<auto v>
static consteval void put();

template<typename v>
static consteval void put();
```
The function 'put' puts the given type or value 'v' at the index get_last_index()+1.

## get functions

### get_refl
```cpp
template<std::size_t index = get_last_index()>
static consteval meta::info get_refl();
```
The function 'get_refl' returns the stored reflection at the index 'index'. By default 'index' is the last element put into the mutable.

### get_v and get_t
```cpp
template<std::size_t index = get_last_index()>
static constexpr auto get_v;

template<std::size_t index = get_last_index()>
using get_t;
```
The function 'get_v' is the stored value at the index 'index'. By default 'index' is the last element put into the mutable. If the element at the index is not a value, then a compilation error occurs.

The function 'get_t' is the stored type at the index 'index'. By default 'index' is the last element put into the mutable. If the element at the index is not a type, then a compilation error occurs.

Example:
```cpp
template<auto>
struct storage; // the storage type

using mut = consteval_mutable<^^storage>; // the mutable type

static_assert(mut::check() == false); // the mutabel is empty

consteval{
    mut::put<54321>(); // put a value into the mutable
}

static_assert(mut::get_v<> == 54321); // get the current stored element as a value

consteval{
    mut::put<int>(); // put a type into the mutable
}

static_assert(std::is_same_v< mut::get_t<>, int>); // get the current stored element as a type

template<int i>
constexpr int templateExample = i;

consteval{
    mut::put_refl<^^templateExample>(); // put a reflextion of a tempalte, which is niether a value or type
}

static_assert( template[:mut::get_refl():]<5> == 5 ); // get the current stored reflection and splice it into back into a tempalte

template<int i>
constexpr int templateExample2 = i+1;

consteval{
    mut::put<^^templateExample2>(); // alternatively you can put the reflection of a template as a value
}

static_assert( template[:mut::get_v<>:]<5> == 6 ); // get the current stored element as a value, which is a reflection, and splice it into back into a tempalte

static_assert(mut::get_v<0> == 54321); // get first element stored
static_assert(std::is_same_v< mut::get_t<1>, int>); // get the second element stored
```
