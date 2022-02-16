#pragma once

namespace fluentiter {
  template <class Input, typename T> class Iterator;

  template <class Input, typename T, typename P> class FilterIterator final
      : public Iterator<FilterIterator<Input, T, P>, T> {
  private:
    Input& m_input;
    P m_predicate;

  public:
    FilterIterator(Input& input, P predicate) : m_input(input), m_predicate(predicate) {}

    std::optional<T> next() override {
      auto value = m_input.next();
      if (!value) return {};

      return m_predicate(*value) ? value : next();
    }

    std::pair<size_t, std::optional<size_t>> size_hint() override {
      auto [lb, ub] = m_input.size_hint();
      return std::make_pair(0, ub);
    }
  };
}  // namespace fluentiter