#ifndef UTHASH_HPP
#define UTHASH_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// self implementation of pair
#include "pair.hpp"

// freestanding C++ support
#include <initializer_list>
#include <new>

#include "uthash.h"

namespace Misc
{

template <typename Key, typename T>
class UTHash
{

private:

    struct HashTable
    {
        Key key;
        T val;
        UT_hash_handle hh;
    } *hashtable;

    // used for begin(), next()
    HashTable *iterator;
    Misc::pair<Key, T> *pair_alloc;

public:

    UTHash() : hashtable(NULL), iterator(NULL), pair_alloc(NULL)
    {
    }

    UTHash(std::initializer_list<Misc::pair<Key, T>> ilist) : UTHash()
    {
        for (auto it = ilist.begin(); it != ilist.end(); it++) {
            insert(it->first, it->second);
        }
    }

    ~UTHash()
    {
        clear();
        free(pair_alloc);
    }

    T &at(const Key &key)
    {
        while (true) {
            HashTable *item = NULL;
            HASH_FIND(hh, hashtable, &key, sizeof(Key), item);

            // create the item if it doesn't exist
            if (!item) {
                insert(key, T());
                continue; // find the item again
            }

            return item->val;
        }
    }

    bool empty()
    {
        return size() == 0;
    }

    void clear()
    {
        HashTable *item, *tmp;

        HASH_ITER(hh, hashtable, item, tmp) {
            erase(item->key);
        }
    }
    
    void insert(const Key &key, const T &val)
    {
        HashTable *item = NULL;
        HASH_FIND(hh, hashtable, &key, sizeof(Key), item);
    
        if (!item) {
            item = (HashTable*) malloc(sizeof(HashTable));
            item->key = key;
            HASH_ADD(hh, hashtable, key, sizeof(Key), item);
        }
    
        item->val = val;
    }
    
    void erase(const Key &key)
    {
        HashTable *item = NULL;
        HASH_FIND(hh, hashtable, &key, sizeof(Key), item);

        if (item) {
            HASH_DEL(hashtable, item);
            free(item);
        }
    }
    
    int size()
    {
        return HASH_COUNT(hashtable);
    }

    bool exists(const Key &key)
    {
        HashTable *item = NULL;
        HASH_FIND(hh, hashtable, &key, sizeof(Key), item);
        return item;
    }

    Misc::pair<Key, T> *begin()
    {
        if (hashtable == NULL) return NULL;

        iterator = hashtable;
        
        // create Misc::pair inside heap
        free(pair_alloc); pair_alloc = NULL;
        void *buf = malloc(sizeof(Misc::pair<Key, T>));
        pair_alloc = new (buf) Misc::pair<Key, T>(iterator->key, iterator->val);
        
        return pair_alloc;
    }

    Misc::pair<Key, T> *next()
    {
        if (iterator == NULL) return NULL;
        if (iterator->hh.next == NULL) return NULL;

        iterator = (HashTable*) iterator->hh.next;

        // create Misc::pair inside heap
        free(pair_alloc); pair_alloc = NULL;
        void *buf = malloc(sizeof(Misc::pair<Key, T>));
        pair_alloc = new (buf) Misc::pair<Key, T>(iterator->key, iterator->val);

        return pair_alloc;
    }

    T &operator[](const Key &key)
    {
        return at(key);
    }

};

template <typename T>
class UTHash_StrKey
{

private:

    struct HashTable
    {
        const char *key;
        T           val;
        UT_hash_handle hh;
    } *hashtable;

    // used for begin(), next()
    HashTable *iterator;
    Misc::pair<const char *, T> *pair_alloc;

public:

    UTHash_StrKey() : hashtable(NULL), iterator(NULL), pair_alloc(NULL)
    {
    }

    UTHash_StrKey(std::initializer_list<Misc::pair<const char *, T>> ilist) : UTHash_StrKey()
    {
        for (auto it = ilist.begin(); it != ilist.end(); it++) {
            insert(it->first, it->second);
        }
    }

    ~UTHash_StrKey()
    {
        clear();
        free(pair_alloc);
    }

    T &at(const char *key)
    {
        while(true) {
            HashTable *item = NULL;
            HASH_FIND_STR(hashtable, key, item);

            // create the item if it doesn't exist
            if (!item) {
                insert(key, T());
                continue; // find the item again
            }

            return item->val;
        }
    }

    bool empty()
    {
        return size() == 0;
    }

    void clear()
    {
        HashTable *item, *tmp;

        HASH_ITER(hh, hashtable, item, tmp) {
            free((char *)item->key);
            erase(item->key);
        }
    }
    
    void insert(const char *key, const T &val)
    {
        HashTable *item = NULL;
        HASH_FIND_STR(hashtable, key, item);
    
        if (!item) {

            // create a local copy of string
            int size = strlen(key);
            char *key_new = (char *) malloc(size+1);
            snprintf(key_new, size+1, "%s", key);
            
            item = (HashTable*) malloc(sizeof(HashTable));
            item->key = key_new;
            HASH_ADD_KEYPTR(hh, hashtable, item->key, size, item);
        }
    
        item->val = val;
    }
    
    void erase(const char *key)
    {
        HashTable *item = NULL;
        HASH_FIND_STR(hashtable, key, item);

        if (item) {
            HASH_DEL(hashtable, item);
            free(item);
        }
    }
    
    int size()
    {
        return HASH_COUNT(hashtable);
    }

    bool exists(const char *key)
    {
        HashTable *item = NULL;
        HASH_FIND_STR(hashtable, key, item);
        return item;
    }

    Misc::pair<const char *, T> *begin()
    {
        if (hashtable == NULL) return NULL;

        iterator = hashtable;
        
        // create Misc::pair inside heap
        free(pair_alloc); pair_alloc = NULL;
        void *buf = malloc(sizeof(Misc::pair<const char *, T>));
        pair_alloc = new (buf) Misc::pair<const char *, T>(iterator->key, iterator->val);
        
        return pair_alloc;
    }

    Misc::pair<const char *, T> *next()
    {
        if (iterator == NULL || iterator->hh.next == NULL) return NULL;

        iterator = (HashTable*) iterator->hh.next;

        // create Misc::pair inside heap
        free(pair_alloc); pair_alloc = NULL;
        void *buf = malloc(sizeof(Misc::pair<const char *, T>));
        pair_alloc = new (buf) Misc::pair<const char *, T>(iterator->key, iterator->val);

        return pair_alloc;
    }

    T &operator[](const char *key)
    {
        return at(key);
    }

};

};

#endif /* UTHASH_HPP */