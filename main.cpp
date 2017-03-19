//
//  main.cpp
//  countlines
//
//  Created by Zhixing Feng on 17/3/17.
//  Copyright © 2017年 Icahn School of Medicine at Mount Sinai. All rights reserved.
//


#include "./countlines.h"

int main(int argc, char* argv[])
{
    try{
        if (argc==1){
            cout << "countlines input_file output_file n_split tmpdir" << endl;
            return 0;
        }else{
            if (argc != 5){
                cout << "countlines input_file output_file n_split tmpdir" << endl;
                return 1;
            }
        }
        string input_file = argv[1];
        string output_file = argv[2];
        int n_split = atoi(argv[3]);
        string tmpdir = argv[4];
    
        countlines(input_file, output_file, n_split, tmpdir);
    }
    catch(const std::overflow_error& e) {
        cerr << "overflow_error: " << e.what() << endl;
    }
    catch(const std::runtime_error& e) {
        cerr << "runtime_error: " << e.what() << endl;
    }
    catch(const std::exception& e) {
        cerr << "expection: " << e.what() << endl;
    }
    catch(...) {
        
    }
    return 0;
}

