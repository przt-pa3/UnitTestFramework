# 4: Leaf-Leaning Red-Black Tree

## Goals

* Get to implement an extreme data structure that is used widely today.

* Implement insert and delete function that inserts and deletes an item
  to/from a red-black tree without affecting the invariants of the tree
  (e.g., black balance, no back-to-back red, etc.)

### Table of contents

1. [Left-Leaning Red-Black Tree](#Leaf-Leaning-Red-Black-Tree)
2. [Types](#Types)
3. [Tasks](#Tasks)

## Left-Leaning Red-Black Tree

### Background

Red-black trees are famous variants of a balanced search tree. It is used
by the Linux kernel, standard library of various programming languages, etc.
However, the original algorithm of red-black tree was *very* complex.
In 2008, Robert Sedgewick invented a simple implementation of the red-black
tree that works pretty fine and whose implementation is way shorter than
the known ones.

### Properties

Red-black trees have to satisfy the following properties:

0. There are red nodes and black nodes.
1. The number of black nodes along with all paths to the leaves are the same.
2. No two red nodes appears back-to-back, which ensures the length of
   the longest path is 2x times longer at maximum than the shortest path.
3. The root node is black

Left-leaning red-black tree adds an extra constraint that simplifies the
implementation:

4. If only one child of a black node is red, it must be the left one.

## Types

We will take a look at the types. Do you remember `std::optional<T>` and
`std::unique_ptr<T>`?:

```c++
template<typename T>
struct RBTree {
    std::unique_ptr<RBNode<T>> root = nullptr;

    ~RBTree() = default;

    bool insert(const T&);
    void remove_max();
    void remove_min();
    void remove(const T&);

    const std::optional<T> leftmost_key();
    const std::optional<T> rightmost_key();

    bool contains(const T& t);
};

template<typename T>
struct RBNode {
    T key;
    color_t color = RED;
    std::unique_ptr<RBNode> left = nullptr;
    std::unique_ptr<RBNode> right = nullptr;

    RBNode(const T& t);
    ~RBNode() = default;

    bool is_leaf();

    void flip_color();
    static RBNode* rotate_right(std::unique_ptr<RBNode>&);
    static RBNode* rotate_left(std::unique_ptr<RBNode>&);
    static bool is_red(const std::unique_ptr<RBNode>&);

    static RBNode* move_red_right(std::unique_ptr<RBNode>&);
    static RBNode* move_red_left(std::unique_ptr<RBNode>&);

    static RBNode* remove_max(std::unique_ptr<RBNode>&);
    static RBNode* remove_min(std::unique_ptr<RBNode>&);
    static RBNode* remove(std::unique_ptr<RBNode>&, const T&);

    static RBNode* insert(std::unique_ptr<RBNode>&, const T&);

    static RBNode* fix_up(std::unique_ptr<RBNode>&);

    bool contains(const T& t);

    const T& leftmost_key();
    const T& rightmost_key();
};
```
## Tasks

Fill TODO

For insertion, you should fill in

* `RBNode::rotate_right`
* `RBNode::rotate_left`
* `RBNode::flip_color`
* `RBNode::insert`

These are functions that are required to implement deletion:

* `RBNode::fix_up`
* `RBNode::remove`
* `RBNode::remove_max`
* `RBNode::remove_min`
* `RBNode::move_red_right`
* `RBNode::move_red_left`

### Tips

* Use `get()` and `release()` methods to extract raw pointers from `unique_ptr`;
* There is a typo in pseudo code of the original slide of left-leaning red-black
tree; and
* Sometimes, `is_red` is not enough, and you should check whether the pointer
is a `nullptr`, e.g, `ptr && is_red(ptr->left)` instead of `is_red(ptr->left)`,
to avoid null pointer dereferences.

## References

1. Robert Sedgewick, Left-Leaning Red-Black Tree, 2008, [Link](https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf)

* Test Cases  
SNU Course "Introduction to Data Structures" (Crs. No. 430.217)  
 - Professor Byuongyoung Lee (https://lifeasageek.github.io/),  
 - TA(PhD Student) Sangyun Kim (ksu880@snu.ac.kr)