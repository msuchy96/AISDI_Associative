//
// Created by pczytanski on 04.12.16.
//

#ifndef AISDI_MAPS_BSTNODE_H
#define AISDI_MAPS_BSTNODE_H

#endif //AISDI_MAPS_BSTNODE_H
template<typename KeyType, typename ValueType>
struct BSTNode{
    BSTNode<KeyType, ValueType>* parent;
    BSTNode<KeyType, ValueType>* left;
    BSTNode<KeyType, ValueType>* right;
    std::pair<const KeyType, ValueType> pair;
    BSTNode(BSTNode* p):parent(p), left(nullptr), right(nullptr){
    }
    BSTNode():parent(nullptr),left(nullptr), right(nullptr){

    }
    BSTNode(KeyType k, ValueType v):parent(nullptr), left(nullptr), right(nullptr), pair(std::make_pair(k, v)){

    }


};
