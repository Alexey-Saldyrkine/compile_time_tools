#include "consteval_random.hpp"

namespace rng_test1{
    template<auto>
    struct storage;

    constexpr std::size_t key = 54321346;

    using rng = consteval_rng<^^storage,key>;

    consteval auto test(std::size_t ctr){
        return square_CBRNG(ctr,rng::key);
    }
    
    static_assert(rng::get() == test(1));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(2));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(3));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(4));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(5));
}

namespace rng_test2{
    template<auto>
    struct storage;
    constexpr std::size_t key = 0;
    using rng = consteval_rng<^^storage,key>;

    consteval auto test(std::size_t ctr){
        return square_CBRNG(ctr,rng::key);
    }
    
     static_assert(rng::get() == test(1));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(2));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(3));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(4));
    consteval{
        rng::next();
    }
    static_assert(rng::get() == test(5));
}

namespace rng_test3{
    template<auto>
    struct storage;
    constexpr std::size_t key = 124;
    using rng = consteval_rng<^^storage,key>;

    consteval auto test(std::size_t min, size_t max, std::size_t ctr){
        return (square_CBRNG(ctr,rng::key) %(max-min)) + min;
    }
    
     static_assert(rng::getInRange(1,6) == test(1,6,1));
    consteval{
        rng::next();
    }
    static_assert(rng::getInRange(1,10) == test(1,10,2));
    consteval{
        rng::next();
    }
    static_assert(rng::getInRange(1,100) == test(1,100,3));
    consteval{
        rng::next();
    }
    static_assert(rng::getInRange(20,40) == test(20,40,4));
    consteval{
        rng::next();
    }
    static_assert(rng::getInRange(4000,99999) == test(4000,99999,5));
}

namespace rng_test4{
    
template<auto>
struct storage;

// rng with key set to 1984
using rng = consteval_rng<^^storage,1984>;

static_assert(rng::getInRange(1,6) == 3);

consteval{
    rng::next();
}

static_assert(rng::getInRange(1,6) == 2);
consteval{
    rng::next();
}
static_assert(rng::getInRange(1,6) == 4);

consteval{
       rng::next();
}
static_assert(rng::getInRange(1,6) == 3);

consteval{
    rng::next();
}
static_assert(rng::getInRange(1,6) == 5);

}