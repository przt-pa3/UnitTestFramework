#ifndef _FHEAP_H
#define _FHEAP_H

#include <iostream>
#include <initializer_list>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>

template <typename T>
class PriorityQueue {
public:
    virtual void insert(const T& item) = 0;
    virtual std::optional<T> extract_min() = 0;
    virtual bool is_empty() const = 0;
};

template <typename T>
class FibonacciNode {
public:
    // constructors
    FibonacciNode()
        :key(std::nullopt), degree(0), child(nullptr), right(nullptr) {}
    FibonacciNode(const T& item)
        :key(item), degree(0), child(nullptr), right(nullptr) {}

    // destructor
    ~FibonacciNode() = default;

    T key;
    size_t degree;

    std::shared_ptr<FibonacciNode<T>> right;
    std::shared_ptr<FibonacciNode<T>> child;
    // NOTE: If you set left/parent pointer to shared_ptr, circular reference may cause!
    // So, left/parent pointer should be set to weak_ptr.
    std::weak_ptr<FibonacciNode<T>> left;
    std::weak_ptr<FibonacciNode<T>> parent;
};

template <typename T>
class FibonacciHeap : public PriorityQueue<T> {
public:
    // Default Constructor
    FibonacciHeap()
        : min_node(nullptr), size_(0) {}

    // Constructor with Value
    FibonacciHeap(const T& item)
        : min_node(nullptr), size_(0) {
        insert(item);
    }

    // Disable copy constructor.
    FibonacciHeap(const FibonacciHeap<T>&);

    // Constructor with initializer_list
    // ex) FibonacciHeap<int> fheap = {1, 2, 3, 4};
    FibonacciHeap(std::initializer_list<T> list) : min_node(nullptr), size_(0) {
        for (const T& item : list) {
            insert(item);
        }
    }

    // Destructor
    ~FibonacciHeap();

    // Insert item into the heap.
    void insert(const T& item) override;

    // Return raw pointer of the min node.
    // If the heap is empty, return nullptr.
    FibonacciNode<T>* get_min_node() { return min_node.get(); }

    // Return minimum value of the min node.
    // If the heap is empty, return std::nullopt.
    std::optional<T> get_min() const;

    // 1. Return minimum value of the min node
    // 2. Remove the node which contains minimum value from the heap.
    // If the heap is empty, return std::nullopt;
    std::optional<T> extract_min() override;

    // Return true if the heap is empty, false if not.
    bool is_empty() const override { return !size_; }

    // Return the number of nodes the heap contains.
    size_t size() const { return size_; }


private:
    // Points to the node which has minimum value.
    std::shared_ptr<FibonacciNode<T>> min_node;

    // Value that represents how many nodes the heap contains.
    size_t size_;

    void insert(std::shared_ptr<FibonacciNode<T>>& node);

    // After extract, clean up the heap.
    void consolidate();

    // Combine two nodes.
    void merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y);
};

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    // TODO

    std::optional<T> temp;
    size_t tempsize = this->size_;
    /*
    if(!this->is_empty()){
        for (size_t i = 0; i < tempsize; ++i){
            temp = this->extract_min();
        }
    }
    */
    //std::cout<<"destructor called"<<std::endl;

    while (!this->is_empty()) {
        //std::cout<<"destructor in loop" << std::endl;
        temp = this->extract_min();
    }
    //std::cout << "size : " << this->size_ << std::endl;
    //if(min_node->right != nullptr)
        //min_node->right = nullptr;
    min_node = nullptr;

}

template <typename T>
std::optional<T> FibonacciHeap<T>::get_min() const {
    // TODO
    if (is_empty())
        return std::nullopt;
    else
        return min_node->key;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
    // TODO
    std::shared_ptr<FibonacciNode<T>> new_node = std::make_shared<FibonacciNode<T>>(item);
    if (is_empty()) {
        min_node = new_node;
        min_node->right = min_node;
        min_node->left = min_node;
        size_++;
    }
    else {
        std::shared_ptr<FibonacciNode<T>> temp = min_node->left.lock();
        if (temp != min_node) {
            temp->right = new_node;
            new_node->left = temp;
            new_node->right = min_node;
            min_node->left = new_node;
        }
        else {
            // min_node is a only root
            min_node->left = new_node;
            min_node->right = new_node;
            new_node->left = min_node;
            new_node->right = min_node;
        }

        if (new_node->key < min_node->key)
            min_node = new_node;
        size_++;
    }
}

template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {
    // TODO
    if (!min_node) {
        min_node = node;
        node->right = node;
        node->left = node;
    }
    else {
        std::shared_ptr<FibonacciNode<T>> temp = min_node->left.lock();
        if (temp != min_node) {
            temp->right = node;
            node->left = temp;
            node->right = min_node;
            min_node->left = node;
        }
        else {
            // min_node is a only root
            min_node->left = node;
            min_node->right = node;
            node->left = min_node;
            node->right = min_node;
        }
    }
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    // TODO
    std::optional<T> return_value = min_node->key;
    std::shared_ptr<FibonacciNode<T>> min_child;

    if (min_node->right == min_node) {
        if (min_node->degree == 0) {
            // Last node
            size_--;
            return_value = min_node->key;
            min_node->right = nullptr;
            min_node = nullptr;
            return return_value;
        }
        else {
            // has child
            return_value = min_node->key;
            min_child = min_node->child;
            min_node->right = nullptr;
            min_node->child = nullptr;
            min_node = nullptr;
        }
    }
    else {
        std::shared_ptr<FibonacciNode<T>> left_root = min_node->left.lock();
        // two node
        if (left_root == min_node->right) {
            left_root->right = left_root;
            left_root->left = left_root;
        }
        else {
            // more than two node
            left_root->right = min_node->right;
            (min_node->right)->left = left_root;
        }
        min_child = min_node->child;
        min_node->child = nullptr;
    }

    if (min_node != nullptr) min_node = min_node->right;
    if (min_child) {
        if (min_child->right != min_child) {
            std::shared_ptr<FibonacciNode<T>> min_sib = min_child->right;
            while (1) {
                if (min_sib->right == min_child) {
                    insert(min_sib);
                    break;
                }
                else {
                    min_child->right = min_sib->right;
                    insert(min_sib);
                    min_sib = min_child->right;
                }
            }
            insert(min_child);
        }
        else {
            // min_child is alone
            insert(min_child);
        }
    }
    consolidate();
    size_--;
    return return_value;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    // TODO
    size_t arr_size = 100;
    std::shared_ptr<FibonacciNode<T>> array[100];

    for (size_t i = 0; i < arr_size; ++i) {
        array[i] = nullptr;
    }
    size_t index = 0;
    std::shared_ptr<FibonacciNode<T>> pin = min_node;
    index = pin->degree;

    array[index] = pin;
    size_t cnt = 1;
    bool test;

    if (pin->right != pin) {
        pin = pin->right;
        while (1) {
            test = true;
            index = pin->degree;
            if (array[index] != nullptr) {
                if (array[index]->key < pin->key) {
                    merge(array[index], pin);
                    if (min_node == pin)
                        min_node = array[index];
                }
                else {
                    merge(pin, array[index]);
                    if (min_node == array[index])
                        min_node = pin;
                }

                if (min_node->right != nullptr) {
                    for (size_t i = 0; i < arr_size; ++i) {
                        array[i] = nullptr;
                    }
                    pin = min_node;
                    test = false;
                    cnt = 1;
                }
                else {
                    // min_node is alone?
                    break;
                }
            }
            else {
                array[index] = pin;
                pin = pin->right;
                cnt++;
            }
            if (pin == min_node && test) {
                break;
            }
        }
    }
    pin = min_node;

    for (size_t i = 0; i < cnt + 1; ++i) {
        if (pin->right != nullptr)
            pin = pin->right;
        if (pin->key < min_node->key)
            min_node = pin;
    }
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
    // TODO

    if (y->left.lock() == x && y->right == x) {
        x->right = x;
        x->left.lock() = x;
    }
    else {
        y->left.lock()->right = y->right;
        y->right->left = y->left;
    }
    if (x->child == nullptr) {
        x->child = y;
        // might be problem when delete..
        y->right = y;
        y->left = y;
    }
    else {
        if (x->child->right == x->child) {
            // x->child was only child
            x->child->right = y;
            x->child->left = y;
            y->right = x->child;
            y->left = x->child;
        }
        else {
            y->right = x->child->right;
            x->child->right = y;
            y->right->left = y;
            y->left = x->child;
        }
    }

    y->parent = x;
    x->degree = (x->degree) + 1;

}

#endif // _FHEAP_H
