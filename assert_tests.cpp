#include "is_lambda.hpp"

#include <iostream>
#include <functional>
#include <cmath>

struct NotLambda {
    void operator()() {}
};

constexpr auto l = [](){};

template<typename T, typename ...Us>
auto templateLambda = [](auto&&, Us&&...) -> T { return T{}; };

constexpr auto funWithReturnLambda() {
    return [](auto&&) { return 42; };
}

auto lamdaInsideLamda = [a = []{}] { return a; };

template<typename T>
auto operator<(T, int) {
    return [] (T&) { return 42; };
}

template<typename T>
auto x(T) { return []{}; }

template<typename T>
struct A {
    auto operator>(int a) {
        return []{} < a;
    }
};

template<typename T>
auto lambdaWithLambdaParam = [](decltype(lamdaInsideLamda), decltype(A<T>{} > 5)) {};
template<typename T = int>
void funWithLambdaParam(decltype(lamdaInsideLamda), decltype(A<T>{} > 5)) {}

namespace mstd {
    void cfun (float) {}
}

int main() {
    auto localLambda = [] (int, char) -> unsigned { return 0; };
    
    static_assert(IS_LAMBDA([](){}), "lambda");
    static_assert(IS_LAMBDA(l), "lambda");
    static_assert(IS_LAMBDA(templateLambda<int>), "lambda");
    static_assert(IS_LAMBDA(funWithReturnLambda()), "lambda");
    static_assert(IS_LAMBDA(lamdaInsideLamda()), "lambda");
    static_assert(IS_LAMBDA(A<double>{} > 5), "lambda");
    static_assert(IS_LAMBDA(localLambda), "lambda");
    static_assert(IS_LAMBDA(lambdaWithLambdaParam<int>), "lambda");

    static_assert(!IS_LAMBDA(5 - 6), "not lambda");
    static_assert(!IS_LAMBDA([]{}()), "not lambda");
    static_assert(!IS_LAMBDA(NotLambda{}), "not lambda");
    static_assert(!IS_LAMBDA(&NotLambda::operator()), "not lambda");
    static_assert(!IS_LAMBDA(A<decltype(l)>{}), "not lambda");
    static_assert(!IS_LAMBDA(A<std::decay_t<decltype(l)>>{}), "not lambda");
    static_assert(!IS_LAMBDA(A<std::remove_pointer_t<decltype(&::x<int>)>>{}), "not lambda");
    static_assert(!IS_LAMBDA(&::x <int>), "not lambda");
    static_assert(!IS_LAMBDA(&::operator< <decltype(l)>), "not lambda");
    static_assert(!IS_LAMBDA(&::mstd::cfun), "not lambda");
    static_assert(!IS_LAMBDA(&funWithLambdaParam<>), "not lambda");

    static_assert(bxlx::is_lambda_v<decltype(l)>, "lambda");
    static_assert(bxlx::is_lambda_v<decltype(templateLambda<int>)>, "lambda");
    static_assert(bxlx::is_lambda_v<std::invoke_result_t<decltype(&funWithReturnLambda)>>, "lambda");
    static_assert(bxlx::is_lambda_v<std::invoke_result_t<decltype(lamdaInsideLamda)>>, "lambda");
    static_assert(bxlx::is_lambda_v<decltype(localLambda)>, "lambda");
    
    static_assert(!bxlx::is_lambda_v<NotLambda>, "not lambda");
    static_assert(!bxlx::is_lambda_v<decltype(NotLambda{})>, "not lambda");
    static_assert(!bxlx::is_lambda_v<decltype(&NotLambda::operator())>, "not lambda");
    static_assert(!bxlx::is_lambda_v<decltype(std::bind(NotLambda{}))>, "not lambda");
    static_assert(!bxlx::is_lambda_v<std::remove_pointer_t<decltype(&::x <int>)>>, "not lambda");
    static_assert(!bxlx::is_lambda_v<std::remove_pointer_t<decltype(&::operator< <decltype(l)>)>>, "not lambda");
    static_assert(!bxlx::is_lambda_v<std::remove_pointer_t<decltype(&::mstd::cfun)>>, "not lambda");
    return 0;
}

