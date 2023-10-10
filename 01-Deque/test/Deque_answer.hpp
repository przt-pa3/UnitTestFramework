#ifndef _DEQUE_H
#define _DEQUE_H

#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
#include <iostream>
#include <memory>
#include <cassert>

/* NOTE: Deque, ArrayDeque, ListDeque Declaration modification is not allowed.
 * Fill in the TODO sections in the following code. */
template <typename T>
class Deque {
public:
	virtual ~Deque() = default;

	/* NOTE: We won't implement push functions that take rvalue references. */
	virtual void push_front(const T&) = 0;
	virtual void push_back(const T&) = 0;

	/* NOTE: Unlike STL implementations which have separate `front` and
	   pop_front` functions, we have one unified method for removing an elem. */
	virtual std::optional<T> remove_front() = 0;
	virtual std::optional<T> remove_back() = 0;

	virtual bool empty() = 0;
	virtual size_t size() = 0;

	virtual T& operator[](size_t) = 0;
};

template <typename T>
class ArrayDeque : public Deque<T> {
public:
	ArrayDeque();
	~ArrayDeque() = default;

	void push_front(const T&) override;
	void push_back(const T&) override;

	std::optional<T> remove_front() override;
	std::optional<T> remove_back() override;

	bool empty() override;
	size_t size() override;
	size_t capacity();

	T& operator[](size_t) override;

private:
	std::unique_ptr<T[]> arr;
	size_t front;
	size_t back;
	size_t size_;
	size_t capacity_;

	void resize();
};

template <typename T>
ArrayDeque<T>::ArrayDeque() :
	front{ 63 /* was 63 OG, You can change this */ },
	back{ 0 /* You can change this */ },
	size_{ 0 }, capacity_{ 64 } {
	arr = std::make_unique<T[]>(capacity_);
}

template <typename T>
void ArrayDeque<T>::push_front(const T& item) {
	// full check
	if (size_ == capacity_) {
		resize();
	}
	arr[front] = item;
	size_ += 1;
	front = (front + capacity_ - 1) % capacity_;
}

template <typename T>
void ArrayDeque<T>::push_back(const T& item) {
	// full check
	if (size_ == capacity_) {
		resize();
	}
	arr[back] = item;
	back = (back + 1) % capacity_;
	size_ += 1;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_front() {
	// empty check
	if (empty()) {
		return std::nullopt;
	}
	front = (front + 1) % capacity_;
	T temp = arr[front];
	arr[front] = 0;
	size_ -= 1;
	return temp;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_back() {
	// empty check
	if (empty()) {
		return std::nullopt;
	}
	back = (back - 1 + capacity_) % capacity_;
	T temp = arr[back];
	arr[back] = 0;
	size_ -= 1;
	return temp;
}

template <typename T>
void ArrayDeque<T>::resize() {
	T* temparr = new T[capacity_ * 2];
	int frontiter = (front + 1) % capacity_;

	for (int i = 0; i < capacity_ * 2; ++i) {
		temparr[i] = 0;
	}
	if (back == 0 && front == capacity_ - 1) {
		for (int i = 0; i < capacity_; ++i) {
			temparr[i] = arr[i];
		}
		front = capacity_ * 2 - 1;
		back = capacity_;
	}
	else {
		for (int i = 0; i < back; ++i) {
			temparr[i] = arr[i];
		}
		for (int i = frontiter; i < capacity_; ++i) {
			temparr[i + capacity_] = arr[i];
		}
		front = front + capacity_;
	}
	arr = std::make_unique<T[]>(capacity_ * 2);

	for (int i = 0; i < capacity_ * 2; ++i) {
		arr[i] = temparr[i];
	}

	delete[] temparr;
	capacity_ *= 2;
}

template <typename T>
bool ArrayDeque<T>::empty() {
	return size_ == 0;
}

template <typename T>
size_t ArrayDeque<T>::size() {
	return size_;
}

template <typename T>
size_t ArrayDeque<T>::capacity() {
	return capacity_;
}

template <typename T>
T& ArrayDeque<T>::operator[](size_t idx) {
	int iter = (front + 1) % capacity_;
	for (int i = 0; i < idx; ++i) {
		iter += 1;
		if (iter == capacity_) iter = 0;
	}
	return arr[iter];
	//T *temp = &arr[iter];
	//return *temp;
}

template<typename T>
struct ListNode {
	std::optional<T> value;
	ListNode* prev;
	ListNode* next;

	ListNode() : value(std::nullopt), prev(this), next(this) {}
	ListNode(const T& t) : value(t), prev(this), next(this) {}

	ListNode(const ListNode&) = delete;
};

template<typename T>
class ListDeque : public Deque<T> {
public:
	ListDeque();
	~ListDeque();

	void push_front(const T&) override;
	void push_back(const T&) override;

	std::optional<T> remove_front() override;
	std::optional<T> remove_back() override;

	bool empty() override;
	size_t size() override;

	T& operator[](size_t) override;

	size_t size_ = 0;
	ListNode<T>* sentinel = nullptr;
};

template<typename T>
ListDeque<T>::ListDeque() : sentinel(new ListNode<T>{}), size_(0) {}

template<typename T>
void ListDeque<T>::push_front(const T& t) {
	if (empty()) {
		ListNode<T>* newnode = new ListNode<T>(t);
		newnode->next = sentinel;
		newnode->prev = sentinel;
		sentinel->next = newnode;
		sentinel->prev = newnode;
		size_ = 1;
	}
	else {
		size_++;
		ListNode<T>* newnode = new ListNode<T>(t);
		newnode->prev = sentinel;
		newnode->next = sentinel->next;
		sentinel->next->prev = newnode;
		sentinel->next = newnode;
	}
}

template<typename T>
void ListDeque<T>::push_back(const T& t) {
	if (empty()) {
		ListNode<T>* newnode = new ListNode<T>(t);
		newnode->next = sentinel;
		newnode->prev = sentinel;
		sentinel->next = newnode;
		sentinel->prev = newnode;
		size_ = 1;
	}
	else {
		size_++;
		ListNode<T>* newnode = new ListNode<T>(t);
		newnode->prev = sentinel->prev;
		newnode->next = sentinel;
		sentinel->prev->next = newnode;
		sentinel->prev = newnode;
	}
}

template<typename T>
std::optional<T> ListDeque<T>::remove_front() {
	std::optional<T> temp = sentinel->next->value;
	ListNode<T>* tempnode = sentinel->next->next;
	delete sentinel->next;
	sentinel->next = tempnode;
	sentinel->next->prev = sentinel;
	size_--;
	return temp;
}

template<typename T>
std::optional<T> ListDeque<T>::remove_back() {
	std::optional<T> temp = sentinel->prev->value;
	ListNode<T>* tempnode = sentinel->prev->prev;
	delete sentinel->prev;
	sentinel->prev = tempnode;
	sentinel->prev->next = sentinel;
	size_--;
	return temp;
}

template<typename T>
bool ListDeque<T>::empty() {
	return sentinel->next == sentinel;
}

template<typename T>
size_t ListDeque<T>::size() {
	return size_;
}

template<typename T>
T& ListDeque<T>::operator[](size_t idx) {
	ListNode<T>* nodeiter = sentinel->next;
	for (int i = 0; i < idx; ++i) {
		nodeiter = nodeiter->next;
	}
	return (nodeiter->value).value();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListNode<T>& n) {
	if (n.value)
		os << n.value.value();

	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListDeque<T>& l) {
	auto np = l.sentinel->next;
	while (np != l.sentinel) {
		os << *np << ' ';
		np = np->next;
	}
	return os;
}

template<typename T>
ListDeque<T>::~ListDeque() {
	while (!empty()) {
		remove_front();
	}
	delete sentinel;
}

#endif // _DEQUE_H
