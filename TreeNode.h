#ifndef AISDI_MAPS_TREENODE_H
#define AISDI_MAPS_TREENODE_H

#endif //AISDI_MAPS_TREENODE_H
template<typename KeyType, typename ValueType>
struct TreeNode
{
    TreeNode<KeyType, ValueType>* parent;
    TreeNode<KeyType, ValueType>* left_son;
    TreeNode<KeyType, ValueType>* right_son;
    std::pair<const KeyType, ValueType> pair;
    TreeNode():parent(nullptr),left_son(nullptr), right_son(nullptr) {}
    TreeNode(KeyType key, ValueType value) : parent(nullptr), left_son(nullptr), right_son(nullptr), pair(std::make_pair(key, value)) {}
    ~TreeNode() {}

};