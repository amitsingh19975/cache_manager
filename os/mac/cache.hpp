#if !defined(BOOST_NUMERIC_UBLAS_OS_MAC_CACHE_HPP)
#define BOOST_NUMERIC_UBLAS_OS_MAC_CACHE_HPP

#include "../config.hpp"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <optional>

namespace boost::numeric::ublas{

    namespace detail{
        
        template<typename IntType>
        constexpr std::optional<IntType> sysctlbyname_helper(std::string_view name) noexcept{
            union{
                int32_t i32;
                int64_t i64;
            } n = {0};
            
            IntType out;
            auto sz = sizeof(n);

            if(sysctlbyname(name.data(), &n, &sz, nullptr, 0) < 0) {
                return std::nullopt;
            }

            switch(sz){
                case sizeof(n.i32):
                    out = static_cast<IntType>(n.i32);
                    break;
                case sizeof(n.i64):
                    out = static_cast<IntType>(n.i64);
                    break;
                default:
                    return std::nullopt;
            }

            return out;
        }

        constexpr auto get_level_one() noexcept 
            -> std::optional<cache_info>
        {
            cache_info res{};

            res.line_size = sysctlbyname_helper<std::size_t>("hw.cachelinesize").value_or(default_l1_cache[0]);
            res.associativity = sysctlbyname_helper<std::size_t>("machdep.cpu.cache.L1_associativity").value_or(default_l1_cache[1]);
            res.sets = default_l1_cache[2];

            if( auto const cache_size_or = sysctlbyname_helper<std::size_t>("hw.l1dcachesize"); cache_size_or.has_value() ){
                auto const cache_size = cache_size_or.value();
                res.sets = cache_size / (res.associativity * res.line_size);
            }else{
                return std::nullopt;
            }

            return res;
        }

        constexpr auto get_level_two() noexcept 
            -> std::optional<cache_info>
        {
            cache_info res{};

            res.line_size = sysctlbyname_helper<std::size_t>("hw.cachelinesize").value_or(default_l2_cache[0]);
            res.associativity = sysctlbyname_helper<std::size_t>("machdep.cpu.cache.L2_associativity").value_or(default_l2_cache[1]);
            res.sets = default_l2_cache[2];

            if( auto const cache_size_or = sysctlbyname_helper<std::size_t>("hw.l2cachesize"); cache_size_or.has_value() ){
                auto const cache_size = cache_size_or.value();
                res.sets = cache_size / (res.associativity * res.line_size);
            }else{
                return std::nullopt;
            }

            return res;
        }

        constexpr auto get_level_three() noexcept 
            -> std::optional<cache_info>
        {
            #if defined(BOOST_NUMERIC_UBLAS_CACHE_L3) || defined(BOOST_NUMERIC_UBLAS_x86_64)
                cache_info res{};

                res.line_size = sysctlbyname_helper<std::size_t>("hw.cachelinesize").value_or(default_l3_cache[0]);
                res.associativity = sysctlbyname_helper<std::size_t>("machdep.cpu.cache.L3_associativity").value_or(default_l3_cache[1]);
                res.sets = default_l3_cache[2];

                if( auto const cache_size_or = sysctlbyname_helper<std::size_t>("hw.l3cachesize"); cache_size_or.has_value() ){
                    auto const cache_size = cache_size_or.value();
                    res.sets = cache_size / (res.associativity * res.line_size);
                }else{
                    return std::nullopt;
                }

                return res;
            #else
                return std::nullopt;
            #endif
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

    cache_manager::base_type const cache_manager::m_data = {detail::get_level_one(), detail::get_level_two(), detail::get_level_three()};

} // namespace boost::numeric::ublas

#endif // BOOST_NUMERIC_UBLAS_OS_MAC_CACHE_HPP
