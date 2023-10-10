#include "include/RBTreeTest.hpp"

// Framework Check

DefineTest(module, FrameworkCheck) {
	expectTrue(1 + 1 == 2);
	int* i = new int;
	delete i;
}

template <typename T>
bool is_red(const RBNode<T>* n) {
	return n && n->color == RED;
}

template<typename T>
bool test_left_lean(const RBTree<T>& rbtree) {
	auto res = true;

	if (!rbtree.root)
		return true;

	rbtree.root->traverse_inorder([&res](RBNode<int>* n) {
		if (is_red(n)) {
			if (RBNode<int>::is_red(n->right) ||
				RBNode<int>::is_red(n->left)) {
				/* No back-to-back red */
				res = false;
			}
		}

		if (RBNode<int>::is_red(n->right) &&
			!RBNode<int>::is_red(n->left)) {
			/* No right leaning node */
			res = false;
		}
		});

	return res;
}

template<typename T>
bool test_black_balance(const RBTree<T>& rbtree) {
	size_t num_black = 0;
	auto all_leaves = rbtree.collect_all_leaves();

	auto res = std::all_of(all_leaves.begin(), all_leaves.end(),
		[&num_black](const auto& pn) {
			if (num_black == 0) {
				num_black = pn.first.num_black_;
				return true;
			}

			return num_black == pn.first.num_black_;
		});

	return res;
}

// RBTree Check
DefineTest(module, BlackBalance)
{
	expectTrue(true);

	RBTree<int> rbtree;
	std::vector<int> xs;
	size_t n = 100'000;

	for (auto i = 0; i < n; i++)
		xs.emplace_back(i);

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto i : xs)
		rbtree.insert(i);

	expectTrue(test_black_balance(rbtree));
}

DefineTest(module, InorderTraversal)
{
	expectTrue(true);

	std::vector<int> xs, ys, zs;
	RBTree<int> rbtree;
	size_t n = 100'000;

	for (auto i = 1; i <= n; i++) {
		xs.emplace_back(i);
		ys.emplace_back(i);
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto i : xs)
		rbtree.insert(i);

	rbtree.root->traverse_inorder([&zs](RBNode<int>* n) {
		zs.emplace_back(n->key);
		});

	expectTrue(zs == ys);
}

DefineTest(module, LeftLean)
{
	expectTrue(true);

	std::vector<int> xs, ys, zs;
	RBTree<int> rbtree;
	size_t n = 100'000;

	for (auto i = 1; i <= n; i++) {
		xs.emplace_back(i);
		ys.emplace_back(i);
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto i : xs)
		rbtree.insert(i);

	expectTrue(test_left_lean(rbtree));
}

DefineTest(module, RemoveMax)
{
	expectTrue(true);

	std::vector<int> xs;
	std::random_device rd;
	std::mt19937 g(rd());
	RBTree<int> rbtree;
	auto N = 1'000u;

	for (auto i = 0; i < N; i++)
		xs.push_back(i + 1);

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto x : xs)
		rbtree.insert(x);

	for (auto i = 0; i < N; i++) {
		expectTrue(rbtree.rightmost_key().value() == N - i);

		rbtree.remove_max();

		expectTrue(test_left_lean(rbtree));
		expectTrue(test_black_balance(rbtree));
	}
}

DefineTest(module, RemoveMin)
{
	expectTrue(true);

	std::vector<int> xs;
	std::random_device rd;
	std::mt19937 g(rd());
	RBTree<int> rbtree;
	auto N = 1'000u;

	for (auto i = 0; i < N; i++)
		xs.push_back(i + 1);

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto x : xs)
		rbtree.insert(x);

	for (auto i = 0; i < N; i++) {
		expectTrue(rbtree.leftmost_key().value() == i + 1);

		rbtree.remove_min();

		expectTrue(test_left_lean(rbtree));
		expectTrue(test_black_balance(rbtree));
	}
}

DefineTest(module, BlackBalanceAfterDeletion)
{
	expectTrue(true);

	RBTree<int> rbtree;
	size_t n = 1'000;

	std::vector<int> xs;

	for (auto i = 0; i < n; i++)
		xs.emplace_back(i);

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto x : xs)
		rbtree.insert(x);

	for (auto j = 0; j < xs.size(); j++) {
		if (j > xs[j]) {
			expectTrue(rbtree.contains(xs[j]));
			rbtree.remove(xs[j]);
			expectTrue(!rbtree.contains(xs[j]));

			auto is_left_lean = test_left_lean(rbtree);
			expectTrue(is_left_lean);

			auto is_black_balance = test_black_balance(rbtree);
			expectTrue(is_black_balance);
		}

	}

	expectTrue(test_left_lean(rbtree));
	expectTrue(test_black_balance(rbtree));
}

DefineTest(module, InorderTraversalAfterDeletion)
{
	expectTrue(true);

	std::vector<int> xs, ys, zs;
	RBTree<int> rbtree;
	size_t n = 1'000;

	for (auto i = 1; i <= n; i++) {
		xs.emplace_back(i);
		ys.emplace_back(i);
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(xs.begin(), xs.end(), g);

	for (auto i : xs)
		rbtree.insert(i);

	auto num_removed = 0u;
	for (auto j = 0; j < xs.size(); j++) {
		if (j > xs[j]) {
			rbtree.remove(xs[j]);
			num_removed++;
			std::remove(ys.begin(), ys.end(), xs[j]);
		}
	}

	ys.resize(ys.size() - num_removed);

	rbtree.root->traverse_inorder([&zs](RBNode<int>* n) {
		zs.emplace_back(n->key);
		});

	expectTrue(zs == ys);
}
