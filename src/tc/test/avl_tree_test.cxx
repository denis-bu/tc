#include "tc/avl_tree.h"
#include "tc/tree.h"

#include <gtest/gtest.h>
#include <vector>
#include <iostream>

TEST(test_insert, avl_tree_test)
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

TEST(test_size, avl_tree_test)
{
  tc::avl_tree<int> subj {};
  EXPECT_EQ(0, subj.size());
}
