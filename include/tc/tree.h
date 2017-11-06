#pragma once

#ifndef DENIS_BU_TREE_H
#define DENIS_BU_TREE_H

#include <functional>
#include <utility>
#include <queue>
#include <cassert>

namespace tc
{

template<class Node>
struct node_traits
{
  using value_type = typename Node::value_type;
  // todo: fix constness
  static const Node * left(const Node* n)
  { return n->left; }
  static const Node * right(const Node* n)
  { return n->right; }
  static const value_type& value(const Node* n)
  { return n->value; }
  //todo: fix min,max
  static value_type min()
  { return std::numeric_limits<value_type>::min(); }
  static value_type max()
  { return std::numeric_limits<value_type>::max(); }

};
// todo: use iterative solution

template<class Node>
bool is_bst_helper(const Node * root, const typename Node::value_type& left_bound, const typename Node::value_type& right_bound) {
  if (root == nullptr)
    return true;
  using nt = node_traits<Node>;
  const auto& val = nt::value(root);
  if (val <= left_bound || val >= right_bound)
    return false;
  return is_bst_helper(nt::left(root), left_bound, val)
      && is_bst_helper(nt::right(root), val, right_bound);
}

template<class Tree>
bool is_bst(const Tree& tree) {
  auto root = tree.croot();
  if (root == nullptr)
    return true;
  using nt = node_traits<typename Tree::node_type>;
  return is_bst_helper(nt::left(root), nt::min(), nt::value(root))
      && is_bst_helper(nt::right(root), nt::value(root), nt::max());
}


template<class Tree, typename PreO, typename InO, typename PostO>
void traverse(const Tree& tree, PreO pre, InO in, PostO post)
{
  auto cur = tree.croot();
  unsigned level = 1u;
  while (cur != nullptr) {
    pre(cur->value, level);
    if (cur->left != nullptr) {
      cur = cur->left;
      ++level;
    } else {
      while (cur != nullptr) {
        in(cur->value, level);
        if (cur->right != nullptr) {
          cur = cur->right;
          ++level;
          break;
        } else {
          post(cur->value, level);
          while (cur->parent != nullptr && cur->parent->left != cur) {
            cur = cur->parent;
            --level;
            post(cur->value, level);
          }
          cur = cur->parent;
          --level;

        }
      }
    }
  }
}

template<class Tree, typename InO>
void inorder_traverse(const Tree& tree, InO in)
{
  auto dummy = [](typename Tree::value_type, unsigned) {};
  traverse(tree, dummy, in, dummy);
}

template<class Tree, typename Callback>
void level_order_traverse(const Tree& tree, Callback callback)
{
  using Node = typename Tree::node_type;
  std::queue<std::pair<const Node*, unsigned>> q;
  q.push(std::make_pair(tree.croot(), 1u));
  while (!q.empty()) {
    auto f = q.front(); q.pop();
    auto n = f.first;
    auto level = f.second;
    callback(n->value, level);
    if (n->left != nullptr) {
      q.push(std::make_pair(n->left, level + 1u));
    }
    if (n->right != nullptr) {
      q.push(std::make_pair(n->right, level + 1u));
    }
  }
}

}

#endif
