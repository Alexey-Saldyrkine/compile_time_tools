#pragma once
#include <meta>

namespace meta = std::meta;

consteval std::string info_to_string(meta::info r) {
    auto sv = meta::display_string_of(r);
    return std::string(sv.data(), sv.size());
}
template <meta::info r>
concept is_proper_sotrage_type =
    meta::is_class_template(r) &&meta::can_substitute(r,{meta::reflect_constant(^^void)})
    && !meta::is_complete_type(meta::substitute(r,{meta::reflect_constant(^^void)}));

template <meta::info storageR>
requires is_proper_sotrage_type<storageR>
struct consteval_map {
    template <auto v>
    struct value_tag {
        static constexpr auto value = v;
    };
    template <meta::info k>
    
    static consteval bool check() {
        return meta::is_complete_type(meta::substitute(storageR, {meta::reflect_constant(k)}));
    }

    static consteval bool check_nonTemplate(meta::info k) {
        return meta::is_complete_type(meta::substitute(storageR, {meta::reflect_constant(k)}));
    }

    template <meta::info k, meta::info v>
    static consteval void put() {
        if constexpr (!check<k>()) {
            auto valueRefl = ^^value_tag<v>;
            //if(!meta::is_complete_type(meta::substitute(storageR, {meta::reflect_constant(k)})))
            meta::define_aggregate(meta::substitute(storageR, {meta::reflect_constant(k)}),
                                   {meta::data_member_spec(valueRefl, {.name = "value"})});
        } else {
            constexpr auto value = get<k>();
            static_assert(false, "key: \"" + info_to_string(k) +
                                     "\" all ready exists with value \"" + info_to_string(value) +
                                     "\", not \"" + info_to_string(v) + "\"");
        }
    }
    struct map_error_key_doesnt_exist {};
    template <meta::info k>
    static consteval meta::info get() {
        if constexpr (!check<k>()) {
            static_assert(false, "no key: \"" + info_to_string(k) + "\"");
            return ^^map_error_key_doesnt_exist;
        } else {
            using strgT = typename[:meta::substitute(storageR, {meta::reflect_constant(k)}):];
            return decltype(strgT::value)::value;
        }
    }

    template<meta::info k>
    static consteval auto get_value(){
        constexpr meta::info refl = get<k>();
        if constexpr(meta::is_value(refl) || meta::is_object(refl)){
            return [:refl:];
        }else{
            static_assert(false,"the reflection of \""+info_to_string(refl)+"\" is not a value or object");
            return;
        }
    }

    template<meta::info k>
    static consteval auto get_type() -> typename[:get<k>():]{
        auto refl = get<k>();
        static_assert(meta::is_type(refl),"reflection \""+info_to_string(refl)+"\" not a type");
    }

    template<typename k>
    static consteval bool checkT(){
        return check<^^k>();
    }

    template<auto k>
    static consteval bool checkV(){
        return check<meta::reflect_constant(k)>();
    }

    template<typename k>
    static consteval bool checkT_is_type(){
        if constexpr(checkT<k>){
            return meta::is_type(get<^^k>);
        }else{
            return false;
        }
    }

    template<typename k>
    static consteval bool checkT_is_value(){
        if constexpr(checkT<k>){
            return meta::is_value(get<^^k>);
        }else{
            return false;
        }
    }

    template<auto k>
    static consteval bool checkV_is_type(){
        if constexpr(checkV<k>){
            return meta::is_type(get<meta::reflect_constant(k)>);
        }else{
            return false;
        }
    }

    template<auto k>
    static consteval bool checkV_is_value(){
        if constexpr(checkV<k>){
            return meta::is_value(get<meta::reflect_constant(k)>);
        }else{
            return false;
        }
    }

    template<typename k, typename v>
    static consteval void putTT(){
        put<^^k,^^v>();
    }

    template<typename k, auto v>
    static consteval void putTV(){
        put<^^k,meta::reflect_constant(v)>();
    }

    template<auto k, typename v>
    static consteval void putVT(){
        put<meta::reflect_constant(k),^^v>();
    }

    template<auto k, auto v>
    static consteval void putVV(){
        put<meta::reflect_constant(k),meta::reflect_constant(v)>();
    }

    template<typename k>
    using getT_t = decltype(get_type<^^k>());

    template<typename k>
    static constexpr auto getT_v = get_value<^^k>(); 

    template<auto k>
    using getV_t = decltype(get_type<meta::reflect_constant(k)>());

    template<auto k>
    static constexpr auto getV_v = get_value<meta::reflect_constant(k)>();

};