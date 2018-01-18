#pragma once

#ifndef TC_BST_ITERATOR_H
#define TC_BST_ITERATOR_H

namespace tc
{

	template<class Node>
	class bst_iterator
	{

		using node_type = Node;
		using node_ptr = node_type*;

		using _iter_type = bst_iterator<node_type>;

		_iter_type left() const;
		_iter_type right();
		_iter_type parent();

		_iter_type& move_left();
		_iter_type& move_right();
		_iter_type& move_up();

	private:
		node_ptr _node;
	};

}

#endif
