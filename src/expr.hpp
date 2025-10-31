#include "consteval_map.hpp"
#include "consteval_mutable.hpp"

template<meta::info storageR, std::size_t mutable_hint = 100>
struct consteval_mutable_map{

    template<meta::info key>
    using mut_type = consteval_mutable<storageR,mutable_hint,key>;

    template<auto key>
    using mutV = consteval_mutable<storageR,mutable_hint,meta::reflect_constant(key)>;

    template<typename key>
    using mutT = consteval_mutable<storageR,mutable_hint,^^key>;

    template<meta::info key>
    static consteval bool check(){
        return mut_type<key>::check();
    }
    
    template<meta::info key, meta::info value>
    static consteval void put(){
        mut_type<key>::template put<value>();
    }

    template<auto key, auto value, std::size_t index = mutV<key>::get_last_index()>
    static consteval void putVV(){
        mutV<key>::template putV<value>();
    }

    template<auto key, typename value, std::size_t index = mutV<key>::get_last_index()>
    static consteval void putVT(){
        mutV<key>::template putT<value>();
    }

    template<typename key, auto value, std::size_t index = mutT<key>::get_last_index()>
    static consteval void putTV(){
        mutT<key>::template putV<value>();
    }

    template<typename key, typename value, std::size_t index = mutT<key>::get_last_index()>
    static consteval void putVV(){
        mutT<key>::template putT<value>();
    }

    template<meta::info key, std::size_t index = mut_type<key>::get_last_index()>
    static consteval auto get(){
        return mutV<key>::template get<index>();
    }

    template<auto key, std::size_t index = mutV<key>::get_last_index()>
    static constexpr auto getV_v = mutV<key>::template get_v<index>;

    template<typename key, std::size_t index = mutT<key>::get_last_index()>
    static constexpr auto getT_v = mutT<key>::template get_v<index>;

    template<auto key, std::size_t index = mutV<key>::get_last_index()>
    using getV_t = mutV<key>::template get_t<index>;

    template<typename key, std::size_t index = mutT<key>::get_last_index()>
    using getT_t = mutT<key>::template get_t<index>;

};

namespace mutable_map_test1{
    template<auto>
    struct storage;

    using mut_map = consteval_mutable_map<^^storage>;

    consteval{
        mut_map::putVV<1,2>();
        mut_map::putVV<35,7>();
        mut_map::putVT<66,int>();
        mut_map::putTV<char,99>();
    }

    static_assert(mut_map::getV_v<1> == 2);
    static_assert(mut_map::getV_v<35> == 7);
    static_assert(std::is_same_v<mut_map::getV_t<66>,int>);
    static_assert(mut_map::getT_v<char> == 99);

    consteval{
        mut_map::putVV<1,246>();
        mut_map::putVV<35,543>();
    }

    static_assert(mut_map::getV_v<1> == 246);
    static_assert(mut_map::getV_v<35> == 543);


}