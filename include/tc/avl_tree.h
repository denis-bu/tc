#pragma once

#ifndef TC_AVL_TREE_H
#define TC_AVL_TREE_H

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
		using balance_type = typename node_type::balance_type;

		static const balance_type LH = node_type::LH; // Left heavy.
		static const balance_type RH = node_type::RH; // Right heavy.

		avl_tree() : _root(nullptr), _size(0u)
		{ }

		size_type size() const
		{ return _size; }

		void insert(const T& value);

		void erase(const T& key);

		const node_type* croot() const
		{ return _root; }

	private:
		Comp _comp;
		avl_node<T>* _root;
		size_type _size;
	};

	template<typename T>
	void assignParent(avl_node<T>* n, avl_node<T>* p) {
		if (n != nullptr) n->parent = p;
	}

	template<typename T, typename Comp>
	void avl_tree<T, Comp>::insert(const T& v) {
		if (_root == nullptr) {
			_root = new avl_node<T>(nullptr, v);
			_size = 1;
			return;
		}

		auto rebalance = _root;
		auto parent = _root->parent;
		auto cur = _root;
		bool goLeft;
		while (cur) {
			if (cur->balance != 0)
				rebalance = cur;
			parent = cur;
			goLeft = _comp(v, cur->key);
			if (goLeft) {
				cur = cur->left;
			} else {
				bool goRight = _comp(cur->key, v);
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
		++_size;
		// adjust balance
		goLeft = (_comp(v, rebalance->key));
		balance_type a = goLeft ? LH : RH; // left heavy or right heavy
		auto r = goLeft ? rebalance->left : rebalance->right; // child of rebalance node
		auto p = r; // runner
		while (p != childptr) {
			assert(p->balance == 0);
			goLeft = (_comp(v, p->key));
			p->balance = goLeft ? LH : RH;
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
				_root = r;
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
				_root = newr;
			return;
		}

	}

	template<typename T, typename Comp>
	void avl_tree<T, Comp>::erase(const T& key) {
		if (_root == nullptr)
			return;

		auto cur = _root;
		while (cur != nullptr)
		{
			bool goLeft = _comp(key, cur->key);
			if (goLeft)
				cur = cur->left;
			else
			{
				bool goRight = _comp(cur->key, key);
				if (goRight)
					cur = cur->right;
				else
					break;
			}

		}
		if (!cur)
			return; // nothing to erase here.

		node_ptr replace = nullptr;
		if (!cur->left)
			assignParent(replace = cur->right, cur->parent);
		else if (!cur->right)
			assignParent(replace = cur->left, cur->parent);
		else
		{
			//replace cur with biggest on the left
			auto it = cur->left;
			while (it->right) it = it->right;
			if (it != cur->left)
			{
				assignParent(it->left, it->parent);// ???fix
				assignParent(cur->left, it);
				it->left = cur->left;
			}
			assignParent(cur->right, it);
			it->right = cur->right;
			assignParent(it, cur->parent);// should be the last one (to handle the case when it->parent is cur)
			// todo: dry below
			avl_node<T>* stub = nullptr;
			auto& parent_link = cur->parent
					? ((cur->parent->left == cur)
						 ? cur->parent->left
						 : cur->parent->right)
					: stub;
			parent_link = it;
			replace = it;
		}
		if (_root == cur)
			_root = replace;

		--_size;
		delete cur;

	}

}

#endif
