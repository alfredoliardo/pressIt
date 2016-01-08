//
//  Node.h
//  pressIt
//
//  Created by Alfredo Liardo on 10/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#ifndef __pressIt__Node__
#define __pressIt__Node__

#include <list> //to store elements with keys repeated

enum Color{RED,BLACK};

template <class Key,class Data>
class Node{
public:
    typedef Key key_type;
    typedef Data data_type;
    typedef Node<key_type,data_type> value_type;
    //Constructors
    Node(value_type* ptr = 0):k(),d(),c(BLACK){
        p = l = r = ptr?ptr:0;//set parent,left and right to 0 if there isn't arguments
    }
    Node(const value_type& node);
    Node(const key_type& k,const data_type& d);
    //Destructor
    ~Node();
    //Operators
    //Node& operator= (const Node& node);
    template <class sT1,class sT2>
    friend std::ostream& operator<<(std::ostream& os,const Node<sT1,sT2>& node);
    //Getter
    Key key()const{return k;};
    std::list<Data>& data()const{return this->d;};
    Color color()const{return this->c;};
    value_type& parent()const{return *this->p;};
    value_type& left()const{return *this->l;};
    value_type& right()const{return *this->r;};
    //Setter
    value_type& setKey(const key_type& k){this->k = k;return *this;};
    value_type& setData(const data_type& d);
    value_type& setColor(const Color& name){this->c = name;return *this;};
    value_type& setParent(value_type& n){this->p = &n;return *this;};
    value_type& setLeft(value_type& n){this->l = &n;return *this;};
    value_type& setRight(value_type& n){this->r = &n;return *this;};
private:
    Key k;
    mutable std::list<Data> d;
    Color c;
    Node<Key,Data> *p,*l,*r;
};

template <class Key,class Data>
Node<Key,Data>& Node<Key,Data>::setData(const data_type& d) {
    this->d.push_back(d);
    return  *this;
}


template <class Key,class Data>
Node<Key,Data>::Node(const value_type& node):d(node.data()) {
    k = node.k;
    c = node.c;
    p = node.p;
    l = node.l;
    r = node.r;
}

template <class Key,class Data>
Node<Key,Data>::Node(const key_type& k,const data_type& d):k(k),p(0),l(0),r(0)
{
    this->d.push_back(d);
}

template <class Key,class Data>
Node<Key,Data>::~Node() {
    d.clear();
}

template <class Key, class Data>
bool operator== (const Node<Key,Data>& lhs, const Node<Key,Data>& rhs)
{ return lhs.key()==rhs.key() && lhs.data()==rhs.data(); }

template <class Key, class Data>
bool operator!= (const Node<Key,Data>& lhs, const Node<Key,Data>& rhs)
{ return !(lhs==rhs); }

template <class Key, class Data>
bool operator<  (const Node<Key,Data>& lhs, const Node<Key,Data>& rhs)
{ return lhs.key()<rhs.key() || (!(rhs.key()<lhs.key()) && lhs.data()<rhs.data()); }

template <class Key, class Data>
bool operator<= (const Node<Key,Data>& lhs, const Node<Key,Data>& rhs)
{ return !(rhs<lhs); }

template <class Key, class Data>
bool operator>  (const Node<Key,Data>& lhs, const Node<Key,Data>& rhs)
{ return rhs<lhs; }

template <class Key, class Data>
bool operator>= (const Node<Key,Data>& lhs, const Node<Key,Data>& rhs)
{ return !(lhs<rhs); }

template <class sT1,class sT2>
std::ostream& operator<<(std::ostream& os,const Node<sT1,sT2>& node) {
    os << '(' << node.key() << '|'<< node.data().size() <<'|'<< (node.color()?"BLACK":"RED") << ')' ;
    return os;
}


#endif /* defined(__pressIt__Node__) */
