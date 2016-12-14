#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "HashNode.h"
#include <ostream>
#include <iostream>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
    using hash_node = HashNode<key_type, mapped_type>;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;


  HashMap() : hash_table(nullptr), counter(0)
  {
    hash_table = new hash_node* [TABLE_SIZE]{};
    for (size_type i = 0; i < TABLE_SIZE; ++i)
      hash_table[i] = nullptr;
  }

  HashMap(std::initializer_list<value_type> list) : HashMap()
  {
      for(auto iterator=list.begin(); iterator!=list.end();iterator++)
      {
          auto newNode = creatingNewNode((*iterator).first);
          increaseCounter();
          newNode->pair.second=(*iterator).second;
      }
  }

  HashMap(const HashMap& other) : HashMap()
  {
      *this = other;
  }

  HashMap(HashMap&& other) : HashMap()
  {
      hash_node **atcual = hash_table;

      counter = other.counter;
      hash_table = other.hash_table;
      other.counter=0;
      other.hash_table=atcual;
  }

  HashMap& operator=(const HashMap& other)
  {
    if(&other != this)
    {
        deleteHashMap();
        for(auto iterator=other.begin(); iterator!=other.end();iterator++)
        {
            auto newNode = creatingNewNode((*iterator).first);
            increaseCounter();
            newNode->pair.second=(*iterator).second;
        }
    }
      return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
      if(&other != this)
      {
          deleteHashMap();
          hash_node **actual = hash_table;

          counter = other.counter;
          hash_table = other.hash_table;
          other.counter=0;
          other.hash_table=actual;

      }
      return *this;
  }

  bool isEmpty() const
  {
    return (counter==0);
  }

  mapped_type& operator[](const key_type& key)
  {
    auto iter=find(key);
    if(iter != end())
      return (*iter).second;
    auto newNode = creatingNewNode(key);
      increaseCounter();
    return (newNode->pair.second);
  }

  const mapped_type& valueOf(const key_type& key) const
  {
      hash_node* currentNode = searchNode(key);
      if(currentNode == nullptr)
          throw std::out_of_range("Out Of Range: Can't take value. There is no such a node.");
      return  currentNode->pair.second;
  }

  mapped_type& valueOf(const key_type& key) /// funckja ta nie jest uzywana w testach
  {
      hash_node* currentNode = searchNode(key);
      if(currentNode == nullptr)
          throw std::out_of_range("Out Of Range: Can't take value. There is no such a node.");
      return  currentNode->pair.second;
  }


  const_iterator find(const key_type& key) const
  {
    return ConstIterator(this,searchNode(key),hashFunction(key));
  }

  iterator find(const key_type& key)
  {
    return Iterator(this,searchNode(key),hashFunction(key));
  }

  void remove(const key_type& key)
  {
    remove(find((key)));
  }

  void remove(const const_iterator& it)
  {
    if(it == end())
        throw std::out_of_range("Out Of Range: Can't remove null element");
      auto current= it.currentHash_Node;
      if(current->prev == nullptr) /// first element
      {
          hash_table[hashFunction(current->pair.first)] = current->next;

      }
      else
          current->prev->next=current->next;

      if(current->next != nullptr)
          current->next->prev = current->prev;

      current->prev = nullptr;
      current->next = nullptr;
      counter--;
      delete current;


  }

  size_type getSize() const
  {
    return counter;
  }

  bool operator==(const HashMap& other) const
  {
    if(other.counter != counter)
        return false;
      auto iter_other = other.begin();
      for(auto iter_this = begin() ; iter_this!=end(); iter_this++)
      {
          if(*iter_this != *iter_other)
              return false;
          iter_other++;
      }
      return true;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {

      size_type index = FirstNotEmptyRecord();

      return Iterator(this, hash_table[index] , index);
  }

  iterator end()
  {
    return Iterator(this, nullptr,TABLE_SIZE);
  }



  const_iterator cbegin() const
  {
      size_type index = FirstNotEmptyRecord();

      return ConstIterator(this,hash_table[index],index);
  }

  const_iterator cend() const
  {
    return ConstIterator(this, nullptr, TABLE_SIZE);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
private:

    hash_node* searchNode(const key_type& key) const
    {
      hash_node *currentNode = hash_table[hashFunction(key)];
      while(currentNode != nullptr)
      {
        if(currentNode->pair.first == key)
          return currentNode;
          currentNode=currentNode->next;
      }
      return currentNode;
    }

     size_type FirstNotEmptyRecord() const
    {
        size_type index = 0;

        while(hash_table[index] == nullptr && index != TABLE_SIZE)
            index++;

        return  index;
    }


    size_type hashFunction(const key_type& key) const
    {
      return std::hash<key_type>()(key) % TABLE_SIZE;
    }

    hash_node* creatingNewNode(const key_type& key) const
    {

      size_type hashKey = hashFunction(key);
      hash_node *currentNode = hash_table[hashKey];

      if(currentNode == nullptr)
      {
        hash_table[hashKey] = new HashNode<key_type, mapped_type>(key, mapped_type{});
        currentNode = hash_table[hashKey];
      }
      else
      {
        while(currentNode->next != nullptr )
          currentNode=currentNode->next;

        currentNode->next = new HashNode<key_type, mapped_type>(key, mapped_type{}, currentNode);
        currentNode=currentNode->next;
      }

      return currentNode;
    }

    void increaseCounter()
    {
        counter++;
    }

    void deleteHashMap()
    {
        if(counter)
        {
            for(size_type i=0; i<TABLE_SIZE; i++)

                if (hash_table[i] != nullptr)
                    deleteHashList(i);

        }

        counter=0;
    }

    void deleteHashList(size_type i)
    {
        hash_node *tmp;
        hash_node *tmp2;
        tmp=hash_table[i];
        while(tmp != nullptr)
        {
            tmp2=tmp->next;
            tmp->prev= nullptr;
            tmp->next= nullptr;
            delete tmp;
            tmp=tmp2;
        }

    }

    static const size_type TABLE_SIZE = 100;
    hash_node **hash_table;
    size_type counter;
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
    using hash_node = HashNode<key_type, mapped_type>;

  explicit ConstIterator() {}



    ConstIterator(const HashMap *cM = nullptr, hash_node *cHN = nullptr, size_type index = 0 ) : currentMap(cM), currentHash_Node(cHN), index(index)
    {
      if(currentMap != nullptr && currentHash_Node == nullptr)
        this->index=currentMap->TABLE_SIZE; /// end
    }


  ConstIterator(const ConstIterator& other) : ConstIterator(other.currentMap, other.currentHash_Node, other.index)
  {}

  ConstIterator& operator++()
  {
    if(currentHash_Node == nullptr || currentMap == nullptr)
        throw std::out_of_range("Out Of Range: Can't increment from last element or map is empty");

      if(currentHash_Node->next != nullptr)
       currentHash_Node=currentHash_Node->next;
      else
      {
          index++;
        while(currentMap->hash_table[index] == nullptr && index != currentMap->TABLE_SIZE)
            index++;

          if(index != currentMap->TABLE_SIZE)
              currentHash_Node = currentMap->hash_table[index];

          else
              currentHash_Node = nullptr;
      }
      return *this;
  }

  ConstIterator operator++(int)
  {
      auto tmp = *this;
    operator++();
      return tmp;
  }

  ConstIterator& operator--()
  {
      if(currentMap == nullptr)
          throw std::out_of_range("Out Of Range: Can't decrement - map is empty");

      if(currentHash_Node == currentMap->hash_table[index]) /// when end to
      {
        index--;
          while(index > 0 && currentMap->hash_table[index] == nullptr)
              index--;

          if(index == 0 && currentMap->hash_table[index] == nullptr)
              throw std::out_of_range("Out Of Range");

          currentHash_Node=currentMap->hash_table[index];

          while(currentHash_Node->next != nullptr)
              currentHash_Node=currentHash_Node->next;

      }
      else
          currentHash_Node=currentHash_Node->prev;


      return *this;
  }

  ConstIterator operator--(int)
  {

      auto tmp = *this;
      operator--();
      return tmp;

  }

  reference operator*() const
  {
    if(currentHash_Node == nullptr)
        throw std::out_of_range("Out Of Range: Can't take value from NULL element");
      return currentHash_Node->pair;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (currentMap == other.currentMap && index == other.index && currentHash_Node == other.currentHash_Node );
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
private:
    const HashMap *currentMap;
    hash_node *currentHash_Node;
    size_type index;
    friend void HashMap<KeyType, ValueType>::remove(const const_iterator&);
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;
    using hash_node = HashNode<key_type, mapped_type>;

  explicit Iterator()
  {}
    Iterator(const HashMap *HM, hash_node *HN, size_type index) : ConstIterator(HM, HN, index) {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
