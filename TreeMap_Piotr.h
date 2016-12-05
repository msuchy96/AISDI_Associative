#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "BSTNode.h"

namespace aisdi {

    template<typename KeyType, typename ValueType>
    class TreeMap {
        friend class ConstIterator;

        friend class Iterator;

    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using tree_node = BSTNode<key_type, mapped_type>;

        class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        ~TreeMap() {
            removeAll();
        }

        TreeMap() : root(nullptr), nullNode(nullptr), count(0) {}

        TreeMap(std::initializer_list<value_type> list) : TreeMap() {
            for (auto it = list.begin(); it != list.end(); ++it) {
                operator[]((*it).first) = (*it).second;
            }
        }

        TreeMap(const TreeMap &other) : TreeMap() {
            for (auto it = other.begin(); it != other.end(); ++it) {
                operator[]((*it).first) = (*it).second;
            }
        }

        TreeMap(TreeMap &&other) {
            count = other.count;
            root = other.root;
            minKey = other.minKey;
            maxKey = other.maxKey;
            other.count = 0;
            other.root = nullptr;

        }

        TreeMap &operator=(const TreeMap &other) {
            if (this == &other)return *this;
            removeAll();
            for (auto it = other.begin(); it != other.end(); ++it) {
                operator[]((*it).first) = (*it).second;
            }
            return *this;
        }

        TreeMap &operator=(TreeMap &&other) {
            count = other.count;
            root = other.root;
            minKey = other.minKey;
            maxKey = other.maxKey;
            other.count = 0;
            other.root = nullptr;
            return *this;
        }

        bool isEmpty() const {
            return (count == 0);
        }

        mapped_type &operator[](const key_type &key) {
            auto it = find(key);
            if (it != end()) return (*it).second;
            auto newNode = createNode(key);
            return (newNode->pair.second);
        }

        tree_node *createNode(const key_type &key) {
            tree_node *tmp = new BSTNode<key_type, mapped_type>(key, mapped_type{});

            count++;
            if (root == nullptr) {
                minKey = maxKey = key;
                root = tmp;
                return root;
            }
            if (key > maxKey)maxKey = key;
            else if (key < minKey)minKey = key;
            tree_node *y = nullptr;
            tree_node *x = root;
            while (x != nullptr) {
                y = x;
                if (key < x->pair.first)
                    x = x->left;
                else
                    x = x->right;
            }
            tmp->parent = y;
            if (key < y->pair.first)
                y->left = tmp;
            else
                y->right = tmp;
            return tmp;
        }

        const mapped_type &valueOf(const key_type &key) const {
            auto tmp = root;
            while ((tmp != nullptr) && (tmp->pair.first != key))
                if (key < tmp->pair.first)
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            if (tmp == nullptr) throw std::out_of_range("OUT OF RANGE");
            return tmp->pair.second;
        }

        mapped_type &valueOf(const key_type &key) {
            auto tmp = root;
            while ((tmp != nullptr) && (tmp->pair.first != key))
                if (key < tmp->pair.first)
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            if (tmp == nullptr) throw std::out_of_range("OUT OF RANGE");
            return tmp->pair.second;
        }

        const_iterator find(const key_type &key) const {
            if (count == 0 || (key > maxKey || key < minKey)) return ConstIterator(nullptr, this);
            tree_node *tmp = root;
            while ((tmp != nullptr) && (tmp->pair.first != key))
                if (key < tmp->pair.first)
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            return ConstIterator(tmp, this);

        }

        iterator find(const key_type &key) {
            if (count == 0 || (key > maxKey || key < minKey)) return ConstIterator(nullptr, this);
            tree_node *tmp = root;
            while ((tmp != nullptr) && (tmp->pair.first != key))
                if (key < tmp->pair.first)
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            return Iterator(tmp, this);

        }

        void removeAll() {
            if (root == nullptr) return;
            count = 0;
            removeRecursivly(root->left);
            removeRecursivly(root->right);

            delete root;
            root = nullptr;

        }

        void removeRecursivly(tree_node *node) {
            if (node == nullptr) return;
            removeRecursivly(node->left);
            removeRecursivly(node->right);
            delete node;
        }

        void remove(const key_type &key) {
            remove(find(key));
        }

        void remove(const const_iterator &it) {
            if (it == end()) throw std::out_of_range("OUT OF RANGE");
            auto node = it.node;
            if (node->left == nullptr) {
                //w miejsce usuwanego wezla wstawiane jego prawe poddrzewo
                transplant(node, node->right);

            } else if (node->right == nullptr) {
                //w miejsce usuwanego wezla wstawiane jego lewe poddrzewo
                transplant(node, node->left);

            } else {//usuwany wezel ma 2 dzieci
                auto tmp = node->right;
                //wyszukiwanie kolejnego elementu wzgledem usuwanego
                while (tmp->left != nullptr)
                    tmp = tmp->left;
                //nie jest bezposrednio pod usuwanym
                if (tmp->parent != node) {

                    transplant(tmp, tmp->right);
                    tmp->right = node->right;
                    tmp->right->parent = tmp;
                }
                //w miejsce usuwanego wezla wstawiany jego nastepnik
                transplant(node, tmp);
                tmp->left = node->left;
                tmp->left->parent = tmp;

            }
            count--;
            if (node->pair.first == minKey) setNewMin();
            if (node->pair.first == maxKey) setNewMax();
            delete node;
        }

        size_type getSize() const {
            return count;
        }

        bool operator==(const TreeMap &other) const {
            if (getSize() != other.getSize()) return false;
            for (auto it = begin(); it != end(); ++it) {
                if (other.valueOf((*it).first) != (*it).second) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const TreeMap &other) const {
            return !(*this == other);
        }

        iterator begin() {
            return iterator(cbegin());
        }

        iterator end() {
            return iterator(cend());
        }

        const_iterator cbegin() const {
            if (count == 0)return cend();
            return const_iterator(root, this);
        }

        const_iterator cend() const {
            return ConstIterator(nullptr, this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        tree_node *GetLastNode() const {
            auto last = root;
            auto tmp = root->right;
            while (tmp != nullptr) {
                last = tmp;
                tmp = last->right;
            }
            return last;
        }

        void transplant(tree_node *first, tree_node *second) {
            if (first->parent == nullptr)
                root = second;
            else if (first == first->parent->left)
                first->parent->left = second;
            else
                first->parent->right = second;
            if (second != nullptr)
                second->parent = first->parent;
        }

        void setNewMin() {
            if (root == nullptr)return;
            auto tmp = root;

            //wyszukiwanie kolejnego elementu wzgledem usuwanego
            while (tmp->left != nullptr)
                tmp = tmp->left;
            minKey = tmp->pair.first;
        }

        void setNewMax() {
            if (root == nullptr)return;
            auto tmp = root;
            //wyszukiwanie kolejnego elementu wzgledem usuwanego
            while (tmp->right != nullptr)
                tmp = tmp->right;
            maxKey = tmp->pair.first;
        }

        tree_node *root;
        tree_node *nullNode;
        size_t count;
        key_type maxKey;
        key_type minKey;
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::ConstIterator {
        friend class TreeMap;

    public:
        using reference = typename TreeMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename TreeMap::value_type;
        using pointer = const typename TreeMap::value_type *;
        using tree_node = BSTNode<KeyType, ValueType>;
        using collection = const TreeMap<KeyType, ValueType>;

        explicit ConstIterator() {}

        ConstIterator(tree_node *n, collection *m) : node(n), map(m) {

        }

        ConstIterator(const ConstIterator &other) : node(other.node), map(other.map) {

        }

        ConstIterator &operator++() {
            if (node == nullptr) throw std::out_of_range("OUT OF RANGE");
            auto tmp = node;
            auto tmp2 = node;
            if (node->right != nullptr) {
                tmp = node->right;
                tmp2 = tmp->left;

                while (tmp2 != nullptr) {
                    tmp = tmp2;
                    tmp2 = tmp->left;
                }
                node = tmp;
                return *this;

            }

            auto parentOfTmp = node->parent;
            while (parentOfTmp != nullptr && tmp == parentOfTmp->right) {
                tmp = parentOfTmp;
                parentOfTmp = tmp->parent;
            }
            node = parentOfTmp;
            return *this;

        }

        ConstIterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        ConstIterator &operator--() {
            if (*this == map->begin()) throw std::out_of_range("OUT OF RANGE");
            if (node == nullptr) {
                node = map->GetLastNode();
                return *this;
            }
            auto tmp = node;
            auto tmp2 = node;
            if (node->left != nullptr) {
                tmp = node->left;
                tmp2 = tmp->right;

                while (tmp2 != nullptr) {
                    tmp = tmp2;
                    tmp2 = tmp->right;
                }
                node = tmp;
                return *this;
            }
            auto parentOfTmp = node->parent;
            while (parentOfTmp != nullptr && tmp == parentOfTmp->left) {
                tmp = parentOfTmp;
                parentOfTmp = tmp->parent;
            }
            node = parentOfTmp;
            return *this;

        }

        ConstIterator operator--(int) {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        reference operator*() const {
            if (node == nullptr) throw std::out_of_range("OUT OF RANGE");
            return node->pair;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const {
            return this->node == other.node;
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }

    private:
        tree_node *node;
        collection *map;

    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator {
        friend class TreeMap;

    public:
        using reference = typename TreeMap::reference;
        using pointer = typename TreeMap::value_type *;
        using tree_node = BSTNode<KeyType, ValueType>;
        using collection = const TreeMap<KeyType, ValueType>;

        explicit Iterator() {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other) {}

        Iterator(tree_node *n, collection *c) : ConstIterator(n, c) {

        }

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif /* AISDI_MAPS_MAP_H */
