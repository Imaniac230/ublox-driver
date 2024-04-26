#ifndef TRAITS_H
#define TRAITS_H

#include <vector>

namespace Traits {
    template<typename, typename = void>
    inline static constexpr bool is_iterable = false;

    template<class T>
    inline static constexpr bool
            is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> = true;

    template<class T, typename VT>
    inline static constexpr bool has_value_type = std::is_same_v<VT, typename T::value_type>;

    template<class T, typename InnerType>
    concept IterableWith = is_iterable<T> && has_value_type<T, InnerType>;
}// namespace Traits

#endif//TRAITS_H
