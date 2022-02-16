#pragma once

#include <functional>
#include <iterator>
#include <optional>

#include "collectors/vectorcollector.h"
#include "ops/filter.h"
#include "ops/map.h"

namespace fluentiter {

  template <class CurType, typename T> class Iterator {
  public:
    virtual ~Iterator() = default;

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
    MapIterator<CurType, T, U, F> map(F f) {
      return MapIterator<CurType, T, U, F>(static_cast<CurType&>(*this), f);
    }

    template <typename F> FilterIterator<CurType, T, F> filter(F f) {
      return FilterIterator<CurType, T, F>(static_cast<CurType&>(*this), f);
    }

    template <typename U, typename F> U reduce(F reducer, U initial_value) {
      auto iter = static_cast<CurType&>(*this);

      auto value = iter.next();
      auto acc = initial_value;

      while (value) {
        acc = reducer(acc, *value);
        value = iter.next();
      }

      return acc;
    }

    template <class Collector, typename U> U collect() {
      Collector collector;
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