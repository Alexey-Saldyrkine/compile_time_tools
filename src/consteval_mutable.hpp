#pragma once
#include "consteval_map.hpp"

template<meta::info storageR, std::size_t search_hint = 100, meta::info key = ^^void>
requires (search_hint >1)
struct consteval_mutable{
    using storage = consteval_map<storageR>;

    static consteval meta::info get_refl(std::size_t index){
        return meta::reflect_constant(std::pair<meta::info,meta::info>{key,meta::reflect_constant(index)}); 
    }

    static consteval std::size_t get_last_index(){
        std::size_t r = search_hint;
        while(storage::check(get_refl(r))){
            r*=search_hint;
        }
        std::size_t l =r/search_hint-1;
        while(l<r){
            std::size_t mid = (l+r)/2;
            if(storage::check(get_refl(mid))){
                l = mid+1;
            }else{
                r = mid;
            }
        }
        return l-1;
    }

    template<meta::info v, std::size_t index = get_last_index()+1>
    static consteval void put(){
        storage::template put<get_refl(index),v>();
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
            return storage::template get<get_refl(index)>();
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

};



