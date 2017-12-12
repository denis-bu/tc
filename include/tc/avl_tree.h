#pragma once

#ifndef DENIS_BU_AVL_TREE_H
#define DENIS_BU_AVL_TREE_H

#include <functional>
#include <utility>
#include <queue>
#include <cassert>

namespace tc
{

template<typename T>
struct avl_node
{
  using value_type = T;
  using balance_type = signed char;

  static const balance_type LH = -1;  // Left heavy.
  static const balance_type RH = -LH; // Right heavy.

  avl_node* parent;
  avl_node* left;
  avl_node* right;
  balance_type balance;
  T key;

  avl_node*& link(signed char b)
  { return b == LH ? left : right; }

  avl_node(avl_node* p, const T& k)
    : parent(p), left(nullptr), right(nullptr), balance(0), key(k)
  { }
};

template<typename T, typename Comp = std::less<T>>
class avl_tree
{
public:
	using size_type = std::size_t;
	using value_type = T;
  using node_type = avl_node<T>;
  using node_ptr = node_type*;
  using const_node_ptr = const node_ptr;

	avl_tree() : root(nullptr), size_(0u)
	{ }

	size_type size() const
	{ return size_; }

	void insert(const T& value);

    const node_type* croot() const
    { return root; }

private:
	Comp comp;
  avl_node<T>* root;
	size_type size_;
};

template<typename T>
void assignParent(avl_node<T>* n, avl_node<T>* p) {
	if (n != nullptr) n->parent = p;
}

template<typename T, typename Comp>
inline void avl_tree<T, Comp>::insert(const T& v) {
	if (root == nullptr) {
        root = new avl_node<T>(nullptr, v);
		size_ = 1;
		return;
	}

	auto rebalance = root;
	auto parent = root->parent;
	auto cur = root;
	bool goLeft;
	while (cur) {
		if (cur->balance != 0)
			rebalance = cur;
		parent = cur;
    goLeft = (comp(v, cur->key));
		if (goLeft) {
			cur = cur->left;
		} else {
      bool goRight = (comp(cur->key, v));
			if (goRight) {
				cur = cur->right;
			} else {
        //  update node's value
        cur->key = v;
				return;
			}
		}
	}

  auto& childptr = goLeft ? parent->left : parent->right;
  childptr = new avl_node<T>(parent, v);
	++size_;
	// adjust balance
  goLeft = (comp(v, rebalance->key));
	signed char a = goLeft ? -1 : 1; // left heavy or right heavy
	auto r = goLeft ? rebalance->left : rebalance->right; // child of rebalance node
	auto p = r; // runner
	while (p != childptr) {
		assert(p->balance == 0);
    goLeft = (comp(v, p->key));
		p->balance = goLeft ? -1 : 1;
		p = goLeft ? p->left : p->right;
	}

	if (rebalance->balance == 0) {
    rebalance->balance = a; // todo: +1 level here
		return;
	}

	if (rebalance->balance == -a) {
		rebalance->balance = 0; // got more balance
		return;
	}

	bool change_root = rebalance->parent == nullptr;
    avl_node<T>* stub = nullptr;
	auto& parent_link = rebalance->parent
		? ((rebalance->parent->left == rebalance)
			? rebalance->parent->left
			: rebalance->parent->right)
		: stub;
	auto parent_ptr = rebalance->parent;
	if (r->balance == a) {
		// single rotation
		rebalance->link(a) = r->link(-a);
		assignParent(rebalance->link(a), rebalance);
		r->link(-a) = rebalance;
		rebalance->parent = r;
		
		rebalance->balance = 0;
		r->balance = 0;

		parent_link = r;
		r->parent = parent_ptr;
		if (change_root)
			root = r;
		return;
	}

	if (r->balance == -a) {
		// double rotation
    auto newr = r->link(-a);
		r->link(-a) = newr->link(a);
		assignParent(r->link(-a), r);
		rebalance->link(a) = newr->link(-a);
		assignParent(rebalance->link(a), rebalance);
		newr->link(a) = r;
		assignParent(newr->link(a), newr);
		newr->link(-a) = rebalance;
		assignParent(newr->link(-a), newr);

    r->balance = newr->balance == -a ? a : 0;
    rebalance->balance = newr->balance == a ? -a : 0;
		newr->balance = 0;

		parent_link = newr;
		newr->parent = parent_ptr;
		if (change_root)
			root = newr;
		return;
	}

}

}

#endif
