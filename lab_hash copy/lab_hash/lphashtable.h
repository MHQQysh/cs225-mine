/**
 * @file lphashtable.h
 * Definition of a Linear Probing Hash Table.
 */

#pragma once

#include "hashtable.h"

/**
 * A HashTable implementation that uses linear probing as a collision
 * resolution strategy.
 */
template <class K, class V>
class LPHashTable : public HashTable<K, V>







{
  private:
    // so we can refer to hash, elems, and size directly, and use the
    // makeIterator function without having to scope it.
    using HashTable<K, V>::elems;
    using HashTable<K, V>::size;
    using HashTable<K, V>::makeIterator;
    using HashTable<K, V>::findPrime;
    using HashTable<K, V>::shouldResize;

    // implementation for our iterator, you don't need to worry about
    // this
    class LPIteratorImpl;







/* 
这里的意思是

*/

  public:

  /* 第一个 */
/* // we use HashTable's iterators here: 这是一个注释，说明 LPHashTable 类将使用另一个名为 HashTable 的类提供的迭代器功能。这暗示了 LPHashTable 和 HashTable 之间可能存在某种关系，比如继承或者组合。 */


/* typedef typename HashTable<K, V>::iterator iterator;: 这一行定义了一个类型别名。
typedef: 这个关键字用于为现有的类型创建一个新的名称（别名）。
typename HashTable<K, V>::iterator: 这指定了 iterator 将要代表的现有类型。


HashTable<K, V>: 这表明存在一个名为 HashTable 的类，它很可能是一个模板类，通过两个类型参数 K（很可能是键的类型）和 V（很可能是存储在哈希表中的值的类型）进行参数化。

::iterator: 这表示 HashTable 类内部定义了一个名为 iterator 的嵌套类型。这在 C++ 中是一种常见的做法，用于提供一种遍历容器类（如哈希表）中元素的方法。

iterator: 这是新定义的别名。现在，在 LPHashTable 类的作用域内，你可以使用 iterator 作为 typename HashTable<K, V>::iterator 的简写。这使得代码在处理底层 HashTable 的迭代器时更易读且更简洁


*/





  typedef typename HashTable<K,T>::iterator iterator;




    /**
     * Constructs a LPHashTable of the given size.
     *
     * @param tsize The desired number of starting cells in the
     *  LPHashTable.
     */
    LPHashTable(size_t tsize);

    /**
     * Destructor for the LPHashTable. We use dynamic memory, and thus
     * require the big three.
     */
    virtual ~LPHashTable();
















    /**
     * Assignment operator.
     *
     * @param rhs The LPHashTable we want to assign into the current
     *  one.
     * @return A const reference to the current LPHashTable.
     */
    const LPHashTable<K, V>& operator=(const LPHashTable<K, V>& rhs);






    /**
     * Copy constructor.
     *
     * @param other The LPHashTable to be copied.
     */
    LPHashTable(const LPHashTable<K, V>& other);

    // functions inherited from HashTable
    virtual void insert(const K& key, const V& value);
    virtual void remove(const K& key);
    virtual V find(const K& key) const;
    virtual bool keyExists(const K& key) const;
    virtual void clear();
    virtual V& operator[](const K& key);

    iterator begin() const
    {
        return makeIterator(new LPIteratorImpl(*this, 0));
    }

    iterator end() const
    {
        return makeIterator(new LPIteratorImpl(*this, size));
    }

  private:
    /**
     * Storage for our LPHashTable.
     *
     * With linear probing, we only need the array, not buckets for each
     * array index. Note that we use an array of pointers to pairs in this
     * case since the check for an "empty" slot is simply a check against
     * NULL in that cell.
     */
    std::pair<K, V>** table;

    /**
     * Flags for whether or not to probe forward when looking at a
     * particular cell in the table.
     *
     * This is a dynamic array of booleans that represents if a slot is
     * (or previously was) occupied. This allows us determine whether
     * or not we need to probe forward to look for our key.
     */
    bool* should_probe;

    /**
     * Helper function to determine the index where a given key lies in
     * the LPHashTable. If the key does not exist in the table, it will
     * return -1.
     *
     * @param key The key to look for.
     * @return The index of this key, or -1 if it was not found.
     */
    int findIndex(const K& key) const;

    // inherited from HashTable
    virtual void resizeTable();
};

#include "lpiterator.h"
#include "lphashtable.cpp"
