#if !defined(BOOST_NUMERIC_UBLAS_OS_WINDOWS_CACHE_HPP)
#define BOOST_NUMERIC_UBLAS_OS_WINDOWS_CACHE_HPP

#include "../config.hpp"
#include <optional>
#include <windows.h>
#include <array>
#include <vector>

namespace boost::numeric::ublas{

    namespace detail{
        
        template<typename IntType>
        auto get_cache_info(){
            using value_type = std::optional<cache_info>;
            std::array<value_type,3ul> res{};

            DWORD buff_sz{};
            GetLogicalProcessorInformation(nullptr,&buff_sz);
            std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buff(buff_sz);
            GetLogicalProcessorInformation(buff.data(), &buff_sz);

            for(auto const& p : buff){
                if(p.Relationship == RelationCache){
                    auto const& c = p.Cache;
                    auto pos = static_cast<IntType>(c.Level) - 1u;
                    if(pos > 2ul) continue;

                    if( ( pos == 0u && c.Type == CacheData ) || pos > 0u){
                        auto const associativity = static_cast<IntType>(c.Associativity);
                        auto const line_size = static_cast<IntType>(c.LineSize);
                        auto const size = static_cast<IntType>(c.Size);
                        auto const sets = size / ( associativity * line_size );
                        res[pos] = cache_info{line_size, associativity, sets};
                    }
                }
            }

            return res;
        }

    } // namespace detal


    constexpr auto make_cache_manager() noexcept -> cache_manager_t{
        return cache_manager_t{detail::get_cache_info<std::size_t>()};
    }

} // namespace boost::numeric::ublas

#endif // BOOST_NUMERIC_UBLAS_OS_WINDOWS_CACHE_HPP
