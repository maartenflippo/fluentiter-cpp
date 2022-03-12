#include <doctest/doctest.h>
#include <fluentiter/fluentiter.h>

#include <vector>

TEST_CASE("FluentIter yields the items in an input iterator") {
  std::vector<int> items{1, 2, 3};
  fluentiter::FluentIter iter(items.begin(), items.end(), items.size());

  CHECK_EQ(1, *iter.next());
  CHECK_EQ(2, *iter.next());
  CHECK_EQ(3, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("map") {
  auto mapper = [](auto num) { return num + 1; };
  std::vector<int> items{1, 3, 5};
  auto iter = fluentiter::FluentIter(items.begin(), items.end(), items.size()).map(mapper);

  CHECK_EQ(2, *iter.next());
  CHECK_EQ(4, *iter.next());
  CHECK_EQ(6, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("filter") {
  auto predicate = [](auto num) { return num % 2 == 0; };
  std::vector<int> items{1, 2, 3, 4};
  auto iter = fluentiter::FluentIter(items.begin(), items.end(), items.size()).filter(predicate);

  CHECK_EQ(2, *iter.next());
  CHECK_EQ(4, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("zip") {
  std::vector<int> nums{1, 2, 3};
  std::vector<char> chars{'a', 'b', 'c'};

  fluentiter::FluentIter iter1(nums.begin(), nums.end(), nums.size());
  fluentiter::FluentIter iter2(chars.begin(), chars.end(), chars.size());

  auto iter = iter1.zip<decltype(iter2), char>(std::move(iter2));

  CHECK_EQ(std::make_pair(1, 'a'), *iter.next());
  CHECK_EQ(std::make_pair(2, 'b'), *iter.next());
  CHECK_EQ(std::make_pair(3, 'c'), *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("reduce") {
  auto reducer = [](auto acc, auto num) { return acc + num; };

  std::vector<int> items{1, 2, 3, 4};
  auto sum = fluentiter::FluentIter(items.begin(), items.end(), items.size()).reduce(reducer, 0);

  CHECK_EQ(sum, 1 + 2 + 3 + 4);
}

TEST_CASE("collect") {
  auto mapper = [](auto num) { return num + 1; };
  auto predicate = [](auto num) { return num % 2 == 0; };

  std::vector<int> items{1, 2, 3, 4};
  auto result = fluentiter::FluentIter(items.begin(), items.end(), items.size())
                    .map(mapper)
                    .filter(predicate)
                    .collect<fluentiter::VectorCollector<int>, std::vector<int>>();

  CHECK_EQ(result, std::vector<int>{2, 4});
}
