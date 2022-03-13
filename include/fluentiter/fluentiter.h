#pragma once

#include <concepts>
#include <functional>
#include <iterator>
#include <optional>

#include "collectors/vectorcollector.h"
#include "ops/filter.h"
#include "ops/map.h"
#include "ops/zip.h"

namespace fluentiter {

  template <typename F, typename In, typename Out> concept MapOperation
      = std::is_same_v<Out, std::invoke_result_t<F, In>>;

  template <typename F, typename In> concept FilterOperation
      = std::is_same_v<bool, std::invoke_result_t<F, In>>;

  template <typename F, typename Acc, typename T> concept ReducerOperation
      = std::is_same_v<Acc, std::invoke_result_t<F, Acc, T>>;

  // clang-format off
  template <typename C, typename Out, typename Iter, typename T>
  concept Collector = std::is_trivially_constructible_v<C>
      && std::is_base_of_v<Iterator<Iter, T>, Iter>
      && requires(C c, Iter& iter) { std::is_same_v<Out, decltype(c.collect(iter))>; };
  // clang-format on

  template <class CurType, typename T> class Iterator {
  public:
    virtual ~Iterator() = default;

    /**
     * Get the current item and advance the iterator.
     *
     * @return The current item, or an empty optional if the iterator ran out of items.
     */
    virtual std::optional<T> next() = 0;

    /**
     * Returns the bounds on the remaining length of the iterator. The first component of the pair
     * is the lower bound of the remaining length, and the second component is the upper bound of
     * the remaining length.
     *
     * Since there exist operators such as filter() that alter the remaining length of the iterator,
     * the actual length cannot be known in advance. Hence, this only serves as a hint. The iterator
     * is guaranteed not to produce more values than the hinted upper bound, or produce less values
     * than the indicated lower bound.
     *
     * @return The bounds on the remaining length of the iterator.
     */
    virtual std::pair<size_t, std::optional<size_t>> size_hint() {
      return std::make_pair<size_t, std::optional<size_t>>(0, {});
    }

    /**
     * Applies the function @p f to every element in the iterator.
     *
     * @tparam F The type of the function being applied to every element.
     * @tparam U The type of the return value of the mapper function.
     * @return An iterator whose values are the result of the function @p f applied to each element.
     */
    template <typename F, typename U = std::invoke_result_t<F, T>>
    requires MapOperation<F, T, U> MapIterator<CurType, T, U, F> map(F f) {
      return MapIterator<CurType, T, U, F>(static_cast<CurType&>(*this), f);
    }

    /**
     * Filters out the values which don't satisfy the predicate @p f.
     *
     * @tparam F The type of the predicate.
     * @param f The predicate to test the elements.
     * @return An iterator whose values pass the predicate @p f.
     */
    template <typename F> requires FilterOperation<F, T> FilterIterator<CurType, T, F> filter(F f) {
      return FilterIterator<CurType, T, F>(static_cast<CurType&>(*this), f);
    }

    /**
     * Combines two iterators by pairing up the elements of the current iterator and @p other.
     *
     * @tparam Other The type of other iterator.
     * @tparam U The type of the elements in @p other.
     * @param other The other iterator.
     * @return An iterator which produces pairs of the elements in the current iterator and @p
     * other.
     */
    template <typename Other, typename U>
    requires std::is_base_of_v<Iterator<Other, U>, Other> ZipIterator<CurType, T, Other, U> zip(
        Other&& other) {
      return ZipIterator<CurType, T, Other, U>(static_cast<CurType&>(*this), other);
    }

    /**
     * Reduce the values in this iterator to a single value.
     *
     * @tparam U The type of the value to reduce to, also known as the accumulator.
     * @tparam F The type of the reducer function.
     * @param reducer The reducer to apply to each element.
     * @param initial_value The initial value of the accumulator.
     * @return The accumulator after @p reducer has been applied to all elements in the iterator.
     */
    template <typename U, typename F>
    requires ReducerOperation<F, U, T> U reduce(F reducer, U initial_value) {
      auto value = next();
      auto acc = initial_value;

      while (value) {
        acc = reducer(acc, *value);
        value = next();
      }

      return acc;
    }

    template <class Q = T> typename std::enable_if<std::is_same<Q, bool>::value, bool>::type any() {
      return reduce([](auto acc, auto elem) { return acc || elem; }, false);
    }

    template <class Q = T>
    typename std::enable_if<std::is_same<Q, bool>::value, bool>::type none() {
      return !any();
    }

    template <class Q = T> typename std::enable_if<std::is_same<Q, bool>::value, bool>::type all() {
      return reduce([](auto acc, auto elem) { return acc && elem; }, true);
    }

    /**
     * Collect the values in the iterator into some container.
     *
     * @tparam C The type of the collector.
     * @tparam U The type of the container.
     * @return The container with all the values collected into it.
     */
    template <class C, typename U> requires Collector<C, U, CurType, T> U collect() {
      C collector;
      return collector.collect(static_cast<CurType&>(*this));
    }
  };

  template <class InputIt, typename T = typename std::iterator_traits<InputIt>::value_type>
  class FluentIter final : public Iterator<FluentIter<InputIt, T>, T> {
  private:
    InputIt m_current;
    InputIt m_end;
    size_t m_size;

  public:
    FluentIter(InputIt current, InputIt end, size_t size)
        : m_current(current), m_end(end), m_size(size) {}

    std::optional<T> next() override {
      if (m_current == m_end) return {};

      T value = *m_current;
      ++m_current;
      return value;
    }

    std::pair<size_t, std::optional<size_t>> size_hint() override {
      return std::make_pair(m_size, m_size);
    }
  };

  template <typename Iter>
  requires std::is_same_v<typename std::iterator_traits<Iter>::iterator_category,
                          std::random_access_iterator_tag> inline FluentIter<Iter>
  from(Iter begin, Iter end) {
    return FluentIter(begin, end, end - begin);
  }

}  // namespace fluentiter