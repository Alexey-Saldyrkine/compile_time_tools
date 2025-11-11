#pragma once
#include "consteval_counter.hpp"

consteval std::size_t processKey(std::size_t key){
    return key | ( 1<<31);
}
// any counter based rng can be used
// https://arxiv.org/pdf/2004.06278 is used as an example
consteval std::size_t square_CBRNG(std::size_t ctr, std::size_t key){
    key = processKey(key);
    std::size_t t,x,y,z;
    y = x = ctr*key;
    z = y+key;
    x = x*x + y; 
    x = (x>>32) | (x<<32);
    x = x*x+z;
    x = (x>>32) | (x<<32);
    x = x*x +y;
    x = (x>>32) | (x<<32);
    t = x = x*x +z;
    x = (x>>32) | (x<<32);
    return t ^ ((x*x +y) >> 32);
}

template<typename T>
concept is_not_void = !std::is_same_v<T,void>;


template<typename T, typename B>
concept is_InRange_compatible = requires(T ret, B v){
    {ret%(v-v)+v}->is_not_void;
};

template<typename T>
concept proper_cbrng_lambda = requires(std::size_t a,std::size_t b){
    {T{}(a,b)} -> is_not_void;
};

using defualt_cbrng_lambda = decltype([](std::size_t ctr, std::size_t key){return square_CBRNG(ctr,key);});

template<meta::info storageR, std::size_t keyV, proper_cbrng_lambda F = defualt_cbrng_lambda>
struct consteval_rng{
    private:
    using counter = consteval_counter<storageR>;
    consteval_rng()=delete;
    public:
    static constexpr std::size_t key = keyV;

    static consteval void next(){
        counter::increment();
    }

    static consteval auto get(){
        return F{}(counter::get(),key);
    }
    template<typename T>
    static consteval auto getInRange(T min, T max){
        using retT = decltype(F{}(0,0));
        if constexpr(is_InRange_compatible<retT,T>){
            std::size_t range = max-min;
            return get()%range + min;
        }else{
            static_assert(false,"getInRange cannot be used as the provided lambda return type R and the template type T cannot be used in the expresion: R%(T-T)+T");
            return;
        }
    }

};  