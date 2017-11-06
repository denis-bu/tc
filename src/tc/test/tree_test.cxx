#include "tc/avl_tree.h"
#include "tc/tree.h"

#include <gtest/gtest.h>
#include <vector>
#include <iostream>

namespace {

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

TEST(test_is_bst, tree_test)
{
  tc::avl_tree<int> subj {};
  subj.insert(1);
  subj.insert(10);
  subj.insert(5);
  subj.insert(-100);
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
