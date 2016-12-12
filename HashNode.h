#ifndef AISDI_MAPS_HASHNODE_H
#define AISDI_MAPS_HASHNODE_H


#endif //AISDI_MAPS_HASHNODE_H

template <typename KeyType, typename ValueType>
struct HashNode
{

    HashNode *next;
    HashNode *prev;
    std::pair<const KeyType, ValueType> pair;
    HashNode(KeyType key, ValueType mapped) :  next(nullptr), prev(nullptr), pair(std::make_pair( key, mapped )) {}
    HashNode(KeyType key, ValueType mapped, HashNode *prev) : HashNode(key, mapped)
    {
        this->prev = prev;
    }

    ~HashNode()
   {
       delete next;
   }
};