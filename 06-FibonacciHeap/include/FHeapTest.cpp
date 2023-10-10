#include "include/FHeapTest.hpp"

// Framework Check

DefineTest(module, FrameworkCheck) {
	expectTrue(1 + 1 == 2);
	int* i = new int;
	delete i;
}

template <typename T>
void traverse_root_list(FibonacciNode<T>* min_node) {
    if (!min_node) {
        return;
    }
    std::queue<T> root_key;
    root_key.push(min_node->key);
    FibonacciNode<T>* temp = min_node->right.get();
    while (temp != min_node) {
        root_key.push(temp->key);
        temp = temp->right.get();
    }
    while (!root_key.empty()) {
        T item = root_key.front();
        std::cout << item << " ";
        root_key.pop();
    }
    std::cout << std::endl;
}

template <typename T>
std::vector<FibonacciNode<T>*> get_root_list(FibonacciHeap<T>& h) {
    std::vector<FibonacciNode<T>*> root_list;

    if (h.is_empty())
        return root_list;

    FibonacciNode<T>* node = h.get_min_node();
    do {
        root_list.push_back(node);
        node = node->right.get();
    } while (node != h.get_min_node());

    return root_list;
}

template <typename T>
void check_min_heap_property(FibonacciNode<T>* n, bool& flag) {
    if (!n->child)
        return;

    std::vector<FibonacciNode<T>*> children_list;
    FibonacciNode<T>* node = n->child.get();
    do {
        children_list.push_back(node);
        node = node->right.get();
    } while (node != n->child.get());

    for (FibonacciNode<T>* child : children_list) {
        if (!(n->key <= child->key))
        {
            flag = false;
        }
        check_min_heap_property(child, flag);
    }
}

// FibonacciHeap Check
DefineTest(module, Construction)
{
    expectTrue(true);

    FibonacciHeap<int> heap1 = {};
    expectTrue(heap1.size() == 0);
    expectTrue(heap1.is_empty() == true);
    expectTrue(heap1.get_min() == std::nullopt);

    FibonacciHeap<int> heap2(3);
    expectTrue(heap2.get_min().value() == 3);

    FibonacciHeap<int> heap3 = { 2, 1 };
    expectTrue(heap3.get_min().value() == 1);
}

DefineTest(module, Insertion)
{
    expectTrue(true);

    FibonacciHeap<int> heap = {};
    std::vector<int> inserted;

    heap.insert(3);
    expectTrue(heap.get_min() == 3);

    for (int i = 0; i < 10; ++i) {
        int temp = rand() % 100;
        heap.insert(temp);
        inserted.push_back(temp);
    }
    int min_value = *std::min_element(inserted.begin(), inserted.end());
    expectTrue(heap.get_min().value() == min_value);
    expectTrue(heap.is_empty() == false);
    expectTrue(heap.size() == 11);
}

DefineTest(module, SImpleExtraction)
{
    expectTrue(true);

    FibonacciHeap<int> heap = { 10 };
    std::optional<int> min_value;

    min_value = heap.extract_min();
    expectTrue(min_value.value() == 10);
    expectTrue(heap.get_min_node() == nullptr);
    expectTrue(heap.get_min() == std::nullopt);
    expectTrue(heap.is_empty() == true);
    expectTrue(heap.size() == 0);

    for (int i = 9; i > 0; --i) {
        heap.insert(i);
    }
    min_value = heap.extract_min();

    // Check Attributes that doesn't affected by consolidate.
    expectTrue(min_value.value() == 1);
    expectTrue(heap.is_empty() == false);
    expectTrue(heap.size() == 8);
    expectTrue(heap.get_min() == 2);

    std::map<int, bool> degree_map;
    std::vector<FibonacciNode<int>*> root_list = get_root_list(heap);
    for (FibonacciNode<int>* root : root_list) {
        // Every root in the root list has a distinct degree value
        expectTrue(degree_map[root->degree] == false);
        degree_map[root->degree] = true;
        bool flag = true;
        check_min_heap_property(root, flag);

        expectTrue(flag);
    }
}

DefineTest(module, RandomTest)
{
    expectTrue(true);

    FibonacciHeap<int> heap;
    std::vector<int> inserted;
    std::vector<int> extracted;
    size_t N = 10;
    // Prepare a random vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<size_t> dis_ins(200, 500);
    std::uniform_int_distribution<size_t> dis_ext(100, 200);
    std::uniform_int_distribution<size_t> dis_key(0, 10000);

    for (size_t i = 0; i < N; ++i) {
        extracted.clear();

        size_t num_ins = dis_ins(g);
        size_t num_ext = dis_ext(g);

        for (size_t j = 0; j < num_ins; ++j) {
            int v = dis_key(g);
            inserted.emplace_back(v);
            heap.insert(v);
        }

        for (size_t k = 0; k < num_ext; ++k) {
            int ext = heap.extract_min().value();
            extracted.emplace_back(ext);
        }


        std::sort(inserted.begin(), inserted.end(), std::less<int>());
        for (size_t l = 0; l < num_ext; ++l) {
            expectTrue(inserted[l] == extracted[l]);
        }
        inserted = std::vector<int>(inserted.begin() + num_ext, inserted.end());


        // Every root in the root list has a distinct degree value
        std::map<int, bool> degree_map;
        std::vector<FibonacciNode<int>*> root_list = get_root_list(heap);
        for (FibonacciNode<int>* root : root_list) {
            expectTrue(degree_map[root->degree] == false);
            degree_map[root->degree] = true;
        }
    }
}

