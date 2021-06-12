#include <iostream>
#include "cache_manager.hpp"


// template<typename T>
// void print(std::ostream& os, T const& c){
//     using value_type = typename T::value_type;
//     os << "[ ";
//     std::copy(c.begin(), c.end() - 1, std::ostream_iterator<value_type const&>(os, ", "));
//     os << c.back() << "]\n";
// }

std::ostream& operator<<(std::ostream& os, boost::numeric::ublas::cache_info const& c){
    return os << "LineSize: " << c.line_size
        << ", Assoc: " << c.associativity
        << ", Sets: " << c.sets;

}

void normalize_size(std::ostream& os, std::size_t s) noexcept{
    auto n = (s >> 10);
    if(n == 0ul){
        os << s << "B";
    }
    
    s = n;
    n >>= 10;

    if(n == 0ul) os << s << "KiB";
    else os << n << "MiB";
}

#ifdef BOOST_NUMERIC_UBLAS_NO_MAKE_CACHE_MANAGER
namespace boost::numeric::ublas{
    constexpr cache_manager_t make_cache_manager() noexcept{
        return cache_manager_t{ cache_info{64,4,256}, cache_info{64,8,512} };
    }
}
#endif

int main(){
    std::cout<<"Cache initialing...\n";
    
    std::cout<<"Register(SIMD) Width[Float]: " << boost::numeric::ublas::detail::simd_register_width<float><<'\n';
    std::cout<<"Register(SIMD) Width[Double]: " << boost::numeric::ublas::detail::simd_register_width<double><<'\n';
    
    auto m = boost::numeric::ublas::cache_manager;
    for(std::size_t j = 1ul, i = 0ul; auto const& l : m){
        std::cout<<"Level(" << j++ << ") => ";
        if(!l.has_value()) {
            std::cout<<"None\n";
            continue;
        }
        std::cout<<*l<<", Size: ";
        normalize_size(std::cout, m.size(i++).value());
        std::cout<<'\n';
    }
    return 0;
}