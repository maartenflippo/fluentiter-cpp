#pragma once

#include <vector>

namespace fluentiter {
  template <typename T> class VectorCollector {
  public:
    VectorCollector() = default;

    template <class Input> std::vector<T> collect(Input& m_iter) {
      std::vector<T> items;
      auto [_, ub] = m_iter.size_hint();

      if (ub) {
        items.reserve(*ub);
      }

      auto value = m_iter.next();
      while (value) {
        items.push_back(*value);
        value = m_iter.next();
      }

      return items;
    }
  };
}  // namespace fluentiter