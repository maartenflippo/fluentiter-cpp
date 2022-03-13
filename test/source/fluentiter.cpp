#include <doctest/doctest.h>
#include <fluentiter/fluentiter.h>

#include <vector>

TEST_CASE("FluentIter yields the items in an input iterator") {
  std::vector<int> items{1, 2, 3};
  auto iter = fluentiter::from(items.begin(), items.end());

  CHECK_EQ(1, *iter.next());
  CHECK_EQ(2, *iter.next());
  CHECK_EQ(3, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("map") {
  auto mapper = [](auto num) { return num + 1; };
  std::vector<int> items{1, 3, 5};
  auto iter = fluentiter::from(items.begin(), items.end()).map(mapper);

  CHECK_EQ(2, *iter.next());
  CHECK_EQ(4, *iter.next());
  CHECK_EQ(6, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("filter") {
  auto predicate = [](auto num) { return num % 2 == 0; };
  std::vector<int> items{1, 2, 3, 4};
  auto iter = fluentiter::from(items.begin(), items.end()).filter(predicate);

  CHECK_EQ(2, *iter.next());
  CHECK_EQ(4, *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("zip") {
  std::vector<int> nums{1, 2, 3};
  std::vector<char> chars{'a', 'b', 'c'};

  auto iter1 = fluentiter::from(nums.begin(), nums.end());
  auto iter2 = fluentiter::from(chars.begin(), chars.end());

  auto iter = iter1.zip<decltype(iter2), char>(std::move(iter2));

  CHECK_EQ(std::make_pair(1, 'a'), *iter.next());
  CHECK_EQ(std::make_pair(2, 'b'), *iter.next());
  CHECK_EQ(std::make_pair(3, 'c'), *iter.next());
  CHECK_FALSE(iter.next().has_value());
}

TEST_CASE("reduce") {
  auto reducer = [](auto acc, auto num) { return acc + num; };

  std::vector<int> items{1, 2, 3, 4};
  auto sum = fluentiter::from(items.begin(), items.end()).reduce(reducer, 0);

  CHECK_EQ(sum, 1 + 2 + 3 + 4);
}

TEST_CASE("collect") {
  auto mapper = [](auto num) { return num + 1; };
  auto predicate = [](auto num) { return num % 2 == 0; };

  std::vector<int> items{1, 2, 3, 4};
  auto result = fluentiter::from(items.begin(), items.end())
                    .map(mapper)
                    .filter(predicate)
                    .collect<fluentiter::VectorCollector<int>, std::vector<int>>();

  CHECK_EQ(result, std::vector<int>{2, 4});
}
