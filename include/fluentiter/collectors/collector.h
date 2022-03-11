#pragma once

#include <vector>

namespace fluentiter {
  template <typename T>
  class Iterator;

  template <typename T> class Collector {
    virtual ~Collector() = default;

    virtual std::vector<T> collect(Iterator<T>* m_iter) = 0;
  };
}
