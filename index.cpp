#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void fetch_json_data(unordered_map<char, double>&, string);
int fetch_text_data_to_tokens(unordered_map<char, double>&, vector<double>&, string);

int main()
{
    unordered_map<char, double> vocab;
    fetch_json_data(vocab, "vocab.json");
    
    vector<double> tokens;
    cout << fetch_text_data_to_tokens(vocab, tokens, "test.txt");
    
    return 0;
}

void fetch_json_data(unordered_map<char, double>& vcb, string path)
{
    ifstream file(path);
    json data;
    file >> data;
    file.close();
    for(auto& [key, values] : data.items())
    {
        if (key.size() == 1)
            vcb[key[0]] = values.get<double>();
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