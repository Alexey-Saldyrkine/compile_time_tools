#pragma once
#include "consteval_map.hpp"

template<meta::info storageR, std::size_t C = 100>
requires (C >1)
struct consteval_mutable{
    using storage = consteval_map<storageR>;

    static consteval std::size_t get_last_index(){
        std::size_t r = C;
        while(storage::check_nonTemplate(meta::reflect_constant(r))){
            r*=C;
        }
        std::size_t l =r/C-1;
        while(l<r){
            std::size_t mid = (l+r)/2;
            if(storage::check_nonTemplate(meta::reflect_constant(mid))){
                l = mid+1;
            }else{
                r = mid;
            }
        }
        return l-1;
    }

    template<meta::info v, std::size_t index = get_last_index()+1>
    static consteval void put(){
        storage::template put<meta::reflect_constant(index),v>();
    }

    template<auto v,std::size_t index = get_last_index()+1>
    static consteval void putV(){
        put<meta::reflect_constant(v),index>();
    }

    template<typename v,std::size_t index = get_last_index()+1>
    static consteval void putT(){
        put<^^v,index>();
    }

    struct consteval_mutable_error_its_empty;
    template<std::size_t index = (get_last_index())>
    static consteval meta::info get(){
        if constexpr(index != -1){
            return storage::template get<meta::reflect_constant(index)>();
        }else{
            static_assert(false,"the mutable with storage reflection \""+info_to_string(storageR)+"\" is empty");
            return ^^consteval_mutable_error_its_empty;
        }
    }
    private:
    template<std::size_t index>
    static consteval auto get_v_impl(){
        constexpr auto refl = get<index>();
        if constexpr(meta::is_value(refl) || meta::is_object(refl)){
            return [:refl:];
        }else{
            static_assert(false,"the reflection \""+info_to_string(refl)+"\" is not a value or object");
            return;
        }
    }

    template<std::size_t index>
    static consteval auto get_type_impl()->typename[:get<index>():]{
        auto refl = get<index>();
        static_assert(meta::is_type(refl),"the reflection \""+info_to_string(refl)+"\" is not a type");
    }
    public:
    template<std::size_t index = get_last_index()>
    static constexpr auto get_v = get_v_impl<index>();

    template<std::size_t index = get_last_index()>
    using get_t = decltype(get_type_impl<index>());

    static consteval bool check(){
        return get_last_index()!=-1;
    }
    template<std::size_t index = get_last_index()>
    static consteval bool checkV(){
        if constexpr(check()){
            return meta::is_value(get<index>()) || meta::is_object(get<index>());
        }else{ 
            return false;
        }
    }
    template<std::size_t index = get_last_index()>
    static consteval bool checkT(){
        if constexpr(check()){
            return meta::is_type(get<index>());
        }else{ 
            return false;
        }
    }

    static consteval std::vector<meta::info> toVector(){
        
    }
};



