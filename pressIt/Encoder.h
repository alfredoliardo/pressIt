//
//  Encoder.h
//  pressIt
//
//  Created by Alfredo Liardo on 12/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#ifndef pressIt_Encoder_h
#define pressIt_Encoder_h
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "minPriorityQueue.h"

class Encoder{
public:
    //Ho usato le typedef per accorciare i parametri delle funzioni
    typedef BST<char, unsigned long> HT;//huffman tree
    typedef Node<char,unsigned long> Node;
    typedef minPriorityQueue<unsigned long, Node > minPriorityQueue;
    Encoder();
    void encode(std::string inFileName,std::string outFileName,bool mode);
private:
    std::ifstream inFile;
    std::ofstream outFile;
    minPriorityQueue Q;
    unsigned long frequencyTable[256];
    std::vector<bool> codes[256];
    unsigned char byteBuffer;
    unsigned short byteCursor;
    unsigned char mask;
    HT *huffmanTree;
    
    void buildFrequencyTable();
    void fillQueue();
    void buildHuffmanTree();
    void generateCodes(const Node &node, const std::vector<bool>& prefix);
    void makeFile();
    void writeBit(unsigned short bit);
    void flushBits();
    void EncodeNode(Node node);
};



#endif
