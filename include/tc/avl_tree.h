#pragma once

#ifndef DENIS_BU_AVL_TREE_H
#define DENIS_BU_AVL_TREE_H

#include <functional>
#include <utility>
#include <queue>
#include <cassert>

namespace db
{

template<typename T>
struct node 
{
	node* parent;
	node* left;
	node* right;
	signed char balance;
	T value;

	node*& link(signed char b) {
		return b == -1 ? left : right;
	}

	node(node* p, const T& v) : parent(p), left(nullptr), right(nullptr), balance(0), value(v)
	{ }
};

template<typename T, typename Comp = std::less<T>>
class avl_tree
{
public:
	using size_type = std::size_t;
	using value_type = T;

	avl_tree() : root(nullptr), size_(0u)
	{ }

	size_type size() const
	{ return size_; }

	void insert(const T& value);
	
	template<typename Preo, typename Ino, typename Posto>
	void traverse(Preo pre, Ino in, Posto post) const;

	template<typename A>
	void level_traverse(A callback) const;

private:
	Comp comp;
	node<T>* root;
	size_type size_;
};

template<typename T>
void assignParent(node<T>* n, node<T>* p) {
	if (n != nullptr) n->parent = p;
}

template<typename T, typename Comp>
inline void avl_tree<T, Comp>::insert(const T& v) {
	if (root == nullptr) {
		root = new node<T>(nullptr, v);
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
		goLeft = (comp(v, cur->value));
		if (goLeft) {
			cur = cur->left;
		} else {
			bool goRight = (comp(cur->value, v));
			if (goRight) {
				cur = cur->right;
			} else {
				cur->value = v;
				return;
			}
		}
	}
	auto& childptr = goLeft ? parent->left : parent->right;
	childptr = new node<T>(parent, v);
	++size_;
	// adjust balance
	goLeft = (comp(v, rebalance->value));
	signed char a = goLeft ? -1 : 1; // left heavy or right heavy
	auto r = goLeft ? rebalance->left : rebalance->right; // child of rebalance node
	auto p = r; // runner
	while (p != childptr) {
		assert(p->balance == 0);
		goLeft = (comp(v, p->value));
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
	node<T>* stub = nullptr;
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

		r->balance = newr->balance == -1 ? 1 : 0;
		rebalance->balance = newr->balance == 1 ? 1 : 0;
		newr->balance = 0;

		parent_link = newr;
		newr->parent = parent_ptr;
		if (change_root)
			root = newr;
		return;
	}

}


template<typename T, typename Comp>
template<typename Preo, typename Ino, typename Posto>
inline void avl_tree<T, Comp>::traverse(Preo pre, Ino in, Posto post) const {
	size_type level = 1u;
	auto cur = root;
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

template<typename T, typename Comp>
template<typename A>
inline void avl_tree<T, Comp>::level_traverse(A callback) const {
	std::queue<std::pair<node<T>*, size_type>> q;
	q.push(make_pair(root, 1u));
	while (!q.empty()) {
		auto f = q.front(); q.pop();
		auto n = f.first;
		auto level = f.second;
		callback(n->value, level, n->balance);
		if (n->left != nullptr) {
			q.push(make_pair(n->left, level + 1u));
		}
		if (n->right != nullptr) {
			q.push(make_pair(n->right, level + 1u));
		}
	}
}

}

#endif
