#include "include/HashTableTest.hpp"

#define LinearIntStrHt LinearProbeHashTable<int, std::string, DefaultHash>
#define QuadIntStrHt QuadProbeHashTable<int, std::string, DefaultHash>

// Framework Check

DefineTest(module, FrameworkCheck) {
	expectTrue(1 + 1 == 2);
	int* i = new int;
	delete i;
}

// HashTable Check
DefineTest(module, SimpleTest_Linear)
{
	expectTrue(true);
	
	LinearIntStrHt htable;
	
	expectTrue(htable.put(1, "1") >= 0);
	expectTrue(htable.put(2, "2") >= 0);
	expectTrue(htable.put(3, "3") >= 0);

	std::string value;
	expectTrue(htable.get(2, value) >= 0);
	expectTrue(value == "2");

	expectTrue(htable.get(3, value) >= 0);
	expectTrue(value == "3");

	expectTrue(htable.remove(3) >= 0);
	expectTrue(htable.get(3, value) == -1);
}

DefineTest(module, SimpleTest_Quad)
{
	expectTrue(true);

	QuadIntStrHt htable;

	expectTrue(htable.put(1, "1") >= 0);
	expectTrue(htable.put(2, "2") >= 0);
	expectTrue(htable.put(3, "3") >= 0);

	std::string value;
	expectTrue(htable.get(2, value) >= 0);
	expectTrue(value == "2");

	expectTrue(htable.get(3, value) >= 0);
	expectTrue(value == "3");

	expectTrue(htable.remove(3) >= 0);
	expectTrue(htable.get(3, value) == -1);
}

DefineTest(module, EnlargeInsert_Linear)
{
	expectTrue(true);

	LinearIntStrHt htable;

	int num_to_test = 10000;
	std::string value;

	expectTrue(htable.get_table_size() == INITIAL_TABLE_SIZE);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.put(i, std::to_string(i)) >= 0);
		expectTrue(htable.get_load_factor() <= 0.5);
	}
	expectTrue(htable.get_size() == num_to_test);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.get(i, value) >= 0);
		expectTrue(value == std::to_string(i));
	}
}

DefineTest(module, EnlargeInsert_Quad)
{
	expectTrue(true);

	QuadIntStrHt htable;

	int num_to_test = 10000;
	std::string value;

	expectTrue(htable.get_table_size() == INITIAL_TABLE_SIZE);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.put(i, std::to_string(i)) >= 0);
		expectTrue(htable.get_load_factor() <= 0.5);
	}
	expectTrue(htable.get_size() == num_to_test);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.get(i, value) >= 0);
		expectTrue(value == std::to_string(i));
	}

}

DefineTest(module, EnlargeRemove_Linear)
{
	expectTrue(true);

	LinearIntStrHt htable;

	int num_to_test = 10000;
	std::string value;

	expectTrue(htable.get_table_size() == INITIAL_TABLE_SIZE);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.put(i, std::to_string(i)) >= 0);
		expectTrue(htable.get_load_factor() <= 0.5);
	}
	expectTrue(htable.get_size() == num_to_test);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.get(i, value) >= 0);
		expectTrue(value == std::to_string(i));
	}

	expectTrue(htable.get_size() == num_to_test);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.remove(i) >= 0);
		expectTrue(htable.get(i, value) == -1);

		expectTrue(htable.get_size() == num_to_test - i - 1);
		expectTrue(htable.get_load_factor() <= 0.5);
	}

	expectTrue(htable.get_size() == 0);
}

DefineTest(module, EnlargeRemove_Quad)
{
	expectTrue(true);

	QuadIntStrHt htable;

	int num_to_test = 10000;
	std::string value;

	expectTrue(htable.get_table_size() == INITIAL_TABLE_SIZE);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.put(i, std::to_string(i)) >= 0);
		expectTrue(htable.get_load_factor() <= 0.5);
	}
	expectTrue(htable.get_size() == num_to_test);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.get(i, value) >= 0);
		expectTrue(value == std::to_string(i));
	}

	expectTrue(htable.get_size() == num_to_test);

	for (auto i = 0; i < num_to_test; i++) {
		expectTrue(htable.remove(i) >= 0);
		expectTrue(htable.get(i, value) == -1);

		expectTrue(htable.get_size() == num_to_test - i - 1);
		expectTrue(htable.get_load_factor() <= 0.5);
	}

	expectTrue(htable.get_size() == 0);
}

DefineTest(module, RandomTest_Linear)
{
	expectTrue(true);

	LinearIntStrHt htable;

	int num_to_insert = 3000;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, num_to_insert / 10);

	generator.seed(0);
	for (auto i = 0; i <= num_to_insert; i++) {
		int k = distribution(generator);
		std::string v = std::to_string(i);
		htable.put(k, v);
	}

	generator.seed(0);
	for (auto i = 0; i <= num_to_insert; i++) {
		int k = distribution(generator);
		std::string v;
		bool res = htable.get(k, v);
		if (res) {
			expectTrue(v == std::to_string(i));
			htable.remove(k);
		}
	}
}

DefineTest(module, RandomTest_Quad)
{
	expectTrue(true);

	QuadIntStrHt htable;

	int num_to_insert = 3000;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, num_to_insert / 10);

	generator.seed(0);
	for (auto i = 0; i <= num_to_insert; i++) {
		int k = distribution(generator);
		std::string v = std::to_string(i);
		htable.put(k, v);
	}

	generator.seed(0);
	for (auto i = 0; i <= num_to_insert; i++) {
		int k = distribution(generator);
		std::string v;
		bool res = htable.get(k, v);
		if (res) {
			expectTrue(v == std::to_string(i));
			htable.remove(k);
		}
	}
}

DefineTest(module, ProbeTest_Linear)
{
	expectTrue(true);

	LinearIntStrHt htable;

	int num_to_test = INITIAL_TABLE_SIZE / 2;
	std::vector<int> arr(num_to_test);
	unsigned long total_steps = 0;

	int after_cluster = 0;
	for (auto i = 0; i < num_to_test; i++) {
		if (i < num_to_test / 2)
			arr[i] = i * INITIAL_TABLE_SIZE;
		else
			arr[i] = ++after_cluster;
	}

	after_cluster = 0;
	for (auto i = 0; i < num_to_test; i++) {
		int step = htable.put(arr[i], std::to_string(arr[i]));

		if (i < num_to_test / 2)
			expectTrue(step == i);
		else {
			expectTrue(step == (num_to_test / 2 - 1));
		}
		total_steps += step;
	}
}

DefineTest(module, ProbeTest_Quad)
{
	expectTrue(true);

	QuadIntStrHt htable;

	int num_to_test = INITIAL_TABLE_SIZE / 2;
	std::vector<int> arr(num_to_test);
	unsigned long total_steps = 0;

	int after_cluster = 0;
	for (auto i = 0; i < num_to_test; i++) {
		if (i < num_to_test / 2)
			arr[i] = i * INITIAL_TABLE_SIZE;
		else
			arr[i] = ++after_cluster;
	}

	after_cluster = 0;
	for (auto i = 0; i < num_to_test; i++) {
		int step = htable.put(arr[i], std::to_string(arr[i]));

		if (i < num_to_test / 2)
			expectTrue(step == i);
		else {
			expectTrue(step < num_to_test / 4);
		}
		total_steps += step;
	}
}
