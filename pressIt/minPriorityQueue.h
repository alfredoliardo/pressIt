//
//  minPriorityQueue.h
//  pressIt
//
//  Created by Alfredo Liardo on 11/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#ifndef pressIt_minPriorityQueue_h
#define pressIt_minPriorityQueue_h
#include <iostream>
#include "RBT.h"

template <class Key,class Data>
class minPriorityQueue {
public:
    minPriorityQueue():tree(),size(0){}
    minPriorityQueue<Key,Data>& Insert(const Key& k,const Data& val);
    Node<Key,Data>& Minimum() const;
    Node<Key,Data>& extractMin();
    unsigned int getSize() const{return size;}
private:
    RBT<Key,Data> tree;
    unsigned int size;
};

template <class Key,class Data>
minPriorityQueue<Key,Data>& minPriorityQueue<Key,Data>::Insert(const Key& k,const Data& val){
    size++;
    tree.Insert(k,val);
    return *this;
}

template <class Key,class Data>
Node<Key,Data>& minPriorityQueue<Key,Data>::Minimum() const{
    Node<Key,Data> *min = &tree.Minimum();
    return *min;
}

template <class Key,class Data>
Node<Key,Data>& minPriorityQueue<Key,Data>::extractMin(){
    
    if(size > 0){
        Node<Key,Data> *y = &tree.Minimum();
        Node<Key,Data> *min = new Node<Key,Data>(*y);
        Data d = y->data().front();
        tree.Delete(y->key(),d);
        size--;
        return *min;
    }
    std::cout << "Impossibile: la coda e' vuota." << std::endl;
    return BST<Key,Data>::NIL;
}

#endif

