#pragma once
#include "consteval_map.hpp"
#include "consteval_counter.hpp"

template <std::size_t N>
    struct universal_enum_fixed_string {
        char data[N] = {};

        consteval universal_enum_fixed_string(const char (&str)[N]) {
            for (int i = 0; i < N; i++) {
                data[i] = str[i];
            }
        }

        constexpr char operator[](std::size_t i) const { return data[i]; }

        constexpr const char* to_cstr() const { return data; }
    };

template <meta::info storageR>
struct universal_enum {
    

    using map = consteval_map<storageR>;
    using counter = consteval_counter<storageR>;

    template<universal_enum_fixed_string str>
    consteval bool check(){
        return map::template checkV<str>();
    }

    template<universal_enum_fixed_string str>
    consteval bool check_is_v(){
        return map::template checkV_is_value<str>();
    }

    template<universal_enum_fixed_string str>
    consteval bool check_is_t(){
        return map::template checkV_is_type<str>();
    }

    template<universal_enum_fixed_string str>
    static consteval void put(){
        map::template putVV<str,counter::get()>();
        counter::increment();
    }

    template<universal_enum_fixed_string str, auto v>
    static consteval void putV(){
        map::template putVV<str,v>();
    }

    template<universal_enum_fixed_string str, typename T>
    static consteval void putT(){
        map::template putVT<str,T>();
    }

    template<universal_enum_fixed_string str>
    static consteval auto get(){
        return map::template getV_v<str>;
    }

    template<universal_enum_fixed_string str>
    using get_t = map::template getV_t<str>;

    template<universal_enum_fixed_string str>
    static constexpr auto get_v = map::template getV_v<str>;
};


