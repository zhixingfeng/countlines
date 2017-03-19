//
//  countlines.h
//  countlines
//
//  Created by Zhixing Feng on 17/3/17.
//  Copyright © 2017年 Icahn School of Medicine at Mount Sinai. All rights reserved.
//

#ifndef countlines_h
#define countlines_h
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

inline int64_t wc_l(string filename);
inline vector<string> getsplitfiles(string dir);

inline void countlines(string input_file, string output_file, int n_split, string tmpdir)
{
    // get number of lines of input file
    int64_t nl = wc_l(input_file);
    cout << "number of lines: " << nl << endl;
    
    // get number of lines of split file
    int64_t nl_split = nl / n_split;
    cout << "number of lines per file: " << nl_split << endl;
    
    // create tmp dir 
    string shell_cmd = "mkdir -p " + tmpdir + "/tmp_countlines";
    cout << shell_cmd << endl;
    system(shell_cmd.c_str());
    
    shell_cmd = "rm -rf " + tmpdir + "/tmp_countlines/*";
    cout << shell_cmd << endl;
    system(shell_cmd.c_str());

    // split files
    shell_cmd = "split -l " + to_string(nl_split) + " " + input_file + " " + tmpdir + "/tmp_countlines/splitfile";
    cout << shell_cmd << endl;
    system(shell_cmd.c_str());
    
    // count split files
    vector<string> splitfiles = getsplitfiles(tmpdir + "/tmp_countlines");
    if (splitfiles.size() != n_split && splitfiles.size() != n_split + 1)
        throw runtime_error("incorrect number of split files");
    for (int i=0; i<(int)splitfiles.size(); i++){
        shell_cmd = "sort " + splitfiles[i] + " | uniq -c > " + splitfiles[i] + ".count";
        cout << shell_cmd << endl;
        system(shell_cmd.c_str());
    }
    
    
}
inline int64_t wc_l(string filename)
{
    ifstream fs_input(filename);
    if (!fs_input.is_open())
        throw runtime_error("fail to open " + filename);
    string buf;
    int64_t n_lines=0;
    while(1){
        getline(fs_input, buf);
        if (fs_input.eof())
            break;
        n_lines++;
    }
    fs_input.close();
    return n_lines;
}

inline vector<string> getsplitfiles(string dir)
{
    // get splitfiles 
    vector<string> splitfiles;
    string shell_cmd = "ls " + dir + "/splitfile* > " + dir+ "/filenames.txt";
    cout << shell_cmd << endl;
    system(shell_cmd.c_str());
    
    // read splitfiles
    ifstream fs_file(dir+ "/filenames.txt");
    if (!fs_file.is_open())
        throw runtime_error("fail to open " + dir+ "/filenames.txt");
    while(1){
        string buf;
        getline(fs_file, buf);
        if (fs_file.eof())
            break;
        splitfiles.push_back(buf);
    }
    fs_file.close();
    return splitfiles;
}
#endif /* countlines_h */
