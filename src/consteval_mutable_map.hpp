#pragma once

#include "consteval_mutable.hpp"

template<meta::info storageR, std::size_t mutable_hint = 100>
struct consteval_mutable_map{

    template<auto key>
    using get_mutable_typeV_t = consteval_mutable<storageR,mutable_hint,meta::reflect_constant(key)>;

    template<typename key>
    using get_mutable_typeT_t = consteval_mutable<storageR,mutable_hint,^^key>;


    template<auto key>
    static consteval bool check(){
        return get_mutable_typeV_t<key>::check();
    }

    template<typename key>
    static consteval bool check(){
        return get_mutable_typeT_t<key>::check();
    }

    template<auto key>
    static consteval bool check_is_value(){
         if constexpr(check<key>()){
            return get_mutable_typeV_t<key>::check_is_value();
         }else{
            return false;
         }
    }
    
    template<auto key>
    static consteval bool check_is_type(){
         if constexpr(check<key>()){
            return get_mutable_typeV_t<key>::check_is_type();
         }else{
            return false;
         }
    }

    template<typename key>
    static consteval bool check_is_value(){
         if constexpr(check<key>()){
            return get_mutable_typeT_t<key>::check_is_type();
         }else{
            return false;
         }
    }

    template<typename key>
    static consteval bool check_is_type(){
         if constexpr(check<key>()){
            return get_mutable_typeT_t<key>::check_is_type();
         }else{
            return false;
         }
    }


    template<auto key, auto value, std::size_t index = get_mutable_typeV_t<key>::get_last_index()>
    static consteval void put(){
        get_mutable_typeV_t<key>::template put<value>();
    }

    template<auto key, typename value, std::size_t index = get_mutable_typeV_t<key>::get_last_index()>
    static consteval void put(){
        get_mutable_typeV_t<key>::template put<value>();
    }

    template<typename key, auto value, std::size_t index = get_mutable_typeT_t<key>::get_last_index()>
    static consteval void put(){
        get_mutable_typeT_t<key>::template put<value>();
    }

    template<typename key, typename value, std::size_t index = get_mutable_typeT_t<key>::get_last_index()>
    static consteval void put(){
        get_mutable_typeT_t<key>::template put<value>();
    }


    template<auto key, std::size_t index = get_mutable_typeV_t<key>::get_last_index()>
    static constexpr auto getV_v = get_mutable_typeV_t<key>::template get_v<index>;

    template<typename key, std::size_t index = get_mutable_typeT_t<key>::get_last_index()>
    static constexpr auto getT_v = get_mutable_typeT_t<key>::template get_v<index>;

    template<auto key, std::size_t index = get_mutable_typeV_t<key>::get_last_index()>
    using getV_t = get_mutable_typeV_t<key>::template get_t<index>;

    template<typename key, std::size_t index = get_mutable_typeT_t<key>::get_last_index()>
    using getT_t = get_mutable_typeT_t<key>::template get_t<index>;

};