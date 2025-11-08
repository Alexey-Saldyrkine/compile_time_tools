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
    static consteval bool check(){
        return map::template check<str>();
    }

    template<universal_enum_fixed_string str>
    static consteval bool check_is_value(){
        return map::template check_is_value<str>();
    }

    template<universal_enum_fixed_string str>
    static consteval bool check_is_type(){
        return map::template check_is_type<str>();
    }

    template<universal_enum_fixed_string str>
    static consteval void put(){
        map::template put<str,counter::get()>();
        counter::increment();
    }

    template<universal_enum_fixed_string str, auto v>
    static consteval void put(){
        map::template put<str,v>();
    }

    template<universal_enum_fixed_string str, typename T>
    static consteval void put(){
        map::template put<str,T>();
    }

    template<universal_enum_fixed_string str>
    using get_t = map::template getV_t<str>;

    template<universal_enum_fixed_string str>
    static constexpr auto get_v = map::template getV_v<str>;
};

