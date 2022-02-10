#include <doctest/doctest.h>
#include <fluentiter/fluentiter.h>
#include <vector>

TEST_CASE("FluentIter yields the items in an input iterator") {
    std::vector<int> items{1, 2, 3};
    fluentiter::FluentIter iter(items.begin(), items.end());

    CHECK_EQ(1, *iter.next());
    CHECK_EQ(2, *iter.next());
    CHECK_EQ(3, *iter.next());
    CHECK_FALSE(iter.next().has_value());
}