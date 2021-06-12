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

int main(){
    std::cout<<"Cache initialing...\n";
    
    std::cout<<"SIMD-Width: " << boost::numeric::ublas::detail::simd_width<<'\n';
    
    auto m = boost::numeric::ublas::cache_manager;
    for(auto j = 1ul; auto const& l : m){
        std::cout<<"Level(" << j++ << ") => ";
        if(!l.has_value()) std::cout<<"None\n";
        std::cout<<*l<<'\n';
    }
    return 0;
}