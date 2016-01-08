//
//  Decoder.cpp
//  pressIt
//
//  Created by Alfredo Liardo on 17/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#include "Decoder.h"

Decoder::Decoder()
{
    huffmanTree = new HT();
    byteBuffer = byteCursor = byte = 0;
    mask = 1 << 7;
}

void Decoder::decode(std::string inFileName,std::string outFileName,bool mode)
{
    //open input file
    std::string inName(inFileName);
    if (inName.rfind(".ebast",inName.size()-6) == std::string::npos) {
        std::cerr << "This isn't a ebast File" << std::endl;
        exit(1);
    }
    inFile.open(inFileName, std::ios::in | std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Unable to read File" << std::endl;
        exit(1);
    }
    
    //open output file
    std::string outName(outFileName);
    outName += ".txt";
    if(std::ifstream(outName.c_str()))
    {
        std::cerr<<outName<<" already exists!"<<std::endl;
        exit(1);
    }
    
    outFile.open(outName.c_str(),std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Unable to create File" << std::endl;
        exit(1);
    }
    
    //reading frequency table from file
    huffmanTree->setRoot(readHeader());
    if (mode) {
        print(huffmanTree->getRoot());
    }
    //make file
    makeFile();
    
}
unsigned short Decoder::readBit()
{
    int i;
    if (byteCursor<1) {
        byte = inFile.get();
        i = ((byte << byteCursor) & mask)?1:0;
        byteCursor++;
    }else{
        i = ((byte << byteCursor) & mask)?1:0;
        byteCursor++;
    }
    if (byteCursor == 8) {
        byteCursor = 0;
    }
    return i;
}

unsigned char Decoder::readByte()
{
    unsigned char c=0;
    for (unsigned short i = 0; i < 8; i++) {
        if (readBit()) {
            c |= (1 << (7 - i));
        }
    }
    return c;
}


Decoder::Node& Decoder::readHeader()
{
        if (readBit())
        {
            Node *z = new Node(readByte(),0);
            z->setParent(HT::NIL).setLeft(HT::NIL).setRight(HT::NIL);
            return *z;
        }
        else
        {
            Node* leftChild = &readHeader();
            Node* rightChild = &readHeader();
            Node* z = new Node(0,0);
            z->setLeft(*leftChild);
            z->setRight(*rightChild);
            return *z;
        }
}

void Decoder::makeFile()
{
    Node *node = &huffmanTree->getRoot();
    while (!inFile.eof()) {
        if(readBit()){
            node = &node->right();
        }else{
            node = &node->left();
        }
        if (&node->left() == &HT::NIL and &node->right() == &HT::NIL) {
            outFile << static_cast<char>(node->key());
            node = &huffmanTree->getRoot();
        }
    }
}
