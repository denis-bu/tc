#pragma once

#ifndef TC_TREE_H
#define TC_TREE_H

#include <functional>
#include <utility>
#include <queue>
#include <cmath>
#include <cassert>
#include <map>
#include <iostream>

namespace tc
{

template<class Node>
struct node_traits
{
  using value_type = typename Node::value_type;
  // todo: fix constness
  static const Node * parent(const Node* n)
  { return n->parent; }
  static const Node * left(const Node* n)
  { return n->left; }
  static const Node * right(const Node* n)
  { return n->right; }
  static const value_type& key(const Node* n)
  { return n->key; }
  //todo: remove in bst min,max
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
  const auto& key = nt::key(root);
  if (key <= left_bound || key >= right_bound)
    return false;
  return is_bst_helper(nt::left(root), left_bound, key)
      && is_bst_helper(nt::right(root), key, right_bound);
}

template<class Tree>
bool is_bst(const Tree& tree) {
  auto root = tree.croot();
  if (root == nullptr)
    return true;
  using nt = node_traits<typename Tree::node_type>;
  return is_bst_helper(nt::left(root), nt::min(), nt::key(root))
      && is_bst_helper(nt::right(root), nt::key(root), nt::max());
}

template<class Tree>
bool is_avl_balanced_tree(const Tree& tree)
{
  using node_type = typename Tree::node_type;
  using nt = node_traits<node_type>;
  const int BAD = -1;
  struct helper
  {
    // -1 not an avl tree, 0+ heights of a tree under root
    int verify(const node_type* root)
    {
      if (root == nullptr)
        return 0;
      auto lh = verify(nt::left(root));
      if (lh < 0)
        return BAD;
      auto rh = verify(nt::right(root));
      if (rh < 0)
        return BAD;
      if (std::abs(lh - rh) > 1)
        return BAD;
      return 1 + std::max(lh, rh);
    }
  };

  helper h;
  return h.verify(tree.croot()) != BAD;
}

template<class Tree>
bool is_avl_tree(const Tree& tree)
{
  return is_bst(tree) && is_avl_balanced_tree(tree);
}

template<class Tree, typename PreO, typename InO, typename PostO>
void traverse(const Tree& tree, PreO pre, InO in, PostO post)
{
  using node_type = typename Tree::node_type;
  using nt = node_traits<node_type>;

  auto cur = tree.croot();
  unsigned level = 1u;
  while (cur != nullptr) {
    pre(nt::key(cur), level);
    if (nt::left(cur) != nullptr) {
      cur = nt::left(cur);
      ++level;
    } else {
      while (cur != nullptr) {
        in(nt::key(cur), level);
        if (nt::right(cur) != nullptr) {
          cur = nt::right(cur);
          ++level;
          break;
        } else {
          post(nt::key(cur), level);
          auto parent = nt::parent(cur);
          while (parent != nullptr && nt::left(parent)!= cur) {
            cur = parent;
            --level;
            post(nt::key(cur), level);
            parent = nt::parent(cur);
          }
          cur = nt::parent(cur);
          --level;

        }
      }
    }
  }
}


template<class Tree, typename Callback>
void preorder_traverse(const Tree& tree, Callback cb)
{
  auto dummy = [](typename Tree::value_type, unsigned) {};
  traverse(tree, cb, dummy, dummy);
}

template<class Tree, typename Callback>
void inorder_traverse(const Tree& tree, Callback cb)
{
  auto dummy = [](typename Tree::value_type, unsigned) {};
  traverse(tree, dummy, cb, dummy);
}

template<class Tree, typename Callback>
void postorder_traverse(const Tree& tree, Callback cb)
{
  auto dummy = [](typename Tree::value_type, unsigned) {};
  traverse(tree, dummy, dummy, cb);
}

template<class Tree, typename Callback>
void level_order_traverse(const Tree& tree, Callback callback)
{
  using node_type = typename Tree::node_type;
  using nt = node_traits<node_type>;
  std::queue<std::pair<const node_type*, unsigned>> q;
  q.push(std::make_pair(tree.croot(), 1u));
  while (!q.empty()) {
    auto f = q.front(); q.pop();
    auto n = f.first;
    auto level = f.second;

    callback(nt::key(n), level);

    auto left = nt::left(n);
    if (left != nullptr) {
      q.push(std::make_pair(left, level + 1u));
    }
    auto right = nt::right(n);
    if (right != nullptr) {
      q.push(std::make_pair(right, level + 1u));
    }
  }
}

template<class Tree>
int tree_height(const Tree& tree)
{
  using node_type = typename Tree::node_type;
  using nt = node_traits<node_type>;
  struct helper
  {
    // -1 not an avl tree, 0+ heights of a tree under root
    int depth(const node_type* root)
    {
      if (root == nullptr)
        return 0;
      auto lh = depth(nt::left(root));
      auto rh = depth(nt::right(root));
      return 1 + std::max(lh, rh);
    }
  };

  helper h;
  return h.depth(tree.croot());
}

template<class Tree>
void print_tree(const Tree& tree, std::ostream& os)
{
  if (tree.size() == 0)
    return;

  const int total_levels = tree_height(tree) - 1;
  std::cout << "Total levels " << total_levels << std::endl;

  using node_type = typename Tree::node_type;
  using nt = node_traits<node_type>;
  std::queue<std::pair<const node_type*, unsigned>> q;
  q.push(std::make_pair(tree.croot(), 0u));
  unsigned cur_lev = 0;

  std::map<const node_type*, int> parents_pos;
  std::map<const node_type*, int> cur_lvl_pos;
  int print_pos = 0;
  while (!q.empty()) {
    auto f = q.front(); q.pop();
    auto n = f.first;
    auto level = f.second;
    if (level != cur_lev) {
      os << std::endl;
      cur_lev = level;
      parents_pos.swap(cur_lvl_pos);
      cur_lvl_pos.clear();
      print_pos = 0;
    }
    auto pn = nt::parent(n);
    auto it = parents_pos.find(pn);
    int position;
    if (it != parents_pos.end()) {
      int pnpos = it->second;
      position = 2 * pnpos + (nt::left(pn) == n ? 0 : 1);
    } else {
      position = 0;
    }
    cur_lvl_pos.insert(std::make_pair(n, position));
    // print
    const int num_chars_per_node = 11 + 1; // chars per int + 1
    const int block_size = (int)std::pow(2, total_levels-level) * num_chars_per_node;
    int start_print = position * block_size + (block_size - num_chars_per_node) / 2;

    auto val_str = std::to_string(nt::key(n)) + "(" + std::to_string(n->balance) + ")";
    int prefix_size = start_print - print_pos + num_chars_per_node - val_str.length();
    if (prefix_size <= 0)
      prefix_size = 1;
    os << std::string(prefix_size, ' ') << val_str;
    print_pos = start_print + num_chars_per_node;

    auto left = nt::left(n);
    if (left != nullptr) {
      q.push(std::make_pair(left, level + 1u));
    }
    auto right = nt::right(n);
    if (right != nullptr) {
      q.push(std::make_pair(right, level + 1u));
    }
  }
}

}

#endif
