#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>

#define INITIAL_TABLE_SIZE 64

#include "HashSlot_answer.hpp"

/* Fill in the TODO sections in the following code. */

template <typename K, typename V, typename F>
class HashTable {
public:
    HashTable();
    ~HashTable();
    int get(const K& key, V& value);
    int put(const K& key, const V& value);
    int remove(const K& key);
    size_t get_table_size();
    size_t get_size();
    double get_load_factor();

protected:
    size_t table_size;

private:
    HashTable(const HashTable& other);
    const HashTable& operator=(const HashTable& other);
    F hash_func;
    size_t size;
    HashSlot<K, V>* table;

    // Should be overriden by the derived class
    virtual unsigned long get_next_pos(unsigned long pos,
        unsigned long step) = 0;
    unsigned long get_pos(const K key);
    void enlarge_table();
};

template <typename K, typename V, typename F>
class LinearProbeHashTable : public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        size_t m = this->get_table_size();
        return (pos + step) % m;
    }
};

template <typename K, typename V, typename F>
class QuadProbeHashTable : public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        size_t m = this->get_table_size();
        return (unsigned long)(pos + 0.5 * step + 0.5 * step * step) % m;
    }
};

template <typename K, typename V, typename F>
HashTable<K, V, F>::HashTable() : table(), hash_func(),
size(0), table_size(INITIAL_TABLE_SIZE) {
    table = new HashSlot<K, V>[table_size];
}

template <typename K, typename V, typename F>
HashTable<K, V, F>::~HashTable() {
    // TODO
    delete[] this->table;
}

template <typename K, typename V, typename F>
void HashTable<K, V, F>::enlarge_table() {
    // TODO
    HashSlot<K, V>* oldtable = this->table;
    HashSlot<K, V>* newtable;
    auto tempsize = this->table_size;
    this->table_size = this->table_size * 2;
    newtable = new HashSlot<K, V>[this->table_size];

    this->table = newtable;
    this->size = 0;
    for (auto i = 0; i < tempsize; ++i) {
        if (!oldtable[i].is_empty()) {
            K tempkey = oldtable[i].get_key();
            V tempvalue = oldtable[i].get_value();
            this->put(tempkey, tempvalue);
        }
    }

    delete[] oldtable;
}

template <typename K, typename V, typename F>
unsigned long HashTable<K, V, F>::get_pos(const K key) {
    // TODO
    return hash_func(key) % this->get_table_size();
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::get(const K& key, V& value) {
    // TODO
    auto init = get_pos(key);
    auto index = init;
    unsigned long probcnt = 0;

    while (1) {
        if (!this->table[index].is_empty() && this->table[index].get_key() == key) {
            value = this->table[index].get_value();
            return probcnt;
        }
        else if (this->table[index].is_empty() && !this->table[index].is_removed()) {
            return -1;
        }
        else {
            probcnt++;
            index = get_next_pos(init, probcnt);
        }
    }

    return -1;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::put(const K& key, const V& value) {
    // TODO
    auto init = get_pos(key);
    auto index = init;
    unsigned long probcnt = 0;

    while (1) {
        if (this->table[index].is_empty()) {
            this->table[index].set_key_value(key, value);
            this->size++;
            if (this->get_load_factor() >= 0.5) {
                this->enlarge_table();
            }
            return probcnt;
        }
        else if (!this->table[index].is_empty() && this->table[index].get_key() == key) {
            return -1;
        }
        else {
            probcnt++;
            index = get_next_pos(init, probcnt);
        }
    }

    return -1;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::remove(const K& key) {
    // TODO    
    V tempvalue;
    auto init = get_pos(key);
    auto index = init;
    auto probcnt = this->get(key, tempvalue);
    if (probcnt >= 0) {
        index = this->get_next_pos(init, probcnt);
        this->table[index].set_empty();
        this->table[index].set_removed();
        this->size--;
        return probcnt;
    }
    else {
        return -1;
    }
}

template <typename K, typename V, typename F>
size_t HashTable<K, V, F>::get_table_size() {
    return table_size;
}

template <typename K, typename V, typename F>
size_t HashTable<K, V, F>::get_size() {
    return size;
}

template <typename K, typename V, typename F>
double HashTable<K, V, F>::get_load_factor() {
    return (double)size / table_size;
}


#endif // _HASHTABLE_H
