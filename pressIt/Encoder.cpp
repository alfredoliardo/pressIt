//
//  Encoder.cpp
//  pressIt
//
//  Created by Alfredo Liardo on 12/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#include "Encoder.h"


Encoder::Encoder()
{
    huffmanTree = new HT();
    mask = 1 << 7;
    byteBuffer = 0;
    byteCursor = 0;
}

void Encoder::encode(std::string inFileName,std::string outFileName,bool mode)
{
    long long inSize,outSize;
    //open input file
    if (inFileName.rfind(".txt",inFileName.size()-4) == std::string::npos) {
        std::cerr << "This isn't a txt File" << std::endl;
        exit(1);
    }
    inFile.open(inFileName.c_str(), std::ios::in | std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Unable to read File" << std::endl;
        exit(1);
    }
    //Calcolo la dimensione del file di input
    inFile.seekg(0, std::ios::end);
    inSize = inFile.tellg();
    inFile.clear();
    inFile.seekg(0,std::ios::beg);
    
    //open output file
    outFileName += ".ebast";
    if(std::ifstream(outFileName.c_str()))
    {
        std::cerr<<outFileName<<" already exists!"<<std::endl;
        exit(1);
    }

    outFile.open(outFileName.c_str(),std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Unable to create File" << std::endl;
        exit(1);
    }
    
    //build frequecy table from input file
    buildFrequencyTable();
    if (mode) {
        std::cout <<"Frequency Table: "<<std::endl;
        for(int i = 0;i<256;i++)
            if(frequencyTable[i]>0)
                std::cout << static_cast<char>(i) <<':'<<frequencyTable[i]<<std::endl;
    }
    //fill the min priority queue
    fillQueue();
    
    //build huffman tree
    buildHuffmanTree();
    if (mode) {
        std::cout << "The Huffman tree: " << std::endl;
        print(huffmanTree->getRoot());
        std::cout << std::endl;
    }
    //Serialize huffman tree, the header of file
    EncodeNode(huffmanTree->getRoot());
    
    //build coding table
    generateCodes(huffmanTree->getRoot(), std::vector<bool>());
    
    if (mode) {
        std::cout << "Code's table: " <<std::endl;
        for (int i = 0; i < 256; i++) {
            if (codes[i].size()>0) {
                std::cout << static_cast<char>(i) <<':';
                for (int j = 0; j< (codes[i].size()); j++ ) {
                    std::cout << (codes[i].at(j)?'1':'0');
                }
                std::cout<<std::endl;
            }
        }
    }
    
    //write the encoded file
    makeFile();
    inFile.open(outFileName.c_str(),std::ios::in | std::ios::binary);
    inFile.seekg(0,std::ios::end);
    outSize = inFile.tellg();
    inFile.close();
    if (1) {
        std::cout <<"Original size: "<<inSize<<"byte"<<std::endl;
        std::cout <<"Encoded size: "<<outSize<<"byte"<<std::endl;
        std::cout <<"Saved "<<(100 - (outSize*100/inSize))<<"%"<<std::endl;
    }
}

void Encoder::buildFrequencyTable()
{
    //array to hold frequency table for all ASCII characters in the file
    for(int i=0;i<256;++i)
        frequencyTable[i]=0;
    
    //read the whole file and count the ASCII char table frequencies
    char c;
    unsigned char ch;
    while(inFile.get(c))
    {
        ch=c;
        ++frequencyTable[ch];
    }
    
    inFile.clear(); //clear EOF flag
    inFile.seekg(0,std::ios::beg); //reset get() pointer to beginning
}

void Encoder::fillQueue()
{
    for(int i=0;i<256;++i)
    {
        if(frequencyTable[i]>0)
        {
            //output char freq table to the output file
            //outFile << static_cast<char>(i)<<' '<<frequencyTable[i]<<std::endl;
            //fill queue
            Node *newNode = new Node(static_cast<unsigned char>(i),frequencyTable[i]);
            newNode->setParent(HT::NIL).setLeft(HT::NIL).setRight(HT::NIL);
            Q.Insert(frequencyTable[i],*newNode);
        }
    }
}


void Encoder::buildHuffmanTree()
{
    int n= Q.getSize();
    for(int i = 1;i<n;i++){
        Node *z = new Node();
        z->setParent(HT::NIL).setLeft(HT::NIL).setRight(HT::NIL);
        z->setLeft(Q.extractMin().data().front());
        z->setRight(Q.extractMin().data().front());
        z->setData(z->left().data().front() + z->right().data().front());
        Q.Insert(z->data().front(),*z);
    }
    huffmanTree->setRoot(Q.extractMin().data().front());
}

void Encoder::generateCodes(const Node &node, const std::vector<bool>& prefix)
{
    if (&node.left() == &HT::NIL and &node.right() == &HT::NIL)//if is a leaf
    {
        codes[static_cast<unsigned char>(node.key())] = prefix;
    }
    else
    {
        std::vector<bool> leftPrefix = prefix;
        leftPrefix.push_back(false);
        generateCodes(node.left(), leftPrefix);
        
        std::vector<bool> rightPrefix = prefix;
        rightPrefix.push_back(true);
        generateCodes(node.right(), rightPrefix);
    }
}


void Encoder::makeFile()
{
    char c;
    unsigned char ch;
    unsigned short bit;
    while(inFile.get(c))
    {
        ch = c;
        //send the Huffman string to output file bit by bit
        for(unsigned int i = 0;i < codes[ch].size();i++)
        {
            if(codes[ch].at(i) == false){
                bit=0;
            }else{
                bit=1;
            }
            writeBit(bit);
        }
    }
    flushBits();
//    bit=-1; // send EOF
//    writeBit(bit);
//    flushBits();

    inFile.close();
    outFile.close();
}

void Encoder::writeBit(unsigned short bit)
{
    if(bit < 2){//if not EOF
        if(bit){
            byteBuffer |= (1 << (7 - byteCursor));
        }
        byteCursor++;
        if (byteCursor == 8) {
            outFile << byteBuffer;
            byteBuffer = 0;
            byteCursor = 0;
        }
    }else{
        outFile << bit;
    }
}

void Encoder::flushBits()
{
    while (byteCursor) {
        writeBit(0);
    }
}

void Encoder::EncodeNode(Node node)
{
    if (&node.left() == &HT::NIL and &node.right() == &HT::NIL)//if is a leaf
    {
        writeBit(1);
        for (unsigned short i = 0; i < 8; i++) {
            unsigned char c = node.key();
            writeBit(((c << i) & mask)?1:0);
        }
    }
    else
    {
        writeBit(0);
        EncodeNode(node.left());
        EncodeNode(node.right());
    }
}