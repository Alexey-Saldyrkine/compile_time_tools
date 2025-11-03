#include "consteval_map.hpp"
#include "consteval_mutable.hpp"

template<meta::info storageR, std::size_t mutable_hint = 100>
struct consteval_mutable_map{

    template<auto key>
    using mutV = consteval_mutable<storageR,mutable_hint,meta::reflect_constant(key)>;

    template<typename key>
    using mutT = consteval_mutable<storageR,mutable_hint,^^key>;


    template<auto key>
    static consteval bool check(){
        return mutV<key>::check();
    }

    template<typename key>
    static consteval bool check(){
        return mutT<key>::check();
    }

    template<auto key>
    static consteval bool check_is_value(){
         if constexpr(check<key>()){
            return mutV<key>::check_is_value();
         }else{
            return false;
         }
    }
    
    template<auto key>
    static consteval bool check_is_type(){
         if constexpr(check<key>()){
            return mutV<key>::check_is_type();
         }else{
            return false;
         }
    }

    template<typename key>
    static consteval bool check_is_value(){
         if constexpr(check<key>()){
            return mutT<key>::check_is_type();
         }else{
            return false;
         }
    }

    template<typename key>
    static consteval bool check_is_type(){
         if constexpr(check<key>()){
            return mutT<key>::check_is_type();
         }else{
            return false;
         }
    }


    template<auto key, auto value, std::size_t index = mutV<key>::get_last_index()>
    static consteval void put(){
        mutV<key>::template put<value>();
    }

    template<auto key, typename value, std::size_t index = mutV<key>::get_last_index()>
    static consteval void put(){
        mutV<key>::template put<value>();
    }

    template<typename key, auto value, std::size_t index = mutT<key>::get_last_index()>
    static consteval void put(){
        mutT<key>::template put<value>();
    }

    template<typename key, typename value, std::size_t index = mutT<key>::get_last_index()>
    static consteval void put(){
        mutT<key>::template put<value>();
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
        mut_map::put<1,2>();
        mut_map::put<35,7>();
        mut_map::put<66,int>();
        mut_map::put<char,99>();
        mut_map::put<long,void>();
    }

    static_assert(mut_map::check_is_value<1>());
    static_assert(mut_map::check_is_type<66>());


    static_assert(mut_map::getV_v<1> == 2);
    static_assert(mut_map::getV_v<35> == 7);
    static_assert(std::is_same_v<mut_map::getV_t<66>,int>);
    static_assert(mut_map::getT_v<char> == 99);

    consteval{
        mut_map::put<1,246>();
        mut_map::put<35,543>();
    }

    static_assert(mut_map::getV_v<1> == 246);
    static_assert(mut_map::getV_v<35> == 543);


}


struct A{

    template<auto,auto>
    static consteval auto f(){
        return 1;
    }

    template<auto,typename>
    static consteval auto f(){
        return 2.1;
    }

    template<typename,auto>
    static consteval auto f(){
        return 3ll;
    }

    template<typename ,typename>
    static consteval auto f(){
        return char(4);
    }
};

static_assert(A::f<1,1>() == 1);
static_assert(A::f<1,int>() == 2.1);
static_assert(A::f<int,1>() == 3ll);
static_assert(A::f<int,int>() == 4);