#include "include/BSTTest.hpp"

// Framework Check

DefineTest(module, FrameworkCheck) {
	expectTrue(1 + 1 == 2);
	int* i = new int;
	delete i;
}

template<typename T>
void is_BST(std::unique_ptr<TreeNode<T>>& t, std::vector<T>& sorted, bool& flag)
{
    if (t)
    {
        auto ref = t->element;
        if (t->left)
            if (!(t->left->element < ref))
            {
                flag = false;
            }
        if (t->right)
            if (!(t->right->element > ref))
            {
                flag = false;
            }

        is_BST(t->left, sorted, flag);
        sorted.push_back(ref);
        is_BST(t->right, sorted, flag);
    }
}

// BST Check

DefineTest(module, InsertTest)
{
    expectTrue(true);

    BST<int> bt;

    std::vector<int> v;
    v.resize(10000);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
    auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end());
    std::random_shuffle(v.begin(), v.end());

    for (auto ele : v) {
        expectTrue(bt.insert(ele) == true);
        expectTrue(bt.insert(ele) == false);
    }

    std::vector<int> sorted;
    bool flag = true;
    is_BST(bt.root, sorted, flag);

    expectTrue(flag);
    expectTrue(std::is_sorted(sorted.begin(), sorted.end()));
    expectTrue(sorted.size() == v.size());
}

DefineTest(module, SearchTest)
{
    expectTrue(true);

    BST<int> bt;

    std::vector<int> v;
    v.resize(10000);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
    auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end());
    std::random_shuffle(v.begin(), v.end());

    for (auto ele : v) {
        expectTrue(bt.search(ele) == false);
        bt.insert(ele);
        expectTrue(bt.search(ele) == true);
    }

    std::vector<int> sorted;
    bool flag = true;
    is_BST(bt.root, sorted, flag);

    expectTrue(flag);
    expectTrue(std::is_sorted(sorted.begin(), sorted.end()));
    expectTrue(sorted.size() == v.size());
}

DefineTest(module, RemoveTest)
{
    expectTrue(true);

    BST<int> bt;

    std::vector<int> v;
    v.resize(10000);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
    auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end());
    std::random_shuffle(v.begin(), v.end());

    for (auto ele : v)
        bt.insert(ele);


    auto x = std::vector<int>(v.begin(), v.begin() + std::distance(v.begin(), v.end()) / 2);

    for (auto ele : x) {
        expectTrue(bt.remove(ele) == true);
        expectTrue(bt.remove(ele) == false);
    }

    std::vector<int> sorted;
    bool flag = true;
    is_BST(bt.root, sorted, flag);
    expectTrue(flag);
    expectTrue(std::is_sorted(sorted.begin(), sorted.end()));
    expectTrue(sorted.size() == v.size() - x.size());
}