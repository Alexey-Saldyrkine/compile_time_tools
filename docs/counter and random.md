# consteval counter
consteval_counter is a non-constructible, compile time only type, that is interacted with using its static consteval member functions.

consteval_counter is a compile time counter, which starts at 0 and increase in an ascending order. 

consteval_counter has two template parameters, storageR (see consteval_map) and hint.
```cpp
template<meta::info storageR, std::size_t hint = 100>
struct consteval_counter;
```
storageR is the incomplete type that will be used to store information about the counter.

hint is used when searching for the counter value.

Note: is two deferent consteval_counter types have the same storage type, then the two different consteval_counter types act as if they are the same type.

## how it works
consteval_counter works in a stateless manner, meaning it cannot store then counter value directly, instead the counter defines class specialization of the storage type to increment the counter, and when searching for the value we preform a binary search on the interval [0,hint^N], where N is the smallest integer which fulfills the conditions: storage with the index hint^N is not defined and N>0. Same as consteval_mutable.

# functions
## get
```cpp
static consteval std::size_t get();
```
The function 'get' returns the current value of the counter.

## increment
```cpp
static consteval void increment(std::size_t n = 1);
```
The function 'increment' increments the current value of the counter by n. By default n is 1.

Example:
```cpp
template<auto>
class storage;

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

# consteval_rng

consteval_rng is a wrapper over consteval_counter with a few additions.

consteval_rng has three template parameters storageR, keyV and a lambda type F.

```cpp
template<meta::info storageR, std::size_t keyV, proper_cbrng_lambda F = defualt_cbrng_lambda>
struct consteval_rng;
```

storageR is the storage type that the internal counter will use.

keyV is the key or seed that the rng function will use.

Lamda type F is the function that will generate the random values. It is constrained by the concept 'proper_cbrng_lambda', meaning 'proper counter based rng'.

```cpp
template<typename T>
concept is_not_void = !std::is_same_v<T,void>;

template<typename T>
concept proper_cbrng_lambda = requires(std::size_t cntr,std::size_t key){
    {T{}(cntr,key)} -> is_not_void;
};
```
The concept constraints F to be a type that is default constructible and has an operator() that accepts two std::size_t values, the counter and key value, and returns a non void value. This means that consteval_rng is not limited to only generating number, but any value.

## functions

### next
```cpp
static consteval void next();
```
The function 'next' simply increments the internal counter by one.

### get
```cpp
static consteval auto get();
```
The function 'get' calls F's operator() with the arguments of the internal counters value and the key.

### getInRange
```cpp
template<typename T>
static consteval auto getInRange(T min, T max);
```
This function can only be used if for the return type R of F::operator() and T the expression 'R%(T-T)+T'. If this expression is invalid then a compilation explaining this will occur.

This function will return the result of the expression 'get()%(max-min) + min'.

Example:
```cpp
template<auto>
struct storage;

// rng with key set to 1984
using rng = consteval_rng<^^storage,1984>;

static_assert(rng::getInRange(1,6) == 1);

consteval{
    rng::next();
}

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
```
