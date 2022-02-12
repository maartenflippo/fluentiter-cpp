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

TEST_CASE("map") {
  auto mapper = [](auto num) { return num + 1; };
  std::vector<int> items{1, 3, 5};
  auto iter = fluentiter::FluentIter(items.begin(), items.end()).map<int, decltype(mapper)>(mapper);

  CHECK_EQ(2, *iter.next());
  CHECK_EQ(4, *iter.next());
  CHECK_EQ(6, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("filter") {
  auto predicate = [](auto num) { return num % 2 == 0; };
  std::vector<int> items{1, 2, 3, 4};
  auto iter = fluentiter::FluentIter(items.begin(), items.end()).filter(predicate);

  CHECK_EQ(2, *iter.next());
  CHECK_EQ(4, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("reduce") {
  auto reducer = [](auto acc, auto num) { return acc + num; };

  std::vector<int> items{1, 2, 3, 4};
  auto sum = fluentiter::FluentIter(items.begin(), items.end()).reduce(reducer, 0);

  CHECK_EQ(sum, 1 + 2 + 3 + 4);
}

TEST_CASE("collect") {
  auto mapper = [](auto num) { return num + 1; };
  auto predicate = [](auto num) { return num % 2 == 0; };

  std::vector<int> items{1, 2, 3, 4};
  auto result = fluentiter::FluentIter(items.begin(), items.end())
                    .map<int, decltype(mapper)>(mapper)
                    .filter(predicate)
                    .collect<fluentiter::VectorCollector<int>, std::vector<int>>();

  CHECK_EQ(result, std::vector<int>{2, 4});
}
