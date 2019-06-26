// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * Test utilities for threading.
 */

#ifndef NCHAIN_SDK_TESTUTILSTHREADSH
#define NCHAIN_SDK_TESTUTILSTHREADSH

#pragma once

#include <sstream>
#include <type_traits>
#include <utility>
#include "utils/ThreadPool.h"


// Version of BOOST_CHECK for use inside threaded lambdas

#define NCHAIN_THREAD_CHECK( Pred )                                                   \
    if(!(Pred))                                                                       \
    {                                                                                 \
        log << __FILE__ << ":" << __LINE__ <<  " Threaded check failed" << std::endl; \
    }


/// Wrapper for holding the result of a call to a threaded test.
template<typename ReturnType>
class ThreadResult
{
  public:
    ThreadResult() = default;
    ThreadResult(const ReturnType& result) : mResult{result} {}
    const ReturnType& get() const { return mResult; }
  private:
    ReturnType mResult {};
};

/// Specialisation for threaded test that returns void.
template<>
class ThreadResult<void>
{
  public:
    ThreadResult() {}
    void get() const {}
};

/// Call a threaded test method that returns a non-void. Packages the result of that call in
/// a generic wrapper type and returns it along with the logging output from the test.
template<typename    Callable,
         typename... Args,
         typename    ResultType = typename std::result_of<Callable(size_t, std::stringstream&, Args...)>::type,
         typename    Enable = typename std::enable_if<! std::is_void<ResultType>::value>::type>
auto RunAndReturn(size_t n, Callable&& func, Args&&... args) -> std::tuple<std::string, ThreadResult<ResultType>>
{
    std::stringstream log {};
    return std::make_tuple(log.str(), func(n, log, std::forward<Args>(args)...));
}

/// Call a threaded test method that returns void. Returns the output from that test.
template<typename    Callable,
         typename... Args,
         typename    ResultType = typename std::result_of<Callable(size_t, std::stringstream&, Args...)>::type,
         typename    Enable = typename std::enable_if<std::is_void<ResultType>::value>::type>
auto RunAndReturn(size_t n, Callable&& func, Args&&... args) -> std::tuple<std::string, ThreadResult<void>>
{
    std::stringstream log {};
    func(n, log, std::forward<Args>(args)...);
    return std::make_tuple(log.str(), ThreadResult<void>{});
}

// Launch a threaded test N times.
// The first 2 arguments to the test (any callable object) will be the current
// N and a stringstream for logging, followed by any other specified arguments.
// It can return any required type (or void) and will return the results in a
// vector of ThreadResults.
// For example:
//
// int x { 123 };
// RunNTimesThreaded<100>([](size_t i, std::stringstream& log, int x) {
//    NCHAIN_THREAD_CHECK(x == 123);
// }, x);
//
// or:
//
// int x { 123 };
// std::vector<ThreadResult<int>> results = RunNTimesThreaded<100>([](size_t i, std::stringstream& log, int x) {
//    NCHAIN_THREAD_CHECK(x == 123);
//    return x + 1;
// }, x);
template<size_t      N,
         typename    Callable,
         typename... Args,
         typename    ResultType = typename std::result_of<Callable(size_t, std::stringstream&, Args...)>::type>
auto RunNTimesThreaded(Callable&& func, Args&&... args) -> std::vector<ThreadResult<ResultType>>
{
    // Results for each threaded call (different to what we actually return from each thread)
    using ThreadResultType = decltype(RunAndReturn(0, func, args...));
    std::vector<std::future<ThreadResultType>> threadResults {};

    // Launch N times threaded
    ThreadPool pool{};
    for(size_t n = 0; n < N; ++n)
    {
        // Once we get C++17 we can hopefully do perfect forwarding of variadic arguments into
        // our lambda, eg something like;
        // auto ourLambda = [func = std::forward<Callable>(func), args = std::forward<Args>(args)...]() {
        //     return std::apply(RunAndReturn, n, args);
        // } 
        auto ourLambda = [n, func, args...]()
        {
            return RunAndReturn(n, func, args...);;
        };
        threadResults.emplace_back(pool.enqueue(ourLambda));
    }

    // Results for our caller
    std::vector<ThreadResult<ResultType>> results {};

    // Check/build results and log failures
    for(auto& tresult : threadResults)
    {
        auto futureResult = tresult.get();
        const std::string& err { std::get<0>(futureResult) };
        BOOST_CHECK_MESSAGE(err.empty(), err);
        results.emplace_back(std::get<1>(futureResult));
    }

    return results;
}
#endif //NCHAIN_SDK_TESTUTILSTHREADSH
