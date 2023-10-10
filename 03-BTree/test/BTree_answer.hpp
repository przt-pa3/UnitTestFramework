#ifndef _BTREE_H
#define _BTREE_H

#pragma once
#include <cstddef>
#include <array>
#include <iostream>
#include <optional>
#include <iterator>
#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>
#include <functional>
using namespace std;
enum class NodeType { LEAF, INTERNAL };

template<typename T, size_t B = 6>
struct BTreeNode;

template<typename T, size_t B = 6>
struct BTree {
    BTreeNode<T, B>* root = nullptr;

    ~BTree() { if (root) delete root; }

    bool insert(const T&);
    bool remove(const T&);

    void for_all(std::function<void(T&)>);
    void for_all_nodes(std::function<void(const BTreeNode<T, B>&)>);

    const std::optional<T> find_rightmost_key() const;
    const std::optional<size_t> depth() const;

    std::string format(void);
};

template<typename T, size_t B>
struct BTreeNode {
    NodeType type;
    size_t n;
    std::array<T, 2 * B - 1> keys;
    std::array<BTreeNode*, 2 * B> edges;

    BTreeNode();
    BTreeNode(const T& t);
    BTreeNode(std::initializer_list<T>);

    template<typename InputIt>
    BTreeNode(InputIt begin, InputIt end);

    ~BTreeNode();

    bool insert(const T& t);
    size_t get_index(const T& t);

    void for_all(std::function<void(T&)> func);

    bool remove(const T& t);

    size_t depth(void);
    std::string format_subtree(size_t);
    std::string format_level(size_t);
    std::string format_node(void);
    std::vector<BTreeNode<T, B>*> find_nodes_at_level(size_t);

    void for_all_nodes(std::function<void(const BTreeNode&)>);

    static std::pair<BTreeNode*, size_t> search(BTreeNode<T, B>*, const T& t);
    static void split_child(BTreeNode<T, B>&, size_t);
    static bool try_borrow_from_sibling(BTreeNode<T, B>&, size_t);
    static bool borrow_from_right(BTreeNode<T, B>&, size_t);
    static bool borrow_from_left(BTreeNode<T, B>&, size_t);

    /* NOTE: If the root node has only one key, it will be empty after
      merging the children. Take care of updating the root. I guess this is
      the only way a B-tree may shrink its height. */
    static bool merge_children(BTreeNode<T, B>&, size_t);

    static T& find_rightmost_key(BTreeNode<T, B>&);
};

template<typename T, size_t B>
bool BTree<T, B>::insert(const T& t) {
    if (!root) {
        root = new BTreeNode<T, B>(t);
        return true;
    }

    /* Make sure the root node is not full. Create an empty tree which has
       the original root as a child. Then split the original root. */
    if (root->n >= 2 * B - 1) {
        BTreeNode<T, B>* new_root = new BTreeNode<T, B>{};
        new_root->edges[0] = root;
        BTreeNode<T, B>::split_child(*new_root, 0);
        root = new_root;
    }

    return root->insert(t);
}

/* By default, use in-order traversal */
template<typename T, size_t B>
void BTree<T, B>::for_all(std::function<void(T&)> func) {
    if (root)
        root->for_all(func);
}

/* This isn't necessarily the in-order traversal */
template<typename T, size_t B>
void BTree<T, B>::for_all_nodes(std::function<void(const BTreeNode<T, B>&)> func) {
    if (root)
        root->for_all_nodes(func);
}

template<typename T, size_t B>
const std::optional<T> BTree<T, B>::find_rightmost_key() const {
    if (!root)
        return std::nullopt;

    return BTreeNode<T, B>::find_rightmost_key(*root);
}

template<typename T, size_t B>
const std::optional<size_t> BTree<T, B>::depth() const {
    if (!root)
        return std::nullopt;

    return root->depth();
}

template<typename T, size_t B>
bool BTreeNode<T, B>::insert(const T& t) {

    // TODO

    BTreeNode<T, B>* pin = this;
    size_t index = 0;

    BTreeNode<T, B>* pinarray[1000] = {};
    size_t indexarray[1000] = {};

    index = pin->get_index(t);

    // for backtracking
    int k = 0;
    pinarray[0] = pin;
    indexarray[0] = index;

    while (1) {
        if (pin->type == NodeType::INTERNAL) {
            if (pin->edges[index]->n == 2 * B - 1) {
                BTreeNode<T, B>::split_child(*pin, index);

                // iterating index after split
                if (pin->keys[index] < t) {
                    index++;
                }
            }

            k++;
            pin = pin->edges[index];
            index = pin->get_index(t);
            pinarray[k] = pin;
            indexarray[k] = index;
        }
        else {
            // pin is leaf node
            // inserting node (this leaf is not full)

            // making room for t
            index = pin->get_index(t);

            for (auto i = 2 * B - 2; i > index; i--) {
                pin->keys[i] = pin->keys[i - 1];
                pin->edges[i + 1] = pin->edges[i];
            }
            // edges have one more object
            pin->edges[index + 1] = pin->edges[index];
            pin->keys[index] = t;
            pin->n++;
            /*
            if (pin->n == 2 * B - 1) {
                // k = 0
                if (k == 1) {
                    BTreeNode<T, B>::split_child(*pinarray[0], indexarray[0]);
                }

                while (k > 1) {
                    if (pinarray[k]->n == 2 * B - 1) {
                        BTreeNode<T, B>::split_child(*pinarray[k - 1], indexarray[k - 1]);
                        k--;
                    }
                    else {
                        //return true;
                        break;
                    }
                }

            }
            */
            // return true;
            break;
        }
    }

    while (k > 0) {
        if (pinarray[k]->n == 2 * B - 1) {
            BTreeNode<T, B>::split_child(*pinarray[k - 1], indexarray[k - 1]);
        }
        k--;
    }
    return true;


}

/**
 * Find the desired position of t in current node.
 *
 * For example, if `n` looks like the following:
 *
 * [ 3 | 9 | 13 | 27 ]
 *
 * Then,
 *     n.get_index(2) = 0
 *     n.get_index(5) = 1
 *     n.get_index(10) = 2
 *     n.get_index(19) = 3
 *     n.get_index(31) = 4
 */

template<typename T, size_t B>
size_t BTreeNode<T, B>::get_index(const T& t) {
    // TODO

    size_t iter = 0;
    for (auto i = 0; i < this->n; i++) {
        if (t > this->keys[i]) {
            iter = i + 1;
        }
        else {
            break;
        }
    }

    return iter;
}

template<typename T, size_t B>
void BTreeNode<T, B>::for_all(std::function<void(T&)> func) {
    if (type == NodeType::LEAF) {
        for (auto j = 0; j < n; j++)
            func(keys[j]);
    }
    else {
        if (n < 1)
            return;

        for (auto j = 0; j < n; j++) {
            edges[j]->for_all(func);
            func(keys[j]);
        }

        /* The rightest edge */
        edges[n]->for_all(func);
    }
}

/* This isn't necessarily the in-order traversal */
template<typename T, size_t B>
void BTreeNode<T, B>::for_all_nodes(std::function<void(const BTreeNode<T, B>&)> func) {
    if (type == NodeType::LEAF) {
        func(*this);
    }
    else {
        if (n < 1)
            return;

        func(*this);

        for (auto j = 0; j < n + 1; j++) {
            edges[j]->for_all_nodes(func);
        }
    }
}

/* Assume this is called only when the child parent->edges[idx] is full, and
   the parent is not full. */
template<typename T, size_t B>
void BTreeNode<T, B>::split_child(BTreeNode<T, B>& parent, size_t idx) {

    BTreeNode<T, B>* needSplit = parent.edges[idx];
    BTreeNode<T, B>* newNode = new BTreeNode<T, B>{};

    //newNode->edges[2 * B - 1] = nullptr;

    // modifying type

    newNode->type = needSplit->type;

    // moving

    newNode->n = B - 1;

    for (auto i = 0; i < B - 1; i++) {
        newNode->keys[i] = needSplit->keys[i + B];
    }

    if (needSplit->type == NodeType::INTERNAL) {
        for (auto i = 0; i < B; i++) {
            newNode->edges[i] = needSplit->edges[i + B];
            needSplit->edges[i + B] = nullptr;
        }
    }

    needSplit->n = B - 1;

    parent.type = NodeType::LEAF;
    parent.insert(needSplit->keys[B - 1]);
    parent.type = NodeType::INTERNAL;
    parent.edges[idx + 1] = newNode;
    // TODO
}

template<typename T, size_t B>
bool BTree<T, B>::remove(const T& t) {
    if (!root)
        return false;

    // MODIFIED -- START
    if (root->n >= 2 * B - 1) {
        BTreeNode<T, B>* new_root = new BTreeNode<T, B>{};
        new_root->edges[0] = root;
        BTreeNode<T, B>::split_child(*new_root, 0);
        root = new_root;
    }
    // MODIFIED -- END

    root->remove(t);

    /* After merging, the size of the root may become 0. */
    if (root->n == 0 && root->type == NodeType::INTERNAL) {
        auto prev_root = root;
        root = root->edges[0];
        prev_root->type = NodeType::LEAF;
        delete prev_root;
    }

    // MODIFIED -- START
    if (root->n >= 2 * B - 1) {
        BTreeNode<T, B>* new_root = new BTreeNode<T, B>{};
        new_root->edges[0] = root;
        BTreeNode<T, B>::split_child(*new_root, 0);
        root = new_root;
    }
    // MODIFIED -- END

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::remove(const T& t) {
    // TODO   
    BTreeNode<T, B>* pinarray[1000] = { nullptr };
    size_t indexarray[1000] = { 0 };

    pinarray[0] = this;
    indexarray[0] = this->get_index(t);

    // for backtrack
    int k = 0;

    BTreeNode<T, B>* currpin = pinarray[0];
    size_t currindex = indexarray[0];

    // found at root

    if (this->keys[this->get_index(t)] == t && this->get_index(t) < this->n
        && this->type == NodeType::LEAF) {
        for (size_t i = currindex + 1; i < currpin->n; ++i) {
            currpin->keys[i - 1] = currpin->keys[i];
        }
        currpin->n--;
        return true;

    }



    while (1) {

        if (currpin->keys[currindex] == t && currindex < currpin->n) {

            // t found at LEAF
            if (currpin->type == NodeType::LEAF) {

                // enough keys -> just delete
                if (currpin->n > B - 1) {
                    for (size_t i = currindex + 1; i < currpin->n; ++i) {
                        currpin->keys[i - 1] = currpin->keys[i];
                    }
                    currpin->n--;
                    return true;
                }

                // not enough keys -> delete and backtrack
                else {
                    for (size_t i = currindex + 1; i < currpin->n; ++i) {
                        currpin->keys[i - 1] = currpin->keys[i];
                    }
                    currpin->n--;

                    // pinarray[k-1]->edges[indexarray[k-1] = currpin = pinarray[k]
                    if (k > 0 && BTreeNode<T, B>::try_borrow_from_sibling(*pinarray[k - 1], indexarray[k - 1])) {
                        return true;
                    }
                    else {
                        // pinarray[k-1]->n = 0
                        while (k > 1) {
                            if (pinarray[k - 1]->n < B - 1) {
                                BTreeNode<T, B>* mergetemp = pinarray[k - 2];
                                if (indexarray[k - 2] != 0) {
                                    BTreeNode<T, B>::merge_children(*mergetemp, indexarray[k - 2] - 1);
                                }
                                else {
                                    BTreeNode<T, B>::merge_children(*mergetemp, indexarray[k - 2]);
                                }
                                k--;
                            }
                            else if (pinarray[k - 1]->n == 2 * B - 1) {
                                cout << "FAILURE" << endl;
                                k--;
                            }
                            else {
                                return true;
                            }
                        }
                        return true;
                    }
                }
            }

            // t found at INTERNAL
            else {
                T pred = BTreeNode<T, B>::find_rightmost_key(*currpin->edges[currindex]);
                this->remove(pred);
                BTreeNode<T, B>::search(this, t).first
                    ->keys[BTreeNode<T, B>::search(this, t).second] = pred;
                return true;
            }

        }

        // key not found
        else {
            if (currpin->type == NodeType::LEAF) {
                cout << "cannot find " << t << endl;
                cout << "Error" << endl;
                system("pause");
            }
            else {
                k++;
                currpin = currpin->edges[currindex];
                currindex = currpin->get_index(t);
                pinarray[k] = currpin;
                indexarray[k] = currindex;
                continue;
            }
        }
    }


    return true;
}

/**
 * Try to borrow a key from sibling.
 *
 * @e: The index of the edge that are trying to borrow a key
 * @return true if borrowing succeed, false otherwise
 */
template<typename T, size_t B>
bool BTreeNode<T, B>::try_borrow_from_sibling(BTreeNode<T, B>& node, size_t e) {
    // node = parent
    // node.edges[e] = child = who need fix (less than min # of keys)

    BTreeNode<T, B>* child = node.edges[e];

    if (BTreeNode<T, B>::borrow_from_left(node, e)) {
        return true;
    }
    else if (BTreeNode<T, B>::borrow_from_right(node, e)) {
        return true;
    }
    else {
        if (e != 0) {
            // merge node.edges[e-1] & node.edges[e]
            return BTreeNode<T, B>::merge_children(node, e - 1);
        }
        else {
            // merge node.edges[e] & node.edges[e+1]
            return BTreeNode<T, B>::merge_children(node, e);
        }
    }


}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_right(BTreeNode<T, B>& node, size_t edge) {
    // TODO

    // node = parent of pin (where key found)
    // node.edges[e] = pin (that are trying to borrowing)
    // node.edges[e - 1] => left sibling
    // node.edges[e + 1] => right sibling
    // only immediate sibling
    // borrow from sibling = only at LEAF

    // if right sibling exists

    if (edge < node.n) {
        BTreeNode<T, B>* donator = node.edges[edge + 1];
        BTreeNode<T, B>* recipient = node.edges[edge];
        if (donator->n > B - 1) {
            recipient->insert(node.keys[edge]);
            T temp = donator->keys[0];
            donator->remove(temp);
            node.keys[edge] = temp;
            return true;
        }
        return false;
    }
    else {
        return false;
    }
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_left(BTreeNode<T, B>& node, size_t edge) {
    // TODO

    // node = parent of pin (where key found)
    // node.edges[e] = pin (that are trying to borrowing)
    // node.edges[e - 1] => left sibling
    // node.edges[e + 1] => right sibling
    // only immediate sibling
    // borrow from sibling = only at LEAF

    // if left sibling exists
    if (edge > 0) {
        BTreeNode<T, B>* donator = node.edges[edge - 1];
        BTreeNode<T, B>* recipient = node.edges[edge];
        if (donator->n > B - 1) {
            // if left sibling node has enough key : can borrow from left sibling

            recipient->insert(node.keys[edge - 1]);
            T temp = donator->keys[donator->n - 1];
            donator->remove(temp);
            node.keys[edge - 1] = temp;
            return true;
        }
        return false;
    }
    else {
        return false;
    }
}

template<typename T, size_t B>
bool BTreeNode<T, B>::merge_children(BTreeNode<T, B>& node, size_t idx) {
    // TODO
    // node => parent of pin
    // node.edges[idx] = pin
    // BTreeNode<T, B>* pin = node.edges[idx];
    // merge ith child. i+1th child of node


    BTreeNode<T, B>* left = node.edges[idx];
    BTreeNode<T, B>* right = node.edges[idx + 1];



    // move parent to left
    left->keys[left->n] = node.keys[idx];
    left->n++;

    // delete parent's moved key { node.keys[i] }
    for (size_t i = idx + 1; i < node.n; ++i) {
        node.keys[i - 1] = node.keys[i];
        node.edges[i] = node.edges[i + 1];
    }
    node.n--;

    // left to right µµ °í·Á?
    // move right to left
    //if (node.n>=1) {
    for (size_t j = 0; j < right->n; j++) {
        left->keys[j + left->n] = right->keys[j];
        left->edges[j + left->n] = right->edges[j];
    }
    left->n += right->n;
    // move one more
    left->edges[left->n] = right->edges[right->n];
    //}

    right->type = NodeType::LEAF;
    delete right;

    if (left->n == 2 * B - 1) {
        BTreeNode<T, B>::split_child(node, idx);
    }

    if (node.n == 0) {
        //idk...
        return false;
    }

    return true;
}

template<typename T, size_t B>
T& BTreeNode<T, B>::find_rightmost_key(BTreeNode<T, B>& node) {
    if (node.type == NodeType::LEAF)
        return node.keys[node.n - 1];

    return find_rightmost_key(*node.edges[node.n]);
}

template<typename T, size_t B>
std::pair<BTreeNode<T, B>*, size_t>
BTreeNode<T, B>::search(BTreeNode<T, B>* node, const T& t) {
    if (node->type == NodeType::LEAF) {
        for (auto i = 0; i < node->keys.size(); i++)
            if (t == node->keys[i])
                return { node, i };

        return { nullptr, -1 };
    }

    size_t i;
    for (i = 0; i < node->n; i++) {
        if (t == node->keys[i])
            return { node, i };

        if (t < node->keys[i]) {
            return search(node->edges[i], t);
        }
    }

    return search(node->edges[i], t);
}

template<typename T, size_t B>
size_t BTreeNode<T, B>::depth(void) {
    if (type == NodeType::LEAF)
        return 0;

    return 1 + edges[0]->depth();
}

template <typename T, size_t B>
std::string BTree<T, B>::format(void) {
    if (!root)
        return std::string{};
    return root->format_subtree(root->depth());
}

template<typename T, size_t B>
std::string BTreeNode<T, B>::format_subtree(size_t depth) {
    std::ostringstream os;

    for (auto i = 0; i <= depth; i++)
        os << format_level(i) << '\n';

    return os.str();
}

template<typename T, size_t B>
std::string BTreeNode<T, B>::format_level(size_t level) {
    std::ostringstream os;
    auto nodes_at_level = find_nodes_at_level(level);

    for (auto node : nodes_at_level)
        os << node->format_node() << ' ';

    return os.str();
}


template<typename T, size_t B>
std::string BTreeNode<T, B>::format_node(void) {
    std::ostringstream os;

    os << '[';
    for (auto i = 0; i < n - 1; i++)
        os << keys[i] << '|';
    os << keys[n - 1];
    os << ']';

    return os.str();
}

template<typename T, size_t B>
std::vector<BTreeNode<T, B>*> BTreeNode<T, B>::find_nodes_at_level(size_t lv) {
    std::vector<BTreeNode<T, B>*> nodes;

    if (lv == 0) {
        nodes.emplace_back(this);
        return nodes;
    }
    else {
        std::vector<BTreeNode<T, B>*> tmp;
        for (auto i = 0; i < n + 1; i++) {
            tmp = edges[i]->find_nodes_at_level(lv - 1);
            std::copy(tmp.begin(), tmp.end(), std::back_inserter(nodes));
        }

        return nodes;
    }
}

template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode() : n(0), type(NodeType::LEAF) {
    for (size_t i = 0; i < 2 * B - 1; i++) {
        keys[i] = 0;
        edges[i] = nullptr;
    }
    edges[2 * B - 1] = nullptr;
}

template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode(const T& t) : n(1), type(NodeType::LEAF) {
    for (size_t i = 0; i < 2 * B - 1; i++) {
        keys[i] = 0;
        edges[i] = nullptr;
    }
    edges[2 * B - 1] = nullptr;

    keys[0] = t;
}

/* Assume the input initializer list is sorted */
template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode(std::initializer_list<T> l)
    : n(l.size()), type(NodeType::LEAF) {
    std::copy(l.begin(), l.end(), keys.begin());
}

/* Assume the input iterator is sorted. */
template<typename T, size_t B>
template<typename InputIt>
BTreeNode<T, B>::BTreeNode(InputIt begin, InputIt end)
    : n(end - begin), type(NodeType::LEAF) {
    std::copy(begin, end, keys.begin());
}

template<typename T, size_t B>
BTreeNode<T, B>::~BTreeNode() {
    if (this->type == NodeType::LEAF)
        return;

    for (auto i = 0; i < n + 1; i++)
        if (edges[i]) delete edges[i];
}


#endif // _BTREE_H