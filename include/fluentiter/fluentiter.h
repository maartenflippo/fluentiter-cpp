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

  template <typename F, typename In, typename Out>
  concept MapOperation = std::is_same_v<Out, std::invoke_result_t<F, In>>;

  template <typename F, typename In>
  concept FilterOperation = std::is_same_v<bool, std::invoke_result_t<F, In>>;

  template <typename F, typename Acc, typename T>
  concept ReducerOperation = std::is_same_v<Acc, std::invoke_result_t<F, Acc, T>>;

  // clang-format off
  template <typename C, typename Out, typename Iter, typename T>
  concept Collector = std::is_trivially_constructible_v<C>
      && std::is_base_of_v<Iterator<Iter, T>, Iter>
      && requires(C c, Iter& iter) { std::is_same_v<Out, decltype(c.collect(iter))>; };
  // clang-format on

  template <class CurType, typename T> class Iterator {
  public:
    Iterator() = default;
    Iterator(Iterator&) = delete;
    Iterator(Iterator&&) noexcept = default;
    virtual ~Iterator() = default;

    Iterator<CurType, T>& operator= (Iterator<CurType, T>&&) noexcept = default;

    /**
     * Get the current item and advance the iterator. If has_next() is true, then this will return
     * the current item, otherwise an empty optional.
     *
     * @return The current item, or an empty optional if the iterator ran out of items.
     */
    virtual std::optional<T> next() = 0;

    virtual std::pair<size_t, std::optional<size_t>> size_hint() {
      return std::make_pair<size_t, std::optional<size_t>>(0, {});
    }

    template <typename F, typename U = std::invoke_result_t<F, T>>
    requires MapOperation<F, T, U> MapIterator<CurType, T, U, F> map(F f) {
      return MapIterator<CurType, T, U, F>(static_cast<CurType&>(*this), f);
    }

    template <typename F>
    requires FilterOperation<F, T> FilterIterator<CurType, T, F> filter(F f) {
      return FilterIterator<CurType, T, F>(static_cast<CurType&>(*this), f);
    }

    template <typename Other, typename U> requires std::is_base_of_v<Iterator<Other, U>, Other>
    ZipIterator<CurType, T, Other, U> zip(Other&& other) {
      return ZipIterator<CurType, T, Other, U>(static_cast<CurType&>(*this), other);
    }

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

    template <class C, typename U>
    requires Collector<C, U, CurType, T> U collect() {
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

}  // namespace fluentiter