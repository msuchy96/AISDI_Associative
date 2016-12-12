#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "HashNode.h"

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

  HashMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO 1");
  }

  HashMap(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO 2");
  }

  HashMap(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO 3");
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO 4");
  }

  HashMap& operator=(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO 5");
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
    counter++;
    return (newNode->pair.second);
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO 8");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO 9");
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
    (void)key;
    throw std::runtime_error("TODO 12");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO 13");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO 14");
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO 15");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    throw std::runtime_error("TODO 16");
  }

  iterator end()
  {
    return Iterator(this);
  }

  const_iterator cbegin() const
  {
    throw std::runtime_error("TODO 18");
  }

  const_iterator cend() const
  {
    return ConstIterator(this);
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

      }
      return currentNode;
    }

    size_type hashFunction(const key_type& key) const
    {
      return std::hash<key_type>()(key) % TABLE_SIZE;
    }

    hash_node* creatingNewNode(const key_type& key) const
    {
      /// nie znalazło node'a o podanym kluczu
      /// chcemy zwrocic node'a o danym kluczu tworzac go w danym miejscu w tablicy hashującjej w tym celu:
      /// wejsc w tablicy miejsce o danym indeksie funkcja haszujaca
      /// jesli dany element jest nullptr dodac nasz element do tego miejsca
      /// jesli nie jest nullptr pojsc do konca listy i wrzucic nasz element

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
    throw std::runtime_error("TODO 21");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO 22");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO 23");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO 24");
  }

  reference operator*() const
  {
    throw std::runtime_error("TODO 25 ");
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
