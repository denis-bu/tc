#include "tc/avl_tree.h"

#include <gtest/gtest.h>

TEST(test_1, avl_tree_test)
{
  tc::avl_tree<int> subj {};
  EXPECT_EQ(0, subj.size());
  subj.insert(100500);
  EXPECT_EQ(1, subj.size());
}
