#ifndef CLUSTERANALYSES_TREENODE_H
#define CLUSTERANALYSES_TREENODE_H


#include <vector>

template<class T>
class TreeNode {
    friend class Field;
    friend class Cluster_Search;

public:
    explicit TreeNode (T v);
    TreeNode (const TreeNode<T> &t);
    TreeNode &operator= (const TreeNode<T> &t);
    ~TreeNode ();
    const TreeNode<T> *first_child () const;
    const TreeNode<T> *brother () const;
    const TreeNode<T> *last_child () const;
    const TreeNode<T> *add_child (TreeNode<T> *t);
    const TreeNode<T> *add_child (T value);
    T value () const;
private:
    T value_;
    TreeNode<T> *last_child_ ();
    TreeNode<T> *first_child_;
    TreeNode<T> *brother_;
};

template<class T>
TreeNode<T>::TreeNode (T value):value_ (value) {
    // creates treenode with defined value
    first_child_ = nullptr;
    brother_ = nullptr;
}

template<class T>
TreeNode<T>::~TreeNode () {
    // deletes all the nodes connected to itself
    delete first_child_;
    delete brother_;
}

template<class T>
const TreeNode<T> *TreeNode<T>::first_child () const {
    // returns pointer to first child as to const
    return first_child_;
}

template<class T>
const TreeNode<T> *TreeNode<T>::brother () const {
    // returns pointer to next brother as to const
    return brother_;
}

template<class T>
TreeNode<T> *TreeNode<T>::last_child_ () {
    // returns pointer to last child
    // works only if there is first child
    TreeNode<T> *curr = first_child_;
    while (curr->brother () != nullptr) {
        curr = curr->brother_;
    }
    return curr;
}

template<class T>
const TreeNode<T> *TreeNode<T>::last_child () const {
    // returns pointer to last child as to const
    // works only if there is first child
    return last_child_ ();
}

template<class T>
const TreeNode<T> *TreeNode<T>::add_child (TreeNode<T> *t) {
    // adds child to node. Returns pointer to it
    // i'm not sure but there are possible problems with linking
    if (first_child () != nullptr) {
        last_child_ ()->brother_ = t;
    } else {
        first_child_ = t;
    }
    return t;
}

template<class T>
const TreeNode<T> *TreeNode<T>::add_child (T v) {
    // adds child to node. Returns pointer to it
    auto *new_node = new TreeNode<T> (v);
    add_child (new_node);
    return new_node;
}

template<class T>
TreeNode<T>::TreeNode (const TreeNode<T> &t) {
    delete brother_;
    delete first_child_;
    brother_ = t.brother_;
    first_child_ = t.first_child_;
    value_ = t.value_;
}

template<class T>
TreeNode<T> &TreeNode<T>::operator= (const TreeNode<T> &t) {
    if (this != t) {
        delete brother_;
        delete first_child_;
        brother_ = t.brother_;
        first_child_ = t.first_child_;
        value_ = t.value_;
    }
    return *this;
}

template<class T>
T TreeNode<T>::value () const {
    return value_;
}


#endif //CLUSTERANALYSES_TREENODE_H
