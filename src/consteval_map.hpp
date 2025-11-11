#pragma once
#include <meta>

namespace meta = std::meta;

consteval std::string info_to_string(meta::info r) {
    auto sv = meta::display_string_of(r);
    return std::string(sv.data(), sv.size());
}

consteval bool is_info_a_value(meta::info r){
    return meta::is_value(r) || meta::is_object(r);
}

template <meta::info r>
concept is_proper_sotrage_type =
    meta::is_class_template(r) &&meta::can_substitute(r,{meta::reflect_constant(^^void)})
    && !meta::is_complete_type(meta::substitute(r,{meta::reflect_constant(^^void)}));

template <meta::info storageR>
requires is_proper_sotrage_type<storageR>
struct consteval_map {
    private:
        consteval_map() = delete;
    
    template <auto v>
    struct value_tag {
        static constexpr auto value = v;
    };
    public:
    static consteval bool check_refl(meta::info k) {
        return meta::is_complete_type(meta::substitute(storageR, {meta::reflect_constant(k)}));
    }

    template <meta::info k, meta::info v>
    static consteval void put_refl() {
        if constexpr (!check_refl(k)) {
            auto valueRefl = ^^value_tag<v>;
            //if(!meta::is_complete_type(meta::substitute(storageR, {meta::reflect_constant(k)})))
            meta::define_aggregate(meta::substitute(storageR, {meta::reflect_constant(k)}),
                                   {meta::data_member_spec(valueRefl, {.name = "value"})});
        } else {
            constexpr auto value = get_refl<k>();
            static_assert(false, "key: \"" + info_to_string(k) +
                                     "\" all ready exists with value \"" + info_to_string(value) +
                                     "\", not \"" + info_to_string(v) + "\"");
        }
    }
    struct map_error_key_doesnt_exist {};
    template <meta::info k>
    static consteval meta::info get_refl() {
        if constexpr (!check_refl(k)) {
            static_assert(false, "no key: \"" + info_to_string(k) + "\"");
            return ^^map_error_key_doesnt_exist;
        } else {
            using strgT = typename[:meta::substitute(storageR, {meta::reflect_constant(k)}):];
            return decltype(strgT::value)::value;
        }
    }
    private:
    template<meta::info k>
    static consteval auto get_value(){
        constexpr meta::info refl = get_refl<k>();
        if constexpr(is_info_a_value(refl)){
            return [:refl:];
        }else{
            static_assert(false,"the reflection of \""+info_to_string(refl)+"\" is not a value or object");
            return;
        }
    }

    template<meta::info k>
    static consteval auto get_type() -> typename[:get_refl<k>():]{
        auto refl = get_refl<k>();
        static_assert(meta::is_type(refl),"reflection \""+info_to_string(refl)+"\" not a type");
    }
    public:
    template<typename k>
    static consteval bool check(){
        return check_refl(^^k);
    }

    template<auto k>
    static consteval bool check(){
        return check_refl(meta::reflect_constant(k));
    }

    template<typename k>
    static consteval bool check_is_type(){
        if constexpr(check<k>()){
            return meta::is_type(get_refl<^^k>());
        }else{
            return false;
        }
    }

    template<typename k>
    static consteval bool check_is_value(){
        if constexpr(check<k>()){
            return is_info_a_value(get_refl<^^k>());
        }else{
            return false;
        }
    }

    template<auto k>
    static consteval bool check_is_type(){
        if constexpr(check<k>()){
            return meta::is_type(get_refl<meta::reflect_constant(k)>());
        }else{
            return false;
        }
    }

    template<auto k>
    static consteval bool check_is_value(){
        if constexpr(check<k>()){
            return is_info_a_value(get_refl<meta::reflect_constant(k)>());
        }else{
            return false;
        }
    }

    template<typename k, typename v>
    static consteval void put(){
        put_refl<^^k,^^v>();
    }

    template<typename k, auto v>
    static consteval void put(){
        put_refl<^^k,meta::reflect_constant(v)>();
    }

    template<auto k, typename v>
    static consteval void put(){
        put_refl<meta::reflect_constant(k),^^v>();
    }

    template<auto k, auto v>
    static consteval void put(){
        put_refl<meta::reflect_constant(k),meta::reflect_constant(v)>();
    }

    template<typename k>
    consteval meta::info get(){
        return get_refl<^^k>();
    }

    template<auto k>
    consteval meta::info get(){
        return get_refl<meta::reflect_constant(k)>();
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