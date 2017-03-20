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
#include <sstream>
#include <stdexcept>
#include <cstdint>
//#include <cstdlib>
#include <stdlib.h>
using namespace std;

inline int64_t wc_l(string filename);
inline vector<string> getsplitfiles(string dir);
inline vector<string> getcountfiles(string dir);
inline bool checksort(string filename);
inline vector<string> countlines_split(const string &s, char delim, bool rm_empty=true);
inline void countlines_merge(string infile, string outfile);

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
        shell_cmd = "sort " + splitfiles[i] + " | uniq -c | awk '{print $1\"\\t\"$2}' > " + splitfiles[i] + ".count";
        cout << shell_cmd << endl;
        system(shell_cmd.c_str());
    }
    
    // check if count file properly sorted
    vector<string> countfiles = getcountfiles(tmpdir + "/tmp_countlines");
    checksort(countfiles[0]);
    
    // merge count files by sort -m 
    shell_cmd = "sort -m -k2,2 " + tmpdir + "/tmp_countlines/splitfile*.count > " + tmpdir + "/tmp_countlines/merge.count";
    cout << shell_cmd << endl;
    system(shell_cmd.c_str());
    
    // merge counts of merge.count file
    countlines_merge(tmpdir + "/tmp_countlines/merge.count",  output_file);
    
    
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

inline vector<string> getcountfiles(string dir)
{
    // get splitfiles 
    vector<string> splitfiles;
    string shell_cmd = "ls " + dir + "/splitfile*.count > " + dir+ "/filenames.txt";
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

inline bool checksort(string filename)
{
    ifstream fs_file(filename);
    if (!fs_file.is_open())
        throw runtime_error("fail to open " + filename);
    int64_t n = 0;
    string last_line = "!";
    while(1){
        string buf;
        getline(fs_file, buf);
        if (fs_file.eof())
            break;
        n++;
        vector<string> buf_vec = countlines_split(buf, '\t', true);
        if (buf_vec.size()!=2)
            throw runtime_error("incorrect format at line " + to_string(n));
        if (buf_vec[1] < last_line)
            throw runtime_error("incorrect order at line " + to_string(n));
    }
    fs_file.close();
    return true;
}

inline vector<string> countlines_split(const string &s, char delim, bool rm_empty) 
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        if (rm_empty){
            if (item!="") tokens.push_back(item);
        }else{
            tokens.push_back(item);
        }
    }
    return tokens;
}

inline void countlines_merge(string infile, string outfile)
{
    ifstream fs_infile(infile);
    if (!fs_infile.is_open())
        throw runtime_error("fail to open " + infile);
    ofstream fs_outfile(outfile);
    if (!fs_outfile.is_open())
        throw runtime_error("fail to open " + outfile);
    string last_line = "!";
    int64_t last_count = 0;
    int64_t n = 0;
    while (1){
        string buf;
        getline(fs_infile, buf);
        if (fs_infile.eof()){
            fs_outfile << last_count << '\t' << last_line << endl;
            break;
        }
        n++;
        vector<string> buf_vec = countlines_split(buf, '\t', true);
        if (buf_vec.size()!=2)
            throw runtime_error("incorrect format at line " + to_string(n));
        if (buf_vec[1] < last_line)
            throw runtime_error("incorrect order at line " + to_string(n));
        string cur_line = buf_vec[1];
        int64_t cur_count = stoll(buf_vec[0]);
        if (cur_line == last_line){
            last_count += cur_count;
        }else{
            if (last_line > "!")
                fs_outfile << last_count << '\t' << last_line << endl;
            last_count = cur_count;
            last_line = cur_line;
        }
        
    }
    
    fs_infile.close();
    fs_outfile.close();
}

#endif /* countlines_h */
