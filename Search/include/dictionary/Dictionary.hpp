//
// Created by pospali on 18.12.2020.
//

#ifndef ATD_DICTIONARY_HPP
#define ATD_DICTIONARY_HPP

template <typename K, typename V>
class Dictionary final {
    // Auxiliary Classes
    enum Color {
        RED,
        BLACK,
    };
    struct Node final {
        K key;
        V value;
        Color color;
        Node *parent, *left, *right;

        Node(Node *parent = nullptr, Node *left = nullptr, Node *right = nullptr)
        :parent(parent), left(left), right(right) {}

        Node(const Node& node)
        :parent(node.parent), left(node.left), right(node.right) {
            new (&key) K(node.k);
            new (&value) V(node.value);
            color = node.color;
        }

        Node& operator =(const Node& node) {
            if (&node == this) return *this;
            parent = node.parent;
            left = node.left;
            right = node.right;
            key.~K();
            value.~value;
            new (&key) K(node.k);
            new (&value) V(node.value);
            color = node.color;
            return *this;
        }
        ~Node() {
            key.~K();
            value.~V();
        }

        void setNode(const K &key, const V &value, Color color = RED) {
            new (&this->key) K(key);
            new (&this->value) V(value);
            this->color = color;
        }
    };
public:
    class Iterator {
        Dictionary<K, V> *dictionary;
        Node *current_node;
    public:
        Iterator(Dictionary<K, V> *dictionary)
        :dictionary(dictionary),
        current_node(dictionary->treeMinimum(dictionary->root)) {}

        Iterator(const Iterator &iterator2)
        :dictionary(iterator2.dictionary),
        current_node(iterator2.current_node) {}

        Iterator &operator =(const Iterator& iterator2) {
            if (&iterator2 == this) return *this;
            dictionary = iterator2.dictionary;
            current_node = iterator2.current_node;
            return *this;
        }

        const K& key() const;
        const V& get() const;
        void set( const V& value);
        void next();
        void prev();
        bool hasNext() const;
        bool hasPrev() const;
    };

// Data
private:
   Node *root;
   int _size;

// Methods
public:
    int size() const;

    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary<K, V>&) = delete;

    Dictionary<K, V>& operator =(const Dictionary<K, V>&) = delete;
    const V& operator [](const K&) const;
    V& operator [](const K&);

    void put(const K&, const V&);
    void remove(const K&);
    bool contains(const K&);

    Iterator iterator() {return Iterator(this);}
    const Iterator iterator() const {return Iterator(this);}

    void print() {print(root);}
private:
    void print(Node *node, int level = 0) {
        if (node == nullptr) return;
        print(node->left, level + 1);

        for (auto i = 0; i < level; i++) std::cout << "   ";
        //std::cout << "LEVEL " << level << " ";
        if (node->color == RED) std::cout << "R ";
        else std::cout << "B ";
        std::cout << node->key << ": " << node->value;
        if (node == root) std::cout << " (ROOT)";
        std::cout << std::endl;

        print(node->right, level + 1);
    }

    void removeList(Dictionary::Node *node) {
        if (node == nullptr) return;
        removeList(node->left);
        removeList(node->right);

        node->~Node();
        free(node);
    }

    Node *createNode(Node *new_node = nullptr, Node *parent = nullptr,
                     Node *left = nullptr, Node *right = nullptr) {
        auto current_node = (Node *)malloc(sizeof(Node));
        if (current_node == nullptr) return nullptr;
        if (new_node != nullptr) current_node->setNode(new_node->key, new_node->value, new_node->color);
        current_node->parent = parent;
        current_node->left = left;
        current_node->right = right;
        return current_node;
    }

    Node *createNode(const K& key, const V& value, Node *parent = nullptr,
                     Node *left = nullptr, Node *right = nullptr) {
        auto current_node = (Node *)malloc(sizeof(Node));
        if (current_node == nullptr) return nullptr;
        current_node->setNode(key, value);
        current_node->parent = parent;
        current_node->left = left;
        current_node->right = right;
        return current_node;
    }

    Node *copyList(Node *node, Node *parent = nullptr) {
        if (node == nullptr) return nullptr;
        auto new_node = createNode(node, parent, copyList(node->left, node),
                                   copyList(node->right, node));
        if (new_node == nullptr)
            throw std::runtime_error("Memory allocation error.");
        return new_node;
    }

    Node *treeInsert(const K& key, const V& value) {
        if (_size == 0) {
            root->setNode(key, value);
            _size++;
            return root;
        }

        Node *y = nullptr;
        auto x = root;
        while (x != nullptr) {
            if (key == x->key) {
                x->setNode(key, value);
                return x;
            }
            y = x;
            if (key < x->key) x = x->left;
            else x = x->right;
        }
        _size++;
        if (key < y->key) {
            y->left = createNode(key, value, y);
            return y->left;
        }
        y->right = createNode(key, value, y);
        return y->right;
    }

    Node *leftRotate(Node *x) {
        auto y = x->right;
        x->right = y->left;
        if (y->left != nullptr) (y->left)->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == (x->parent)->left) (x->parent)->left = y;
        else (x->parent)->right = y;
        y->left = x;
        x->parent = y;
        return x;
    }

    Node *rightRotate(Node *x) {
        auto y = x->left;
        x->left = y->right;
        if (y->right != nullptr) (y->right)->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == (x->parent)->right) (x->parent)->right = y;
        else (x->parent)->left = y;
        y->right = x;
        x->parent = y;
        return x;
    }

    Node *recolorBecauseLeftSubtree(Node *x) {
        auto y = ((x->parent)->parent)->right;
        if ((y != nullptr) && (y->color == RED)) {
            (x->parent)->color = BLACK;
            y->color = BLACK;
            ((x->parent)->parent)->color = RED;
            x = (x->parent)->parent;
        }
        else {
            if (x == (x->parent)->right) leftRotate(x->parent);
            (x->parent)->color = BLACK;
            ((x->parent)->parent)->color = RED;
            rightRotate((x->parent)->parent);
        }
        return x;
    }

    Node *recolorBecauseRightSubtree(Node *x) {
        auto y = ((x->parent)->parent)->left;
        if ((y != nullptr) && (y->color == RED)) {
            (x->parent)->color = BLACK;
            y->color = BLACK;
            ((x->parent)->parent)->color = RED;
            x = (x->parent)->parent;
        }
        else {
            if (x == (x->parent)->left) rightRotate(x->parent);
            (x->parent)->color = BLACK;
            ((x->parent)->parent)->color = RED;
            leftRotate((x->parent)->parent);
        }
        return x;
    }

    Node *getNode(const K& key) {
        if (_size == 0) return nullptr;
        Node *current_node = root, *node;
        while (current_node != nullptr) {
            if (key < current_node->key) current_node = current_node->left;
            else if (current_node->key < key) current_node = current_node->right;
            else {
                node = current_node;
                current_node = nullptr;
            }
        }
        return node;
    }

    Node *treeMinimum(Node *x) {
        while (x->left != nullptr) x = x->left;
        return x;
    }

    Node *treeMaximum(Node *x) {
        while (x->right != nullptr) x = x->right;
        return x;
    }

    Node *treeSuccessor(Node *x) {
        if (x->right != nullptr) return treeMinimum(x->right);
        auto y = x->parent;
        while ((y != nullptr) && (x == y->right)) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node *treePredecessor(Node *x) {
        if (x->left != nullptr) return treeMaximum(x->left);
        auto y = x->parent;
        while ((y != nullptr) && (x == y->left)) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node *recolorLeftSubtreeAfterDeleting(Node *x) {
        auto w = (x->parent)->right;
        if (w->color = RED) {
            w->color = BLACK;
            (x->parent)->color = RED;
            leftRotate(x->parent);
            w = (x->parent)->right;
        }
        if (((w->left)->color == BLACK) && ((w->right)->color == BLACK)) {
            w->color = RED;
            x = x->parent;
        }
        else {
            if ((w->right)->color == BLACK) {
                (w->left)->color =  BLACK;
                w->color = RED;
                rightRotate(w);
                w = (x->parent)->right;
            }
            w->color = (x->parent)->color;
            (x->parent)->color = BLACK;
            (w->right)->color = BLACK;
            leftRotate(x->parent);
            x = root;
        }
        return x;
    }

    Node *recolorRightSubtreeAfterDeleting(Node *x) {
        auto w = (x->parent)->left;
        if (w->color = RED) {
            w->color = BLACK;
            (x->parent)->color = RED;
            rightRotate(x->parent);
            w = (x->parent)->left;
        }
        if (((w->left)->color == BLACK) && ((w->right)->color == BLACK)) {
            w->color = RED;
            x = x->parent;
        }
        else {
            if ((w->left)->color == BLACK) {
                (w->right)->color =  BLACK;
                w->color = RED;
                leftRotate(w);
                w = (x->parent)->right;
            }
            w->color = (x->parent)->color;
            (x->parent)->color = BLACK;
            (w->left)->color = BLACK;
            rightRotate(x->parent);
            x = root;
        }
        return x;
    }

    void DeleteFixup(Node *x) {
        while ((x != root) && (x->color == BLACK))
            if (x == (x->parent)->left) x = recolorLeftSubtreeAfterDeleting(x);
            else x = recolorRightSubtreeAfterDeleting(x);
        x->color = BLACK;
    }
};

template<typename K, typename V>
int Dictionary<K, V>::size() const {return _size;}

template<typename K, typename V>
Dictionary<K, V>::Dictionary()
:_size(0) {
    root = (Node *)malloc(sizeof(Node));
    if (root == nullptr) throw std::runtime_error("Memory allocation error.");
    root->parent = nullptr;
    root->left = nullptr;
    root->right = nullptr;
}

template<typename K, typename V>
Dictionary<K, V>::~Dictionary() {
    if (_size != 0) removeList(root);
    else free(root);
}

/*template<typename K, typename V>
Dictionary<K, V>::Dictionary(const Dictionary<K, V> &dictionary)
:_size(dictionary.size()) {
    root = copyList(dictionary->root);
}

template<typename K, typename V>
Dictionary<K, V>& Dictionary<K, V>::operator =(const Dictionary<K, V> &dictionary) {
    if (&dictionary == this) return *this;
    try {
        auto new_root = copyList(dictionary->root);
        removeList(root);
        root = new_root;
    }
    catch (std::runtime_error &ex) {}
    return *this;
}*/

template<typename K, typename V>
const V &Dictionary<K, V>::operator[](const K &key) const {
    auto node = getNode(key);
    if (node == nullptr) throw std::invalid_argument("Error key");
    return node->value;
}

template<typename K, typename V>
V &Dictionary<K, V>::operator[](const K &key) {
    auto node = getNode(key);
    if (node == nullptr) throw std::invalid_argument("Error key");
    return node->value;
}

template<typename K, typename V>
void Dictionary<K, V>::put(const K &key, const V &value) {
    auto x = treeInsert(key, value);
    while ((x != root) && ((x->parent)->color) == RED)
        if (x->parent == ((x->parent)->parent)->left) x = recolorBecauseLeftSubtree(x);
        else x = recolorBecauseRightSubtree(x);
    root->color = BLACK;
}


template<typename K, typename V>
void Dictionary<K, V>::remove(const K &key) {
    if (_size == 0) return;
    Node *z = getNode(key), *y;
    if (z == nullptr) return;
    _size--;
    if (_size == 0) {
        z->~Node();
        return;
    }

    if ((z->left == nullptr) || (z->right == nullptr)) y = z;
    else y = treeSuccessor(z);

    auto x = (y->left != nullptr) ? y->left : y->right;
    if (x != nullptr) x->parent = y->parent;
    if (y->parent == nullptr) root = x;
    else if (y == (y->parent)->left) (y->parent)->left = x;
    else (y->parent)->right = x;
    if (y != z) {
        z->~Node();
        new (&z->key) K(y->key);
        new (&z->value) V(y->value);
    }
    if (y->color == BLACK) DeleteFixup(x);
    y->~Node();
    free(y);
}

template<typename K, typename V>
bool Dictionary<K, V>::contains(const K &key) {
    auto node = getNode(key);
    if (node == nullptr) return false;
    return true;
}

/*
 * Methods of the iterator
 */

template<typename K, typename V>
const K &Dictionary<K, V>::Iterator::key() const {
    if (current_node == nullptr) throw std::invalid_argument("Empty dictionary.");
    return current_node->key;
}

template<typename K, typename V>
const V &Dictionary<K, V>::Iterator::get() const {
    if (current_node == nullptr) throw std::invalid_argument("Empty dictionary.");
    return current_node->value;
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::set(const V &value) {
    if (current_node != nullptr) {
        current_node->value.~V();
        new (&current_node->value) V(value);
    }
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::next() {
    auto new_node = dictionary->treeSuccessor(current_node);
    if (new_node != nullptr) current_node = new_node;
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::prev() {
    auto new_node = dictionary->treePredecessor(current_node);
    if (new_node != nullptr) current_node = new_node;
}

template<typename K, typename V>
bool Dictionary<K, V>::Iterator::hasNext() const {
    return dictionary->treeSuccessor(current_node) != nullptr;
}

template<typename K, typename V>
bool Dictionary<K, V>::Iterator::hasPrev() const {
    return dictionary->treePredecessor(current_node) != nullptr;
}

#endif //ATD_DICTIONARY_HPP