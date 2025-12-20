#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void fetch_json_data(unordered_map<char, double>&, string);
int tokens_to_pairs(vector<double>&,vector<vector<double>>&);
int fetch_text_data_to_tokens(unordered_map<char, double>&, vector<double>&, string);
template <typename vectr> void display(vector<vector<vectr>>&);
template <typename vectrr> void displayy(vector<vector<vectrr>>&);

int main()
{
    unordered_map<char, double> vocab;
    fetch_json_data(vocab, "vocab.json");
    
    vector<double> tokens;
    fetch_text_data_to_tokens(vocab, tokens, "test.txt");

    vector<vector<double>> pairs;
    tokens_to_pairs(tokens, pairs);

/*    display(pairs);
    cout << endl << endl; */

    bool isthat[pairs.size()] = {false};
    vector<double> frequecy;
    for (int i = 0; i < pairs.size(); ++i)
    {
        if (isthat[i]) continue;
        int ct = 1;
        for (int j = i + 1; j < pairs.size(); ++j)
        {
            if (pairs[i][0]==pairs[j][0] && pairs[i][1]==pairs[j][1])
            {
                isthat[j] = true;
                ++ct;
            }
        }
        frequecy.push_back(ct);
    }

    for (auto& fre : frequecy)
    {
        cout << fre << " ";
    }
    return 0;
}

void fetch_json_data(unordered_map<char, double>& vcb, string path)
{
    ifstream file(path);
    if (!file.is_open()) cout << path << " Can Not Open " << endl;
    else
    {
        json data;
        file >> data;
        file.close();
        for(auto& [key, values] : data.items())
        {
            if (key.size() == 1)
                vcb[key[0]] = values.get<double>();
        }
    }
}

int fetch_text_data_to_tokens(unordered_map<char, double>& vcb, vector<double>& tk, string path)
{
    ifstream file(path);
    if (!file.is_open()) cout << path << " Can Not Open " << endl;
    else
    {
        string line;
        while(getline(file, line))
        {
            for (int i = 0; i < line.size(); ++i) tk.push_back(vcb[line[i]]);
            line.clear();
        }
    }
    return tk.size();
}

int tokens_to_pairs(vector<double>& tokens ,vector<vector<double>>& pairs)
{
    int ct = 0;
    vector<double> pair;
    for (int i = 0; i < tokens.size(); ++i)
    {
        if (ct==2)
        {
            pairs.push_back(pair);
            pair.clear();
            ct = 0;
            i-=2;
        }
        else
        {
            pair.push_back(tokens[i]);
            ++ct;
        }
    }
    return pairs.size();
}

template <typename vectr> void display(vector<vector<vectr>>& vec)
{
    for (auto& values : vec)
    {
        for (auto& value : values) cout << value << " ";
        cout << endl;
    }
}

template <typename vectrr> void displayy(vector<vector<vectrr>>& vec)
{
    for (auto& values : vec)
    {
        for (auto& value : values)
        {    
            for (auto& val : value)
                cout << val << " ";
            cout << endl;
        }
        cout << endl;
    }
}