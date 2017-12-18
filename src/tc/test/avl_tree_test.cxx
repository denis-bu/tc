#include "tc/avl_tree.h"
#include "tc/tree.h"

#include <gtest/gtest.h>

#include <chrono>
#include <vector>
#include <iostream>

#include <cstdlib>

TEST(avl_tree_test, test_insert)
{
  tc::avl_tree<int> subj {};
  EXPECT_EQ(0, subj.size());

  subj.insert(1);
  subj.insert(2);
  subj.insert(3);
  subj.insert(4);
  subj.insert(5);

  EXPECT_EQ(5, subj.size());

  {
      std::vector<int> val_trace;
      std::vector<unsigned> lvl_trace;
      tc::inorder_traverse(subj, [&](int v, unsigned level) {
          val_trace.push_back(v);
          lvl_trace.push_back(level);
      });

      EXPECT_EQ(std::vector<int>({1, 2, 3, 4, 5}), val_trace);
      EXPECT_EQ(std::vector<unsigned>({2, 1, 3, 2, 3}), lvl_trace);
  }
  {
      std::vector<int> val_trace;
      std::vector<unsigned> lvl_trace;
      tc::level_order_traverse(subj, [&](int v, unsigned level) {
          val_trace.push_back(v);
          lvl_trace.push_back(level);
      });

      EXPECT_EQ(std::vector<int>({2, 1, 4, 3, 5}), val_trace);
      EXPECT_EQ(std::vector<unsigned>({1, 2, 2, 3, 3}), lvl_trace);
  }
}

TEST(avl_tree_test, test_size)
{
  tc::avl_tree<int> subj {};
  EXPECT_EQ(0, subj.size());
}

TEST(avl_tree_test, test_monotonic_sequence)
{
  tc::avl_tree<int> subj {};
  tc::avl_tree<int> subjr {};

  for (int i = 0; i < 1000; ++i) {
    int x = i-500;
    subj.insert(x);
    subjr.insert(- x);
    ASSERT_TRUE(tc::is_avl_tree(subj));
    ASSERT_TRUE(tc::is_avl_tree(subjr));
  }
}

TEST(avl_tree_test, test_random_sequence)
{
  using namespace std::chrono;
  auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  std::srand(ms);

  tc::avl_tree<int> subj {};

  for (int i = 0; i < 1777; ++i) {
    int x = rand();
    std::cout << x << ", ";
    subj.insert(x);
    ASSERT_TRUE(tc::is_avl_tree(subj));
  }
  //tc::print_tree(subj, std::cout);
}
