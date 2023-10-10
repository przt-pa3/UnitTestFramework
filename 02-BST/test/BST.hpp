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

    // TODO
    // if insertion fails (i.e. if the key already exists in tree), return false
    // otherwise, return true

    return false;
}

template <typename T>
bool BST<T>::search(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if key exists in tree, return true
    // otherwise, return false

    return false;
}

template <typename T>
bool BST<T>::remove(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if key does not exist in tree, return false
    // otherwise, return true

    return false;
}


#endif // _BST_H
