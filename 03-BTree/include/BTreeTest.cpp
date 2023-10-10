#include "include/BTreeTest.hpp"

// Framework Check

DefineTest(module, FrameworkCheck) {
	expectTrue(1 + 1 == 2);
	int* i = new int;
	delete i;
}

// BTree Check

DefineTest(module, 2_4_InorderTraversal)
{
	expectTrue(true);

    BTree<int, 2> tree;

    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<int> ws;
    size_t N = 100'000;

    for (auto i = 1; i <= N; i++)
        ws.push_back(i);

    std::shuffle(ws.begin(), ws.end(), g);

    for (auto i = 0; i < N; i++)
        tree.insert(ws[i]);

    std::vector<int> xs, ys;

    for (auto i = 1; i <= N; i++)
        xs.push_back(i);

    tree.for_all([&](int& i) { ys.push_back(i); });

    expectTrue(xs == ys);
}

DefineTest(module, InorderTraversal)
{
	expectTrue(true);

    BTree<int> tree;
    std::vector<int> xs, ys, zs;
    size_t N = 1'000'000;

    /* Prepare a random vector */
    std::random_device rd;
    std::mt19937 g(rd());

    for (auto i = 1; i < N; i++) {
        xs.push_back(i);
        zs.push_back(i);
    }

    std::shuffle(xs.begin(), xs.end(), g);

    /* Insert a random sequence to the B-tree */
    for (auto i : xs)
        tree.insert(i);

    /* Iterate over the B-tree. Are the elements sorted? */
    tree.for_all([&](int& i) { ys.push_back(i); });

    expectTrue(zs == ys);
    expectTrue(BTreeNode<int, 6>::find_rightmost_key(*(tree.root)) == N - 1);
}

DefineTest(module, NodeUtilization)
{
	expectTrue(true);

    static constexpr size_t B = 10;
    BTree<int, B> btree;
    std::vector<int> xs;
    size_t n = 100'000;

    std::random_device rd;
    std::mt19937 g(rd());

    for (auto i = 1; i <= n; i++)
        xs.push_back(i);

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto i : xs)
        btree.insert(i);

    std::vector<size_t> num_keys;

    /* Visit all nodes and collect the number of keys */
    btree.for_all_nodes([&num_keys](const BTreeNode<int, B>& bn) {
        num_keys.emplace_back(bn.n);
        });

    /* sum(# keys per nodes) should equal to the # keys that are inserted */
    auto total_num_keys =
        std::accumulate(num_keys.begin(), num_keys.end(), 0 /* sum */);

    expectTrue(total_num_keys == n);

    /* All nodes (except one node) should contain # keys between B-1 and
       2B-1 */
    bool root_visited = false;
    expectTrue(std::all_of(num_keys.begin(), num_keys.end(),
        [&root_visited](auto num_keys) {
            if (B - 1 <= num_keys && num_keys <= 2 * B - 1)
                return true;
            else if (!root_visited) {
                /* One node (i.e., the root node) is allowed to
                   have less than B-1 keys. */
                root_visited = true;
                return true;
            }
            else {
                return false;
            }}));
}

DefineTest(module, PerfectBalance)
{
	expectTrue(true);

    static constexpr size_t B = 7;
    BTree<int, B> btree;
    std::vector<int> xs;
    size_t n = 10'000;

    std::random_device rd;
    std::mt19937 g(rd());

    for (auto i = 1; i <= n; i++)
        xs.push_back(i);

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto i : xs)
        btree.insert(i);

    auto depth = btree.depth();

    expectTrue(btree.find_rightmost_key().has_value());
    expectTrue(btree.find_rightmost_key().value() == n);
    expectTrue(depth.has_value());

    /* Test internal nodes */
    for (auto i = 0; i < depth.value(); i++) {
        auto nodes_at_level_i = btree.root->find_nodes_at_level(i);

        expectTrue(std::all_of(nodes_at_level_i.begin(),
            nodes_at_level_i.end(),
            [](const BTreeNode<int, B>* n) {
                return n->type == NodeType::INTERNAL;
            }));
    }


    /* Test leaf nodes */
    auto nodes_at_leaf_level = btree.root->find_nodes_at_level(depth.value());
    expectTrue(std::all_of(nodes_at_leaf_level.begin(),
        nodes_at_leaf_level.end(),
        [](const BTreeNode<int, B>* n) {
            return n->type == NodeType::LEAF;
        }));
}

DefineTest(module, Delete)
{
	expectTrue(true);

    BTree<int, 2> tree;
    size_t N = 100'000;

    std::vector<int> xs, ys;

    for (auto i = 1; i <= N; i++) {
        xs.push_back(i);
        ys.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(xs.begin(), xs.end(), g);
    std::shuffle(ys.begin(), ys.end(), g);

    for (auto i : xs)
        tree.insert(i);

    for (auto i : ys)
        tree.remove(i);

    expectTrue(tree.root->n == 0);
}

DefineTest(module, TraversalAfterDeletion)
{
	expectTrue(true);

    BTree<int> btree;
    std::vector<int> xs, ys, zs, ws;
    auto N = 100'000u;

    std::random_device rd;
    std::mt19937 g(rd());

    for (auto i = 1u; i <= N; i++) {
        xs.push_back(i);
        ys.push_back(i);
        if (i % 2 == 0)
            zs.push_back(i);
    }

    std::shuffle(xs.begin(), xs.end(), g);
    std::shuffle(ys.begin(), ys.end(), g);

    for (auto i : xs)
        btree.insert(i);

    /* Remove all odd numbers */
    for (auto i : ys)
        if (i % 2 != 0)
            btree.remove(i);

    /* In-order traversal */
    btree.for_all([&ws](int& i) { ws.push_back(i); });

    expectTrue(zs == ws);
}

DefineTest(module, NodeUtilizationAfterDeletion)
{
	expectTrue(true);

    static constexpr size_t B = 2;
    BTree<int, B> btree;
    std::vector<int> xs;
    size_t n = 100'000;

    std::random_device rd;
    std::mt19937 g(rd());

    for (auto i = 1; i <= n; i++)
        xs.push_back(i);

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto i : xs)
        btree.insert(i);


    for (auto i : xs)
        if (i % 2 == 0)
            btree.remove(i);

    std::vector<size_t> num_keys;

    /* Visit all nodes and collect the number of keys */
    btree.for_all_nodes([&num_keys](const BTreeNode<int, B>& bn) {
        num_keys.emplace_back(bn.n);
        });

    /* sum(# keys per nodes) should equal to the # keys that are inserted */
    auto total_num_keys =
        std::accumulate(num_keys.begin(), num_keys.end(), 0 /* sum */);

    expectTrue(total_num_keys == n / 2);

    /* All nodes (except one node) should contain # keys between B-1 and
       2B-1 */
    bool root_visited = false;
    expectTrue(std::all_of(num_keys.begin(), num_keys.end(),
        [&root_visited](auto num_keys) {
            if (B - 1 <= num_keys && num_keys <= 2 * B - 1)
                return true;
            else if (!root_visited) {
                /* One node (i.e., the root node) is allowed to
                   have less than B-1 keys. */
                root_visited = true;
                return true;
            }
            else {
                return false;
            }}));
}

