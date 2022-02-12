#pragma once

namespace fluentiter {
    template<class Input, typename T>
    class Iterator;

    template<class Input, typename T, typename P>
    class FilterIterator final : public Iterator<FilterIterator<Input, T, P>, T> {
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
    };
}