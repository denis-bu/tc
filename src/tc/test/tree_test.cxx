#include "tc/avl_tree.h"
#include "tc/tree.h"

#include <gtest/gtest.h>
#include <vector>
#include <iostream>

namespace
{

struct tnode
{
  using value_type = int;

  tnode* left;
  tnode* right;
  int value;

  tnode(tnode* l, tnode* r, int v) : left(l), right(r), value(v)
  { }
  tnode(int v) : left(nullptr), right(nullptr), value(v)
  { }
};

struct ttree
{
  using node_type = tnode;
  tnode* root;
  const tnode* croot() const
  { return root; }
};

tnode* mnode(int v)
{ return new tnode { v }; }

tnode* mnode(tnode* l, tnode* r, int v)
{ return new tnode { l, r, v }; }

}

// is_bast test cases

TEST(trivial_trees_are_bst, tree_test)
{
  // Empty tree is avl tree
  ASSERT_TRUE(tc::is_bst(ttree{nullptr}));
  // Tree with 1 or 2 nodes is avl tree
  ASSERT_TRUE(tc::is_bst(ttree{mnode(109)}));
  ASSERT_TRUE(tc::is_bst(ttree{mnode(mnode(5), nullptr, 9)}));
}

TEST(test_is_bst, tree_test)
{
  ttree subj = {
    mnode(
      mnode(-5),
      mnode(2),
      1
    )
  };
  ASSERT_TRUE(tc::is_bst(subj));

  subj = {
      mnode(
        nullptr,
        mnode(
          nullptr,
          mnode(99),
          2
        ),
        1
      )
    };
    ASSERT_TRUE(tc::is_bst(subj));
}

TEST(test_is_bst_tricky, tree_test)
{
  ttree subj = {
      mnode(
        mnode(0),
        mnode(
          nullptr,
          mnode(
           mnode(7),
           mnode(100),
           99
          ),
          2
        ),
        1
      )
    };
    ASSERT_TRUE(tc::is_bst(subj));
}


TEST(test_is_not_bst, tree_test)
{
  ttree subj = {
    mnode(
      mnode(3),
      mnode(2),
      1
    )
  };
  ASSERT_FALSE(tc::is_bst(subj));
}

TEST(test_is_not_bst_duplicate, tree_test)
{
  ttree subj = {
    mnode(
      mnode(0),
      mnode(1),
      1
    )
  };
  ASSERT_FALSE(tc::is_bst(subj));
}


TEST(test_is_not_bst_tricky, tree_test)
{
  ttree subj = {
    mnode(
      mnode(0),
      mnode(
        mnode(
          nullptr,
          mnode(7), // 7 violates bst property, since it's not in (3,4)
          3),
        mnode(5),
        4
      ),
      1
    )
  };
  ASSERT_FALSE(tc::is_bst(subj));
  subj = {
      mnode(
        mnode(-1),
        mnode(
          mnode(
            mnode(0), // 0 violates bst property, since it's not in (1, 3)
            nullptr,
            3),
          mnode(5),
          4
        ),
        1
      )
    };
    ASSERT_FALSE(tc::is_bst(subj));
}

// avl balance test cases

TEST(trivial_trees_are_avl_balanced, tree_test)
{
  // Empty tree is avl tree
  ASSERT_TRUE(tc::is_avl_tree(ttree{nullptr}));
  // Tree with 1 or 2 nodes is avl tree
  ASSERT_TRUE(tc::is_avl_tree(ttree{mnode(109)}));
  ASSERT_TRUE(tc::is_avl_tree(ttree{mnode(mnode(5), nullptr, 9)}));
}

TEST(balanced_tree_is_avl_balanced, tree_test)
{
  ttree subj = {
    mnode(
      mnode(
        mnode(5),
        mnode(2),
        3
      ),
      mnode(
        mnode(11),
        mnode(22),
      -7
      ),
      0
    )
  };
  ASSERT_TRUE(tc::is_avl_tree(subj));
  ASSERT_FALSE(tc::is_bst(subj));
}

TEST(almost_balanced_tree_is_avl_balanced, tree_test)
{
  ttree subj = {
    mnode(
      mnode(
        nullptr,
        mnode(2),
        3
      ),
      mnode(
        mnode(11),
        nullptr,
      -7
      ),
      0
    )
  };
  ASSERT_TRUE(tc::is_avl_tree(subj));
  ASSERT_FALSE(tc::is_bst(subj));
}

TEST(list_of_three_nodes_is_not_avl_balanced, tree_test)
{

  ttree subj = {
    mnode(
      mnode(
        mnode(5),
        nullptr,
        3
      ),
      nullptr,
      1
    )
  };
  ASSERT_FALSE(tc::is_avl_tree(subj));
}

TEST(test_right_heavy_tree_is_not_avl_balanced, tree_test)
{
  ttree subj = {
    mnode(
      mnode(3),
      mnode(
        mnode(
          mnode(7),
          nullptr,
          11
         ),
        nullptr,
      -7
      ),
      0
    )
  };
  ASSERT_FALSE(tc::is_avl_tree(subj));
}


TEST(test_left_heavy_tree_is_not_avl_balanced, tree_test)
{
  ttree subj = {
    mnode(
      mnode(
        mnode(
          mnode(7),
          nullptr,
          11
         ),
        nullptr,
        -7
      ),
      mnode(3),
      0
    )
  };
  ASSERT_FALSE(tc::is_avl_tree(subj));
}
