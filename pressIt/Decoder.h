//
//  Decoder.h
//  pressIt
//
//  Created by Alfredo Liardo on 17/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#ifndef __pressIt__Decoder__
#define __pressIt__Decoder__

#include <fstream>
#include <string>
#include "minPriorityQueue.h"

class Decoder
{
public:
    typedef BST<char,unsigned long> HT;//huffman tree
    typedef Node<char,unsigned long> Node;
    Decoder();
    void decode(std::string inFileName,std::string outFileName,bool mode);
private:
    std::ifstream inFile;
    std::ofstream outFile;
    unsigned long frequencyTable[256];
    unsigned char byteBuffer;
    unsigned char byte;
    unsigned char mask;
    unsigned short byteCursor;
    HT *huffmanTree;
    unsigned short readBit();
    unsigned char readByte();
    Node& readHeader();
    void makeFile();
};

#endif /* defined(__pressIt__Decoder__) */
