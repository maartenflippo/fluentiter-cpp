#pragma once

#include <optional>
#include <utility>

namespace fluentiter {
  template <class Input, typename T> class Iterator;

  template <class I1, typename T1, typename I2, typename T2> class ZipIterator final
      : public Iterator<ZipIterator<I1, T1, I2, T2>, std::pair<T1, T2>> {
  private:
    I1& m_input1;
    I2& m_input2;

  public:
    ZipIterator(I1& input1, I2& input2) : m_input1(input1), m_input2(input2) {}

    std::optional<std::pair<T1, T2>> next() override {
      auto value1 = m_input1.next();
      auto value2 = m_input2.next();
      if (!value1 || !value2) return {};

      return std::make_pair(*value1, *value2);
    }

    std::pair<size_t, std::optional<size_t>> size_hint() override {
      return m_input1.size_hint();
    }
  };
}  // namespace fluentiter