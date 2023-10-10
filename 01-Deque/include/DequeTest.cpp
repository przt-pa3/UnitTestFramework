#include "include/DequeTest.hpp"

// Framework Check

DefineTest(module, FrameworkCheck) {
	expectTrue(1 + 1 == 2);
	int* i = new int;
	delete i;
}

// ArrayDeque Test

DefineTest(module, ArrayDequeConstruction) {
	expectTrue(true);

	ArrayDeque<int> ad;
	expectTrue(ad.size() == 0);
	expectTrue(ad.capacity() == 64);
	expectTrue(ad.empty());
}

DefineTest(module, ArrayDequePushFront) {
	expectTrue(true);

	ArrayDeque<int> ad;
	ad.push_front(1);
	ad.push_front(2);
	ad.push_front(3);
	expectTrue(ad.remove_front() == 3);
}

DefineTest(module, ArrayDequePushBack) {
	expectTrue(true);

	ArrayDeque<int> ad;
	ad.push_back(1);
	ad.push_back(2);
	ad.push_back(3);
	expectTrue(ad.remove_back() == 3);
}

DefineTest(module, ArrayDequePushAndResize) {
	expectTrue(true);

	ArrayDeque<int> ad;
	for (auto i = 0; i < 64; ++i) {
		ad.push_front(i);
	}
	ad.push_back(64);

	expectTrue(ad.capacity() == 128);
	expectTrue(ad.size() == 65);
	expectTrue(ad.remove_back() == 64);
}

DefineTest(module, ArrayDequeRandomPushIndexing) {
	expectTrue(true);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1);

	ArrayDeque<int> ad;
	std::deque<int> dq;
	size_t N = 10;
	for(auto i=0; i<N; ++i) {
		if (dis(gen)) {
			ad.push_front(i);
			dq.emplace_front(i);
		}
		else {
			ad.push_back(i);
			dq.emplace_back(i);
		}
	}

	for (auto i = 0; i < N; ++i) {
		expectTrue(ad[i] == dq[i]);
	}
}

DefineTest(module, ArrayDequeRemoveFront) {
	expectTrue(true);

	ArrayDeque<int> ad;
	ad.push_front(1);
	ad.push_front(2);
	ad.push_front(3);

	expectTrue(ad.remove_front() == 3);
	expectTrue(ad.remove_front() == 2);
	expectTrue(ad.remove_front() == 1);
}

DefineTest(module, ArrayDequeRemoveBack) {
	expectTrue(true);

	ArrayDeque<int> ad;
	ad.push_back(1);
	ad.push_back(2);
	ad.push_back(3);

	expectTrue(ad.remove_back() == 3);
	expectTrue(ad.remove_back() == 2);
	expectTrue(ad.remove_back() == 1);
}

DefineTest(module, ArrayDequeRemoveInEmpty) {
	expectTrue(true);

	ArrayDeque<int> ad;
	expectTrue(ad.remove_front() == std::nullopt);
	expectTrue(ad.remove_back() == std::nullopt);

	ad.push_front(1);
	ad.remove_back();

	expectTrue(ad.remove_front() == std::nullopt);
	expectTrue(ad.remove_back() == std::nullopt);
}

DefineTest(module, ArrayDequeEmpty) {
	expectTrue(true);

	ArrayDeque<int> ad;
	for (auto i = 0; i < 256; ++i) {
		ad.push_back(i);
	}
	expectTrue(ad.empty() == false);
	for (auto i = 0; i < 256; ++i) {
		ad.remove_front();
	}
	expectTrue(ad.empty() == true);
}



// ListDeque Test

DefineTest(module, ListDequeConstruction) {
	expectTrue(true);

	ListDeque<int> ld;
	expectTrue(ld.size() == 0);
	expectTrue(ld.empty() == true);
}

DefineTest(module, ListDequePushFront) {
	expectTrue(true);

	ListDeque<int> ld;

	size_t N = 10;
	for (auto i = 1; i <= N; ++i) {
		ld.push_front(i);
	}
	expectTrue(ld.size() == N);
	expectTrue(ld.empty() == false);

	auto it = ld.sentinel;
	for (auto i = 0; i <= N; ++i) {
		it = it->next;
	}

	expectTrue(it == ld.sentinel);

	it = ld.sentinel;
	for (auto i = 0; i <= N; ++i) {
		it = it->prev;
	}

	expectTrue(it == ld.sentinel);
}

DefineTest(module, ListDequePushBack) {
	expectTrue(true);

	ListDeque<int> ld;

	size_t N = 10;
	for (auto i = 1; i <= N; ++i) {
		ld.push_back(i);
	}
	expectTrue(ld.size() == N);
	expectTrue(ld.empty() == false);

	auto it = ld.sentinel;
	for (auto i = 0; i <= N; ++i) {
		it = it->next;
	}

	expectTrue(it == ld.sentinel);

	it = ld.sentinel;
	for (auto i = 0; i <= N; ++i) {
		it = it->prev;
	}

	expectTrue(it == ld.sentinel);
}

DefineTest(module, ListDequeRemoveFront) {
	expectTrue(true);

	std::cout << "LDRF" << std::endl;
	ListDeque<int> ld1, ld2;
	std::vector<int> vec1, vec2;
	size_t N = 10;

	for(auto i = 1; i<=N; ++i) {
		ld1.push_front(i);
		ld2.push_front(i);
	}
	
	auto it = ld2.sentinel->next;

	while(!ld1.empty()) {
		vec1.emplace_back(ld1.remove_front().value());
		vec2.emplace_back(it->value.value());
		it = it->next;
	}
	expectTrue(vec1 == vec2);
	expectTrue(ld1.empty());
	expectTrue(ld1.size() == 0);
}

DefineTest(module, ListDequeRemoveBack) {
	expectTrue(true);

	ListDeque<int> ld1, ld2;
	std::vector<int> vec1, vec2;
	size_t N = 10;

	for (auto i = 1; i <= N; ++i) {
		ld1.push_front(i);
		ld2.push_front(i);
	}

	auto it = ld2.sentinel->prev;
	while (!ld1.empty()) {
		vec1.emplace_back(ld1.remove_back().value());
		vec2.emplace_back(it->value.value());
		it = it->prev;
	}
	expectTrue(vec1 == vec2);
	expectTrue(ld1.empty());
	expectTrue(ld1.size() == 0);
}

DefineTest(module, ListDequeIndexing) {
	expectTrue(true);

	ListDeque<int> ld;
	size_t N = 10;

	for (auto i = 1; i <= N; ++i) {
		ld.push_back(i);
	}
	for (auto i = 0; i < N; ++i) {
		expectTrue(ld[i] == i + 1);
	}
}


// Palindrome Test

DefineTest(module, PalindromeArrayDeque) {
	expectTrue(true);

	Palindrome<ArrayDeque<char>> p;
	std::string s{"abc def gh i hg fed cba"};
	expectTrue(p.is_palindrome(s) == true);

	s += 'z';
	expectTrue(p.is_palindrome(s) == false);
}

DefineTest(module, PalindromeListDeque) {
	expectTrue(true);

	Palindrome<ListDeque<char>> p;
	std::string s{"abc def gh i hg fed cba"};
	expectTrue(p.is_palindrome(s) == true);

	s += 'z';
	expectTrue(p.is_palindrome(s) == false);
}