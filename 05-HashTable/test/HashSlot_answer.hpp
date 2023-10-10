#ifndef _HASHSLOT_H
#define _HASHSLOT_H

#include <cstddef>

struct DefaultHash {
    unsigned long operator()(const int& k) const {
        return k;
    }

    unsigned long operator()(const std::string& k) const
    {
        unsigned long hash = 0;
        for (const char& c : k) {
            hash += (unsigned long)c;
        }
        return hash;
    }
};

/* Fill in the TODO sections in the following code. */

template <typename K, typename V>
class HashSlot
{
public:
    HashSlot() : _empty(true), _removed(false) {
    }

    K get_key() const {
        return _key;
    }

    V get_value() const {
        return _value;
    }

    void set_key_value(K key, V value) {
        // TODO
        this->_key = key;
        this->_value = value;
        this->_empty = false;
        this->_removed = false;
    }

    bool is_empty() const {
        // TODO                
        return this->_empty;
    }

    void set_empty() {
        // TODO        
        this->_empty = true;
    }

    bool is_removed() const {
        // TODO
        return this->_removed;
    }

    void set_removed() {
        // TODO
        this->_removed = true;
    }

private:
    // key-value pair
    K _key;
    V _value;
    bool _empty;
    bool _removed;

    // disallow copy and assignment
    HashSlot(const HashSlot&);
    HashSlot& operator=(const HashSlot&);
};


#endif // _HASHSLOT_H
