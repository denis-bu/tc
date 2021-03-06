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

  tnode* parent;
  tnode* left;
  tnode* right;
  int key;

  tnode(tnode* l, tnode* r, int v) : parent(nullptr), left(l), right(r), key(v)
  { }
  tnode(int v) : parent(nullptr), left(nullptr), right(nullptr), key(v)
  { }
};

struct ttree
{
  using node_type = tnode;
  tnode* root;
  unsigned size_;
  const tnode* croot() const
  { return root; }
  unsigned size() const {
    return size_;
  }
};

tnode* mnode(int v)
{ return new tnode { v }; }

tnode* mnode(tnode* l, tnode* r, int v)
{
  auto node = new tnode { l, r, v };
  if (l)
    l->parent = node;
  if (r)
    r->parent = node;
  return node;
}

}

// is_bast test cases

TEST(tree_test, trivial_trees_are_bst)
{
  // Empty tree is avl tree
  ASSERT_TRUE(tc::is_bst(ttree{nullptr}));
  // Tree with 1 or 2 nodes is avl tree
  ASSERT_TRUE(tc::is_bst(ttree{mnode(109)}));
  ASSERT_TRUE(tc::is_bst(ttree{mnode(mnode(5), nullptr, 9)}));
}

TEST(tree_test, test_is_bst)
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

TEST(tree_test, test_is_bst_tricky)
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
      ), 1
    };
    ASSERT_TRUE(tc::is_bst(subj));
    std::cout << "Yo\n";
    //tc::print_tree(subj, std::cout);
}


TEST(tree_test, test_is_not_bst)
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

TEST(tree_test, test_is_not_bst_duplicate)
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


TEST(tree_test, test_is_not_bst_tricky)
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

TEST(tree_test, trivial_trees_are_avl_balanced)
{
  // Empty tree is avl tree
  ASSERT_TRUE(tc::is_avl_balanced_tree(ttree{nullptr}));
  // Tree with 1 or 2 nodes is avl tree
  ASSERT_TRUE(tc::is_avl_balanced_tree(ttree{mnode(109)}));
  ASSERT_TRUE(tc::is_avl_balanced_tree(ttree{mnode(mnode(5), nullptr, 9)}));
}

TEST(tree_test, balanced_tree_is_avl_balanced)
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
  ASSERT_TRUE(tc::is_avl_balanced_tree(subj));
  ASSERT_FALSE(tc::is_bst(subj));
}

TEST(tree_test, almost_balanced_tree_is_avl_balanced)
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
  ASSERT_TRUE(tc::is_avl_balanced_tree(subj));
  ASSERT_FALSE(tc::is_bst(subj));
}

TEST(tree_test, list_of_three_nodes_is_not_avl_balanced)
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
  ASSERT_FALSE(tc::is_avl_balanced_tree(subj));
}

TEST(tree_test, test_right_heavy_tree_is_not_avl_balanced)
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
  ASSERT_FALSE(tc::is_avl_balanced_tree(subj));
}


TEST(tree_test, test_left_heavy_tree_is_not_avl_balanced)
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
  ASSERT_FALSE(tc::is_avl_balanced_tree(subj));
}
