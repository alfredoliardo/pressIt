//
//  main.cpp
//  pressIt
//
//  Created by Alfredo Liardo on 10/02/15.
//  Copyright (c) 2015 Alfredo Liardo. All rights reserved.
//

#include <iostream>
#include "Decoder.h"
#include "Encoder.h"
using namespace std;

void usage_msg ( const string & pname )
{
    cerr << "Usage: " << pname << " : valid flags are :" << endl
    << " -i input_file  : required" << endl
    << " -o output_file : required" << endl
    << " -e  : turn on encoding mode ( default )" << endl
    << " -d  : turn on decoding mode" << endl
    << " -v  : verbose mode" << endl
    << " -h  : this help screen" << endl;
}

int main(int argc, const char * argv[]) {
    string in_name;
    string out_name;
    bool encode = true;
    bool verbose = false;
    
    for ( int i = 1 ; i < argc ; ++i )
    {
        if ( !strcmp( "-h", argv[i] ) || !strcmp( "--help", argv[i] ) )
        {
            usage_msg( argv[0] );
            exit(1);
        }
        else if ( !strcmp( "-i", argv[i] ) )
        {
            cout << "input file is '" << argv[++i] << "'" << endl;
            in_name = argv[i];
        }
        else if ( !strcmp( "-o", argv[i] ) )
        {
            cout << "output file is '" << argv[++i] << "'" << endl;
            out_name = argv[i];
        }
        else if ( !strcmp( "-d", argv[i] ) )
        {
            encode = false;
        }
        else if ( !strcmp( "-e", argv[i] ) )
        {
            encode = true;
        }
        else if ( !strcmp( "-v", argv[i] ) )
        {
            verbose = true;
        }
        
    }
    if ( !in_name.size() || !out_name.size() )
    {
        cerr << "input and output file are required, nothing to do!" << endl;
        usage_msg( argv[0] );
        exit( 1 );
    }
    
    if ( encode )
    {
        cerr << "running in encoder mode" << endl;
        Encoder encoder;
        encoder.encode(in_name, out_name,verbose);
    }
    else
    {
        cerr << "running in decoder mode" << endl;
        Decoder decoder;
        decoder.decode(in_name, out_name,verbose);
    }
    cerr << "done .... (created by Alfredo Liardo)" << endl;
    
    return 0;
}
