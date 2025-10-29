#pragma once
#include "consteval_counter.hpp"


// any counter based rng can be used
// https://arxiv.org/pdf/2004.06278 is used as an example
consteval std::size_t square_CBRNG(std::size_t ctr, std::size_t key){
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
concept proper_cbrng_lambda = requires(std::size_t a,std::size_t b){
    {T{}(a,b)};
};

consteval std::size_t processKey(std::size_t key){
    return key | ( 1<<31);
}

template<meta::info storageR, std::size_t keyV, proper_cbrng_lambda F = decltype([](std::size_t ctr, std::size_t key){return square_CBRNG(ctr,key);}) >
struct consteval_rng{
    using counter = consteval_counter<storageR>;
    static constexpr std::size_t key = processKey(keyV);
    static consteval void next(){
        counter::increment();
    }

    static consteval auto get(){
        return F{}(counter::get()+1,key);
    }

    static consteval auto getInRange(std::size_t min =0, std::size_t max=0){
        if(max < min){
            auto tmp = max;
            max = min;
            min = tmp;
        }
        std::size_t range = max-min;
        return get()%range + min;
    }

};  