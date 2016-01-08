//
//  RBT.h
//  pressIt
//
//  Created by Alfredo Liardo on 10/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#ifndef pressIt_RBT_h
#define pressIt_RBT_h

#include "BST.h"

template <class Key,class Data>
class RBT : public BST<Key,Data> {
public:
    RBT();
    ~RBT(){};
    BST<Key,Data>& Insert(const Key& k,const Data& d); //Override
    BST<Key,Data>& Delete(const Key& k,const Data& d); //Override
private:
    void leftRotate(Node<Key,Data>& node);
    void rightRotate(Node<Key,Data>& node);
    void InsertFixup(Node<Key,Data>& node);
    void DeleteFixup(Node<Key,Data>& node);
    BST<Key,Data>& Transplant(Node<Key,Data>& n1,Node<Key,Data>& n2);
};

template <class Key,class Data>
RBT<Key,Data>::RBT() {
    BST<Key,Data>();
    BST<Key,Data>::root->setColor(BLACK);
}

template <class Key,class Data>
BST<Key,Data>& RBT<Key,Data>::Insert(const Key &k, const Data &d) {
    Node<Key,Data> *x = BST<Key,Data>::root;
    Node<Key,Data> *y = &BST<Key,Data>::NIL;
    BST<Key,Data>::s++;
    while (x != &BST<Key,Data>::NIL) {
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
    node->setLeft(BST<Key,Data>::NIL);
    node->setRight(BST<Key,Data>::NIL);
    node->setParent(*y);
    if (y == &BST<Key,Data>::NIL) {//albero vuoto
        BST<Key,Data>::root = node;
    }else if(node->key() < y->key()){
        y->setLeft(*node);
    }else{
        y->setRight(*node);
    }
    node->setLeft(BST<Key,Data>::NIL);
    node->setRight(BST<Key,Data>::NIL);
    node->setColor(RED);
    InsertFixup(*node);
    return *this;
}

template <class Key,class Data>
void RBT<Key,Data>::InsertFixup(Node<Key, Data> &node) {
    Node<Key,Data> *z = &node;;
    Node<Key,Data> *y;
    while (z->parent().color() == RED) {
        if (&z->parent() == &z->parent().parent().left()) {
            y = &z->parent().parent().right();
            if (y->color()==RED) {
                z->parent().setColor(BLACK);            //Case 1
                y->setColor(BLACK);                         //Case 1
                z->parent().parent().setColor(RED);//Case 1
                z = &z->parent().parent();            //Case 1
            } else {
                if(z == &z->parent().right()){
                    z = &z->parent();                     //Case 2
                    leftRotate(*z);                          //Case 2
                }
                z->parent().setColor(BLACK);            //Case 3
                z->parent().parent().setColor(RED); //Case 3
                rightRotate(z->parent().parent());   //Case 3
            }
        } else {
            y = &z->parent().parent().left();
            if (y->color()==RED) {
                z->parent().setColor(BLACK);
                y->setColor(BLACK);
                z->parent().parent().setColor(RED);
                z = &z->parent().parent();
            } else {
                if(z == &z->parent().left()){
                    z = &z->parent();
                    rightRotate(*z);
                }
                z->parent().setColor(BLACK);
                z->parent().parent().setColor(RED);
                leftRotate(z->parent().parent());
            }
        }
    }
    this->BST<Key,Data>::root->setColor(BLACK);
}

template <class Key,class Data>
void RBT<Key,Data>::leftRotate(Node<Key, Data> &node) {
    Node<Key,Data>* y = &node.right();
    node.setRight(y->left());
    if (&y->left() != &BST<Key,Data>::NIL) {
        y->left().setParent(node);
    }
    y->setParent(node.parent());
    if (&node.parent() == &BST<Key,Data>::NIL) {
        BST<Key,Data>::root = y;
    }else if(&node == &node.parent().left()){
        node.parent().setLeft(*y);
    }else{
        node.parent().setRight(*y);
    }
    y->setLeft(node);
    node.setParent(*y);
}

template <class Key,class Data>
void RBT<Key,Data>::rightRotate(Node<Key, Data> &node) {
    Node<Key,Data>* y = &node.left();
    node.setLeft(y->right());
    if (&y->right() != &BST<Key,Data>::NIL) {
        y->right().setParent(node);
    }
    y->setParent(node.parent());
    if (&node.parent() == &BST<Key,Data>::NIL) {
        BST<Key,Data>::root = y;
    }else if(&node == &node.parent().right()){
        node.parent().setRight(*y);
    }else{
        node.parent().setLeft(*y);
    }
    y->setRight(node);
    node.setParent(*y);
}

template <class Key,class Data>
BST<Key,Data>& RBT<Key,Data>::Transplant(Node<Key, Data> &n1, Node<Key, Data> &n2) {
    if (&n1.parent() == &BST<Key,Data>::NIL) {
        BST<Key,Data>::root = &n2;
    }
    else if(&n1 == &n1.parent().left()){//is a left chile
        n1.parent().setLeft(n2);
    }
    else{
        n1.parent().setRight(n2);
    }
    if(&n2 != &BST<Key,Data>::NIL)
        n2.setParent(n1.parent());
    return *this;
}

template <class Key,class Data>
BST<Key,Data>& RBT<Key,Data>::Delete(const Key& k, const Data& d){
    Node<Key,Data>* z= &BST<Key,Data>::Search(k);
    Node<Key,Data>* y = z;
    Node<Key,Data>* x;
    Color origColor = y->color();
    if (z->data().size() > 1) {
        z->data().pop_front();
        return *this;
    }
    if (&z->left() == &BST<Key,Data>::NIL) {
        x = &z->right();
        Transplant(*z,z->right());
    }else if(&z->right() == &BST<Key,Data>::NIL){
        x = &z->left();
        Transplant(*z,z->left());
    }else{
        y = &BST<Key,Data>::Minimum(z->right());
        origColor = y->color();
        x = &y->right();
        if (&y->parent()== z) {
            x->setParent(*y);
        }else{
            Transplant(*y, y->right());
            y->setRight(z->right());
            y->right().setParent(*y);
        }
        Transplant(*z, *y);
        y->setLeft(z->left());
        y->left().setParent(*y);
        y->setColor(z->color());
    }
    if (origColor == BLACK) {
        DeleteFixup(*x);
    }
    BST<Key,Data>::s--;
    return *this;
}

template <class Key,class Data>
void RBT<Key,Data>::DeleteFixup(Node<Key,Data>& node){
    Node<Key,Data>  *it = &node;
    while (it != &BST<Key,Data>::NIL and it->color() == BLACK) {
        if (it == &it->parent().left()) {
            Node<Key,Data>  *w = &it->parent().right();
            if (w->color() == RED) {
                w->setColor(BLACK);
                it->parent().setColor(RED);
                leftRotate(it->parent());
                w = &it->parent().right();
            }
            if (w->left().color() == BLACK and w->right().color() == BLACK) {
                w->setColor(RED);
                it = &it->parent();
            }else {
                if (w->right().color() == BLACK){
                    w->left().setColor(BLACK);
                    w->setColor(RED);
                    rightRotate(*w);
                    w = &it->parent().right();
                }
                w->setColor(it->parent().color());
                it->parent().setColor(BLACK);
                w->right().setColor(BLACK);
                leftRotate(it->parent());
                it = this->root;
            }
        }else{
            Node<Key,Data>  *w = &it->parent().left();
            if (w->color() == RED) {
                w->setColor(BLACK);
                it->parent().setColor(RED);
                leftRotate(it->parent());
                w = &it->parent().left();
            }
            if (w->right().color() == BLACK and w->left().color() == BLACK) {
                w->setColor(RED);
                it = &it->parent();
            }else {
                if (w->left().color() == BLACK){
                    w->right().setColor(BLACK);
                    w->setColor(RED);
                    rightRotate(*w);
                    w = &it->parent().left();
                }
                w->setColor(it->parent().color());
                it->parent().setColor(BLACK);
                w->left().setColor(BLACK);
                leftRotate(it->parent());
                it = this->root;
            }
        }
    }
    it->setColor(BLACK);
}

#endif
