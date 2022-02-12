#pragma once

namespace fluentiter {
  template <class Input, typename T> class Iterator;

  template <class Input, typename T, typename U, typename F> class MapIterator final
      : public Iterator<MapIterator<Input, T, U, F>, U> {
  private:
    Input& m_iter;
    F m_f;

  public:
    MapIterator(Input& iter, F f) : m_iter(iter), m_f(f) {}

    std::optional<U> next() override {
      auto value = m_iter.next();
      if (!value) return {};

      return m_f(*value);
    }
  };
}  // namespace fluentiter