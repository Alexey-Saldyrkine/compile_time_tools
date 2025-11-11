#pragma once
#include <meta>

namespace meta = std::meta;

template<meta::info storageR, std::size_t hint = 100>
struct consteval_counter{

    private:
    static consteval meta::info substitute(std::size_t index){
        return meta::substitute(storageR, { std::meta::reflect_constant(index) });
    }
    consteval_counter() = delete;
    public:

    static consteval std::size_t get(){
        std::size_t r = hint;
        while(meta::is_complete_type(substitute(r))){
            r*=hint;
        }
        std::size_t l =r/hint-1;
        while(l<r){
            std::size_t mid = (l+r)/2;
            if(meta::is_complete_type(substitute(mid))){
                l = mid+1;
            }else{
                r = mid;
            }
        }
        return l;
    }

    static consteval void increment(std::size_t n = 1){
        std::size_t offset = get();
            for(int i=0;i<n;i++){
                meta::define_aggregate(substitute(offset+i),{});
            }
    }
};



