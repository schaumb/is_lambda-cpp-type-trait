#ifndef BXLX_IS_LAMBDA_INCLUDED_HPP
#define BXLX_IS_LAMBDA_INCLUDED_HPP

#include "nameof.hpp" // https://raw.githubusercontent.com/Neargye/nameof/master/include/nameof.hpp

namespace bxlx {
    namespace impl {
        template<typename T>
        constexpr bool is_lambda() {
            constexpr std::string_view wholeName = nameof::nameof_type<T>();
#if defined(__clang__)
            return wholeName.rfind("(lambda at ", 0) == 0 && *wholeName.rbegin() == ')';
#elif defined(__GNUC__)
            static_assert(false, "TODO it's harder than I thought first");
#elif defined(_MSC_VER)
            return wholeName.rfind("class <lambda_", 0) == 0 && *wholeName.rbegin() == '>';
#endif
        }
    }

    template<typename T>
    struct is_lambda : std::integral_constant<bool, impl::is_lambda<std::decay_t<T>>()> {};

    template<typename T>
    constexpr bool is_lambda_v = typename is_lambda<T>::value;
}

#ifdef IS_LAMBDA
#undef IS_LAMBDA
#endif // IS_LAMBDA

#define IS_LAMBDA(x)                                        \
[]{                                                         \
    auto&& v = x;                                           \
    return bxlx::is_lambda_v<std::decay_t<decltype(v)>>;    \
}()

#endif // BXLX_IS_LAMBDA_INCLUDED_HPP
