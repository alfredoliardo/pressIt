//
//  BST.h
//  pressIt
//
//  Created by Alfredo Liardo on 10/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#ifndef pressIt_BST_h
#define pressIt_BST_h

#include "Node.h"

template <class Key,class Data>
class BST{
public:
    static Node<Key,Data> NIL;
    BST();//void tree
    BST(Node<Key,Data>& node):root(node),s(0){};//new tree with default root
    ~BST(){};
    //Modificators
    virtual BST<Key,Data>& Insert(const Key& k,const Data& d);
    virtual BST<Key,Data>& Delete(const Key& k,const Data& d);
    //Operations
    Node<Key,Data>& Search(const Key& k)const;//find a node
    Node<Key,Data>& Search(Node<Key,Data>&,const Key& k)const;//overload
    Node<Key,Data>& Minimum()const;//from root
    Node<Key,Data>& Minimum( Node<Key,Data>& node)const;//from node
    Node<Key,Data>& Maximum()const;//from root
    Node<Key,Data>& Maximum( Node<Key,Data>& node)const;//from node
    bool isLeaf(const Node<Key,Data>& node)const;
    //Iterators
    Node<Key,Data>& Successor(const Node<Key,Data>& node);
    Node<Key,Data>& Predecessor(const Node<Key,Data>& node);
    //set/getter
    Node<Key,Data>& getRoot(){return *root;};
    BST<Key,Data>& setRoot(Node<Key,Data>& node);
    int size(){return s;};
protected:
    virtual BST<Key,Data>& Transplant(Node<Key,Data>& n1,Node<Key,Data>& n2);//funzione polimorfa
    Node<Key,Data> *root;
    int s;//size
};

template <class Key,class Data>
Node<Key,Data> BST<Key,Data>::NIL(0);


template <class Key,class Data>
BST<Key,Data>::BST() {
    NIL.setParent(NIL).setLeft(NIL).setRight(NIL);
    root = &NIL;
    s = 0;
}

template <class Key,class Data>
BST<Key,Data>& BST<Key,Data>::setRoot(Node<Key,Data>& node){
    root = &node;
    root->setParent(NIL);
    s++;
    return *this;
}
template <class Key,class Data>
BST<Key,Data>& BST<Key,Data>::Insert(const Key &k, const Data &d) {
    Node<Key,Data> *x = root;
    Node<Key,Data> *y = &NIL;
    s++;
    while (x != &NIL) {
        y = x;
        if(k == x->key()){
            x->setData(d);
            return *this;
        }else if (k < x->key()) {
            x = &x->left();
        }else{
            x = &x->right();
        }
    }
    Node<Key,Data> *node = new Node<Key,Data>(k,d);
    node->setLeft(NIL);
    node->setRight(NIL);
    node->setParent(*y);
    if (y == &NIL) {//albero vuoto
        root = node;
    }else if(node->key() < y->key()){
        y->setLeft(*node);
    }else{
        y->setRight(*node);
    }
    return *this;
}

template <class Key,class Data>
BST<Key,Data>& BST<Key,Data>::Transplant(Node<Key, Data> &n1, Node<Key, Data> &n2) {
    if (&n1.parent() == &NIL) {
        root = &n2;
    }
    else if(&n1 == &n1.parent().left()){
        n1.parent().setLeft(n2);
    }
    else{
        n1.parent().setRight(n2);
    }
    if (&n2 != &NIL) {
        n2.setParent(n1.parent());
    }
    return *this;
}

template <class Key,class Data>
BST<Key,Data>& BST<Key,Data>::Delete(const Key &k, const Data &d){
    Node<Key,Data>* z= &Search(k);
    if (z->data().size() > 1) {
        z->data().pop_front();
    }else if (&z->left() == &NIL) {
        Transplant(*z,z->right());
    }else if(&z->right() == &NIL){
        Transplant(*z,z->left());
    }else{
        Node<Key,Data> *y = &Minimum(z->right());
        if (&y->parent()!= z) {
            Transplant(*y, y->right());
            y->setRight(z->right());
            y->right().setParent(*y);
        }
        Transplant(*z, *y);
        y->setLeft(z->left());
        y->left().setParent(*y);
    }
    s--;
    return *this;
}

template <class Key,class Data>
Node<Key,Data>& BST<Key,Data>::Search(Node<Key,Data> &x, const Key &k)
const{
    Node<Key,Data>* it;
    it = &x;
    while (it != &NIL and k!=it->key() ) {
        if(k < it->key()){
            it = &it->left();
        }else{
            it = &it->right();
        }
    }
    
    return *it;
}

template <class Key,class Data>
Node<Key,Data>& BST<Key,Data>::Search(const Key &k)const{
    return Search(*root, k);
}

template <class Key,class Data>
Node<Key,Data>& BST<Key,Data>::Minimum(Node<Key,Data> &node) const{
    Node<Key,Data>* it = &node;
    while (&it->left() != &NIL) {
        it = &it->left();
    }
    return *it;
}

template <class Key,class Data>
Node<Key,Data>& BST<Key,Data>::Minimum() const{
    return Minimum(*root);
}

template <class Key,class Data>
Node<Key,Data>& BST<Key,Data>::Maximum(Node<Key,Data> &node) const{
    Node<Key,Data>* it = &node;
    while (&it->right() != &NIL) {
        it = &it->right();
    }
    return *it;
}

template <class Key,class Data>
Node<Key,Data>& BST<Key,Data>::Maximum() const{
    return Maximum(*root);
}

template <class Key,class Data>
bool BST<Key,Data>::isLeaf(const Node<Key,Data>& node)const{
    return (&node.left() == &NIL and &node.right() == &NIL);
}

/*this function print the whole tree with a preOrderVisit approach*/
template <class Key,class T>
void print(const Node<Key,T>& node, std::string prefix = ""){
    if (&node == &BST<Key,T>::NIL) {
        std::cout << prefix + "+- <nil>" << std::endl;
        return;
    }
    std::cout << prefix + "+- "<< node << std::endl;
    print(node.left(), prefix + "|  ");
    print(node.right(),prefix + "|  ");
}

#endif
