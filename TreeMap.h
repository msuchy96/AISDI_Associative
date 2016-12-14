#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "TreeNode.h"
#include <queue>


namespace aisdi
{

    template <typename KeyType, typename ValueType>
    class TreeMap
    {

    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type&;
        using const_reference = const value_type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using node = TreeNode<key_type, mapped_type>;

        TreeMap(): root(nullptr), counter(0)  {}
        ~TreeMap()
        {
            deleteTree();
        }

        TreeMap(std::initializer_list<value_type> list) : TreeMap()
        {
            for(auto iterator=list.begin(); iterator!=list.end();iterator++)
            {
                auto newNode = creatingNewNode((*iterator).first);
                newNode->pair.second=(*iterator).second;
            }

        }
        TreeMap(const TreeMap& other) : TreeMap()
        {
            *this = other;
        }

        TreeMap(TreeMap&& other)
        {
            root=other.root;
            other.root=nullptr;
            counter=other.counter;
            other.counter=0;
            maximumKey=other.maximumKey;
            minimumKey=other.minimumKey;

        }

        TreeMap& operator=(const TreeMap& other)
        {

            if (&other != this)
            {
                deleteTree();
                std::queue<node*> tmpQueue;
                node* currentNode;
                tmpQueue.push(other.root);
                while(!tmpQueue.empty())
                {
                    currentNode = tmpQueue.front();
                    tmpQueue.pop();
                    if( currentNode != nullptr )
                    {
                        tmpQueue.push(currentNode->left_son);
                        tmpQueue.push(currentNode->right_son);
                        auto newNode = creatingNewNode((currentNode->pair).first);
                        newNode->pair.second=(currentNode->pair).second;
                    }
                }
            }
            return *this;
        }

        TreeMap& operator=(TreeMap&& other)
        {
            if(&other!=this) {

                deleteTree();
                root = other.root;
                other.root = nullptr;
                counter = other.counter;
                other.counter = 0;
                maximumKey = other.maximumKey;
                minimumKey = other.minimumKey;
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
            return (newNode->pair.second);
        }


        const mapped_type& valueOf(const key_type& key) const
        {
            auto constiter = find(key);
            if(constiter == end())
                throw std::out_of_range("Out Of Range");
            return (*constiter).second;

        }

        mapped_type& valueOf(const key_type& key)
        {
            auto iter = find(key);
            if(iter == end())
                throw std::out_of_range("Out Of Range");
            return (*iter).second;
        }



        const_iterator find(const key_type& key) const
        {
            if (counter == 0 || key > maximumKey || key < minimumKey)
                return ConstIterator(nullptr, this);
            node *tmp = root;
            while ((tmp != nullptr) && (tmp->pair.first != key))
                if (key < tmp->pair.first)
                    tmp = tmp->left_son;
                else
                    tmp = tmp->right_son;
            return ConstIterator(tmp, this);
        }

        iterator find(const key_type& key)
        {
            if (counter == 0 || key > maximumKey || key < minimumKey)
                return ConstIterator(nullptr, this);
            node *tmp = root;
            while ((tmp != nullptr) && (tmp->pair.first != key))
                if (key < tmp->pair.first)
                    tmp = tmp->left_son;
                else
                    tmp = tmp->right_son;
            return Iterator(tmp, this);

        }

        void remove(const key_type& key)
        {
            remove(find(key));
        }

        void remove(const const_iterator& it)
        {
            if(it == end())
                throw std::out_of_range("Out Of Range");
            auto current = it.currentNode;

            if (current->left_son == nullptr) /// nie ma dziecka lub ma tylko prawe
                replace(current, current->right_son);

            else if (current->right_son == nullptr) /// ma tylko lewe dziecko
                replace(current, current->left_son);

            else /// ma 2 dzieci
            {
                auto tmp = current->right_son;

                while (tmp->left_son != nullptr) /// tmp - następnik current
                    tmp = tmp->left_son;

                replace(tmp, tmp->right_son);
                replace(current, tmp);
            }



            if ( maximumKey == current->pair.first)
                setMax();

            if (minimumKey == current->pair.first )
                setMin();

            current->left_son = nullptr;
            current->right_son = nullptr;
            current->parent = nullptr;
            delete current;
            counter--;

        }

        void replace(node* delNode, node* repNode)
        {


            if (delNode == root)
                root = repNode;
            else if (delNode->parent->left_son == delNode)
                delNode->parent->left_son = repNode;
            else
                delNode->parent->right_son = repNode;
            if (repNode != nullptr)
                repNode->parent = delNode->parent;

        }

        size_type getSize() const
        {
            return counter;
        }

        bool operator==(const TreeMap& other) const
        {
            if (other.getSize() != getSize())
                return false;
            for (auto iterator = begin(); iterator != end(); iterator++)
            {
                if (other.valueOf((*iterator).first) != (*iterator).second)
                    return false;
            }
            return true;
        }

        bool operator!=(const TreeMap& other) const
        {
            return !(*this == other);
        }

        iterator begin()
        {
            return cbegin();
        }

        iterator end()
        {
            return cend();
        }

        const_iterator cbegin() const
        {
            if(counter==0)
                return cend();
            node* tmp=root;
            while((tmp->left_son)!=nullptr)
                tmp=tmp->left_son;
            return const_iterator(tmp, this);
        }

        const_iterator cend() const
        {
            return ConstIterator(nullptr, this);
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

        node *GetLastNode() const
        {
            auto last = root;
            while (last->right_son != nullptr)
                last = last->right_son;

            return last;
        }

        void deleteTree()
        {
            if(root==nullptr)
                return;
            deleteAllNodes(root->left_son);
            deleteAllNodes(root->right_son);
            counter=0;
            delete root;
            root=nullptr;

        }

        void deleteAllNodes(node *tmpNode)
        {
            if(tmpNode == nullptr) return;
            deleteAllNodes(tmpNode->left_son);
            deleteAllNodes(tmpNode->right_son);
            delete tmpNode;

        }

        void copyingNodes(const TreeMap& other)
        {
            for(auto iterator=other.begin(); iterator!=other.end();iterator++)
            {
                auto newNode = creatingNewNode((*iterator).first);
                newNode->pair.second=(*iterator).second;
            }

        }

        node *creatingNewNode(const key_type &key)
        {
            counter++;
            node *tmp = new TreeNode<key_type, mapped_type>(key, mapped_type{});

            if (root == nullptr)
            {
                root=tmp;
                maximumKey=minimumKey=key;
                return tmp;
            }

            node *parent_current = nullptr;
            node *current = root;
            while (current != nullptr)
            {
                parent_current = current;
                if (current->pair.first < key)
                    current = current->right_son;
                else
                    current = current->left_son;
            }

            if (parent_current->pair.first < key)
                parent_current->right_son = tmp;
            else
                parent_current->left_son = tmp;
            tmp->parent = parent_current;

            if (key > maximumKey)
                maximumKey = key;
            else if (key < minimumKey)
                minimumKey = key;


            return tmp;
        }

        void setMin()
        {
            if(root == nullptr)
                return;

            auto tmp=root;

            while(tmp->left_son != nullptr)
                tmp=tmp->left_son;

            minimumKey=tmp->pair.first;

        }

        void setMax()
        {
            if(root == nullptr)
                return;

            auto tmp=root;

            while(tmp->right_son != nullptr)
                tmp=tmp->right_son;

            maximumKey=tmp->pair.first;

        }

        node* root;
        size_t counter;
        key_type maximumKey;
        key_type minimumKey;


    };

    template <typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::ConstIterator
    {
    public:
        using reference = typename TreeMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename TreeMap::value_type;
        using pointer = const typename TreeMap::value_type*;
        using collection = const TreeMap<KeyType,ValueType>;
        using node = TreeNode<KeyType,ValueType>;
        explicit ConstIterator()
        {}

        ConstIterator(node *cN, collection *cC) : currentNode(cN), currentCollection(cC) {}

        ConstIterator(const ConstIterator& other) : currentNode(other.currentNode), currentCollection(other.currentCollection){}

        ConstIterator& operator++()
        {
            if(currentNode == nullptr)
                throw std::out_of_range("OUT OF RANGE 3: Can't increment null element!");
            if(currentNode->right_son == nullptr)
            {
                auto tmpParent=currentNode->parent;
                while(tmpParent!= nullptr && currentNode==tmpParent->right_son) // go to parent as long as he is a left son
                {
                    currentNode=tmpParent;
                    tmpParent=currentNode->parent;
                }
                currentNode=tmpParent;
            }
            else
            {
                currentNode=currentNode->right_son;
                while(currentNode->left_son!= nullptr)

                    currentNode=currentNode->left_son;
            }

            return *this;

        }

        ConstIterator operator++(int)
        {
            auto tmp=*this;
            operator++();
            return  tmp;

        }

        ConstIterator& operator--()
        {

            if(currentCollection->begin()==(*this))
                throw std::out_of_range("OUT OF RANGE 3: Can't decrement root");
            if(currentNode == nullptr)
            {
                currentNode=currentCollection->GetLastNode();
                return *this;
            }

            if(currentNode->left_son == nullptr)
            {
                auto tmpParent=currentNode->parent;
                while(tmpParent!= nullptr && currentNode==tmpParent->left_son) // go to parent as long as he is a right son
                {
                    currentNode=tmpParent;
                    tmpParent=currentNode->parent;
                }
                currentNode=tmpParent;
            }
            else
            {
                currentNode=currentNode->left_son;
                while(currentNode->right_son != nullptr)

                    currentNode=currentNode->right_son;
            }

            return *this;

        }

        ConstIterator operator--(int)
        {
            auto tmp=(*this);
            operator--();
            return tmp;
        }

        reference operator*() const
        {
            if (currentNode==nullptr)
                throw std::out_of_range("OUT OF RANGE 1: You're trying to get pair of empty object ");
            return currentNode->pair;
        }

        pointer operator->() const
        {
            return &this->operator*();
        }

        bool operator==(const ConstIterator& other) const
        {
            return (other.currentNode==(this->currentNode));
        }

        bool operator!=(const ConstIterator& other) const
        {
            return !(*this == other);
        }


    private:
        node* currentNode;
        collection* currentCollection;
        friend void TreeMap<KeyType, ValueType>::remove(const const_iterator&);

    };

    template <typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
    {
    public:
        using reference = typename TreeMap::reference;
        using pointer = typename TreeMap::value_type*;
        using collection = const TreeMap<KeyType,ValueType>;
        using node = TreeNode<KeyType,ValueType>;

        explicit Iterator()
        {}


        Iterator(node *cN,collection *cC) : ConstIterator(cN,cC) {}


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

#endif /* AISDI_MAPS_MAP_H */
