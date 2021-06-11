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
                    auto pos = static_cast<std::size_t>(c.Level) - 1u;
                    if(pos > 2ul) continue;

                    if( ( pos == 0u && c.Type == CacheData ) || pos > 0u){
                        auto const associativity = static_cast<std::size_t>(c.Associativity);
                        auto const line_size = static_cast<std::size_t>(c.LineSize);
                        auto const size = static_cast<std::size_t>(c.Size);
                        auto const sets = size / ( associativity * line_size );
                        res[pos] = cache_info{line_size, associativity, sets};
                    }
                }
            }

            return res;
        }

    } // namespace detal
    

    struct cache_manager{
        using value_type = std::optional<cache_info>;
        using base_type = std::array<value_type,3ul>;
        using size_type = typename base_type::size_type;
        using const_reference = typename base_type::const_reference;
        using const_iterator = typename base_type::const_iterator;

        constexpr cache_manager() = default;
        constexpr cache_manager(cache_manager const&) = default;
        constexpr cache_manager(cache_manager &&) = default;
        constexpr cache_manager& operator=(cache_manager const&) = default;
        constexpr cache_manager& operator=(cache_manager &&) = default;
        ~cache_manager() = default;

        constexpr const_reference operator[](size_type k) const { return m_data[k]; }
        constexpr const_reference at(size_type k){ return m_data.at(k); }

        constexpr const_iterator begin() const noexcept{ return m_data.begin(); }
        constexpr const_iterator end() const noexcept{ return m_data.end(); }

        constexpr static const_reference l1() noexcept{ return m_data[0ul]; }
        constexpr static const_reference l2() noexcept{ return m_data[1ul]; }
        constexpr static const_reference l3() noexcept{ return m_data[2ul]; }

        constexpr static auto is_valid(std::size_t k) noexcept 
            -> bool
        {
            return m_data[k].has_value();
        }

        constexpr static auto size(size_type k) 
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            auto const& cache = m_data[k];
            return cache->sets * cache->line_size * cache->associativity;
        }

        constexpr static auto line_size(size_type k) 
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            return m_data[k]->line_size;
        }

        constexpr static auto sets(size_type k) 
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            return m_data[k]->sets;
        }

        constexpr static auto associativity(size_type k) 
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            return m_data[k]->associativity;
        }

    private:
        static base_type const m_data;
    };

    cache_manager::base_type const cache_manager::m_data = detail::get_cache_info();

} // namespace boost::numeric::ublas

#endif // BOOST_NUMERIC_UBLAS_OS_WINDOWS_CACHE_HPP
