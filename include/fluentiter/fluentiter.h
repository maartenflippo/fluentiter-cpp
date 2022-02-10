#pragma once

#include <iterator>
#include <optional>

namespace fluentiter {

    template <typename T>
    class Iterator {
    public:
        virtual ~Iterator() = default;

        /**
         * Get the current item and advance the iterator. If has_next() is true, then this will return the current item,
         * otherwise an empty optional.
         *
         * @return The current item, or an empty optional if the iterator ran out of items.
         */
        virtual std::optional<T> next() = 0;
    };

    template <class InputIt, typename T = typename std::iterator_traits<InputIt>::value_type>
    class FluentIter final : public Iterator<T> {
    private:
        InputIt m_current;
        InputIt m_end;

    public:
        FluentIter(InputIt current, InputIt end) : m_current(current), m_end(end) {}

        std::optional<T> next() override {
            if (m_current == m_end)
                return {};

            T value = *m_current;
            ++m_current;
            return value;
        }
    };

}