#ifndef _BST_H
#define _BST_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>


template <typename T>
class TreeNode
{
public:
	T element;
	std::unique_ptr<TreeNode<T>> left;
	std::unique_ptr<TreeNode<T>> right;

	TreeNode<T>(const T& e)
		:element{ e }, left{ nullptr }, right{ nullptr } {}

	~TreeNode() {}

};


template <typename T>
struct BST
{
public:
	std::unique_ptr<TreeNode<T>> root = nullptr;

	~BST() {}

	bool insert(const T& key);
	bool search(const T& key);
	bool remove(const T& key);

private:
	bool insert(std::unique_ptr<TreeNode<T>>& t, const T& key);
	bool search(std::unique_ptr<TreeNode<T>>& t, const T& key);
	bool remove(std::unique_ptr<TreeNode<T>>& t, const T& key);

};

template <typename T>
bool BST<T>::insert(const T& key) {
	return insert(root, key);
}

template <typename T>
bool BST<T>::search(const T& key) {
	return search(root, key);
}

template <typename T>
bool BST<T>::remove(const T& key) {
	return remove(root, key);
}

template <typename T>
bool BST<T>::insert(std::unique_ptr<TreeNode<T>>& t, const T& key) {
	if (search(t, key)) {
		return false;
	}
	else {
		std::unique_ptr<TreeNode<T>> newnode = std::make_unique<TreeNode<T>>(key);
		if (t == nullptr) {
			t = std::move(newnode);
		}
		else {
			TreeNode<T>* parent = t.get();
			TreeNode<T>* schnode = t.get();
			while (schnode != nullptr) {
				parent = schnode;
				if (key > parent->element)
					schnode = schnode->right.get();
				else
					schnode = schnode->left.get();
			}

			if (key > parent->element)
				parent->right = std::move(newnode);
			else
				parent->left = std::move(newnode);
		}
		return true;
	}
}

template <typename T>
bool BST<T>::search(std::unique_ptr<TreeNode<T>>& t, const T& key) {
	TreeNode<T>* schnode = t.get();
	while (schnode != nullptr) {
		if (key == schnode->element)
			return true;
		else if (key > schnode->element)
			schnode = schnode->right.get();
		else
			schnode = schnode->left.get();
	}
	return false;
}

template <typename T>
bool BST<T>::remove(std::unique_ptr<TreeNode<T>>& t, const T& key) {
	if (!search(t, key)) {
		return false;
	}
	else {
		TreeNode<T>* parent = t.get();
		TreeNode<T>* schnode = t.get();
		int lr = 0;

		// searching
		while (schnode->element != key) {
			if (key > schnode->element) {
				parent = schnode;
				schnode = schnode->right.get();
				lr = 2;
			}
			else {
				parent = schnode;
				schnode = schnode->left.get();
				lr = 1;
			}
		}
		// schnode->elem == key, parent == schnode's parent
		// lr=0 : parent==schnode==key
		// lr=1 : parent->left == schnode
		// lr=2 : parent->right == schnode

		// keynode is leaf
		if (schnode->left.get() == nullptr && schnode->right.get() == nullptr) {
			if (lr == 2)
				parent->right = nullptr;
			else if (lr == 1)
				parent->left = nullptr;
			else
				t = nullptr;
			return true;
		}

		// keynode has one child at right
		else if (schnode->left.get() == nullptr) {
			if (lr == 2)
				parent->right = std::move(schnode->right);
			else if (lr == 1)
				parent->left = std::move(schnode->right);
			else
				t = std::move(schnode->right);
			return true;
		}

		// keynode has one child at left
		else if (schnode->right.get() == nullptr) {
			if (lr == 2)
				parent->right = std::move(schnode->left);
			else if (lr == 1)
				parent->left = std::move(schnode->left);
			else
				t = std::move(schnode->left);
			return true;
		}

		//keynode has two child
		else {
			TreeNode<T>* maxsch = schnode->left.get();
			TreeNode<T>* maxpar = maxsch;
			std::unique_ptr<TreeNode<T>> tempptr = nullptr;

			while (maxsch->right.get() != nullptr) {
				maxpar = maxsch;
				maxsch = maxsch->right.get();
			}

			// maxsch->right = null ptr && should consider maxsch->left if exists
			// maxpar->right = maxsch
			// *** should consider when schnode->left == maxpar == maxsch

			if (lr == 2) {
				if (maxpar == maxsch) {
					maxsch->right = std::move(schnode->right);
					parent->right = std::move(schnode->left);
				}
				else {
					if (maxsch->left != nullptr)
						tempptr = std::move(maxsch->left);

					maxsch->left = std::move(schnode->left);
					maxsch->right = std::move(schnode->right);
					parent->right = std::move(maxpar->right);

					if (tempptr != nullptr)
						maxpar->right = std::move(tempptr);
				}
			}
			else if (lr == 1) {
				if (maxpar == maxsch) {
					maxsch->right = std::move(schnode->right);
					parent->left = std::move(schnode->left);
				}
				else {
					if (maxsch->left != nullptr)
						tempptr = std::move(maxsch->left);

					maxsch->left = std::move(schnode->left);
					maxsch->right = std::move(schnode->right);
					parent->left = std::move(maxpar->right);

					if (tempptr != nullptr)
						maxpar->right = std::move(tempptr);
				}
			}
			else {
				if (maxpar == maxsch) {
					maxsch->right = std::move(schnode->right);
					t = std::move(schnode->left);
				}
				else {
					if (maxsch->left != nullptr)
						tempptr = std::move(maxsch->left);

					maxsch->left = std::move(parent->left);
					maxsch->right = std::move(parent->right);
					t = std::move(maxpar->right);

					if (tempptr != nullptr)
						maxpar->right = std::move(tempptr);
				}
			}
			return true;
		}
	}
}


#endif // _BST_H