#ifndef BXLX_IS_LAMBDA_INCLUDED_HPP
#define BXLX_IS_LAMBDA_INCLUDED_HPP

#include "nameof.hpp" // https://raw.githubusercontent.com/Neargye/nameof/master/include/nameof.hpp

namespace bxlx {
    namespace impl {
        template<typename T>
        constexpr bool is_lambda() {
            constexpr std::string_view wholeName = nameof::nameof_type<T>();
#if defined(__clang__)
            // format: "(lambda at filename:line:char)"
            return wholeName.rfind("(lambda at ", 0) == 0 && *wholeName.rbegin() == ')' 
                && wholeName.find(':') > wholeName.rfind('('); // This condition filters function pointers/references which returns lambda. 
                // '(' can be part of filename, but ':' is not. 
#elif defined(__GNUC__)
            static_assert(false, "TODO it's harder than I thought first");
#elif defined(_MSC_VER)
            // format: "class <lambda_idchars>"
            return wholeName.rfind("class <lambda_", 0) == 0 && *wholeName.rbegin() == '>';
#endif
        }
    }

    template<typename T>
    struct is_lambda : std::integral_constant<bool, impl::is_lambda<T>()> {};

    template<typename T>
    constexpr bool is_lambda_v = is_lambda<T>::value;
}

#ifdef IS_LAMBDA
#undef IS_LAMBDA
#endif // IS_LAMBDA

#define IS_LAMBDA(x)                                                    \
[&]{                                                                    \
    [[maybe_unused]]                                                    \
    auto&& _lname = [&]() -> decltype(auto) { return x; };              \
    return bxlx::is_lambda_v<std::invoke_result_t<decltype(_lname)>>;   \
}()

#endif // BXLX_IS_LAMBDA_INCLUDED_HPP
