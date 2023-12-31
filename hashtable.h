#ifndef HASHTABLE_H
#define HASHTABLE_H

// names / uniquenames
// Connor Turco / cturco
//

// Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        buckets.resize(4);
        // TODO: a default constructor (possibly empty).

        // You can use the following to avoid needing to implement rehashing.
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehashing.
        //    buckets.resize(10000);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        if(2 * num_elements >= buckets.size()) {
            rehash_and_grow();
        }
        Hasher hasher;
        size_t hash_result = hasher(key);
        size_t desired_bucket = hash_result % buckets.size();
        
        //Perfect hash
        if(buckets[desired_bucket].status == Status::Empty) {
            V dummy;
            buckets[desired_bucket].val = dummy;
            buckets[desired_bucket].key = key;
            buckets[desired_bucket].status = Status::Occupied;
            ++num_elements;
            return buckets[desired_bucket].val;
        }
        
        int j = 0;
        size_t first_erased_index = 0;
        bool erased_found = false;
        //Move to next empty spot or where key is, whichever is first;
        do {
            desired_bucket = (hash_result + j) % buckets.size();
            ++j;
            if(buckets[desired_bucket].status == Status::Occupied
               && buckets[desired_bucket].key == key) {
                //Not adding element so don't need to ++
                return buckets[desired_bucket].val;
            }
            if(buckets[desired_bucket].status == Status::Deleted && !erased_found) {
                first_erased_index = desired_bucket;
                erased_found = true;
                
            }
            if(j > (int)buckets.size()) {
                break;
            }
        }
        while(buckets[desired_bucket].status != Status::Empty);
        
        //Couldn't find the key in there, putting in next empty or deleted spot
        if(erased_found) {
            desired_bucket = first_erased_index;
        }
        V dummy;

        buckets[desired_bucket].val = dummy;
        buckets[desired_bucket].key = key;
        buckets[desired_bucket].status = Status::Occupied;
        ++num_elements;
        return buckets[desired_bucket].val;
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        if(2 * num_elements >= buckets.size()) {
            rehash_and_grow();
        }
        Hasher hasher;
        size_t hash_result = hasher(key);
        size_t desired_bucket = hash_result % buckets.size();
        
        //Perfect hash
        if(buckets[desired_bucket].status == Status::Empty) {
            buckets[desired_bucket].val = val;
            buckets[desired_bucket].key = key;
            buckets[desired_bucket].status = Status::Occupied;
            ++num_elements;
            return true;
        }
        int j = 0;
        size_t first_erased_index = 0;
        bool erased_found = false;
        //Move to next empty spot or where key is, whichever is first;
        do {
            desired_bucket = (hash_result + j) % buckets.size();
            ++j;
            if(buckets[desired_bucket].status == Status::Occupied
               && buckets[desired_bucket].key == key) {
                //Not adding element so don't need to ++
                return false;
            }
            if(buckets[desired_bucket].status == Status::Deleted && !erased_found) {
                first_erased_index = desired_bucket;
                erased_found = true;
                
            }
            if(j > (int)buckets.size()) {
                break;
            }
        }
        while(buckets[desired_bucket].status != Status::Empty);
        
            //Couldn't find the key in there, putting in next empty or deleted spot
        if(erased_found) {
            desired_bucket = first_erased_index;
        }

        buckets[desired_bucket].val = val;
        buckets[desired_bucket].key = key;
        buckets[desired_bucket].status = Status::Occupied;
        ++num_elements;
        return true;

        // TODO
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        Hasher hasher;
        size_t hash_result = hasher(key);
        size_t desired_bucket;
        
        int j = 0;
        //Move to next empty spot or where key is, whichever is first;
        do {
            desired_bucket = (hash_result + j) % buckets.size();
            ++j;
            //Found the element
            if(buckets[desired_bucket].status == Status::Occupied
               && buckets[desired_bucket].key == key) {
                --num_elements;
                ++num_deleted;
                buckets[desired_bucket].status = Status::Deleted;
                return 1;
            }
            if(j > (int)buckets.size()) {
                break;
            }
        }
        while(buckets[desired_bucket].status != Status::Empty);
        
        return 0;
        // TODO
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use this to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        size_t old_num_elements = num_elements;
        std::vector<Bucket> old_vector;
        old_vector.swap(buckets);
        
        buckets.resize(2 * old_vector.size());
        for(auto it = old_vector.begin(); it != old_vector.end(); ++it) {
            if(it->status == Status::Occupied) {
                insert(it->key, it->val);
            }
        }
        num_elements = old_num_elements;
        // TODO (optional)
    }
    
    // You can add methods here if you like.
    // TODO
};

#endif // HASHTABLE_H
