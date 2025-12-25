#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
char token_to_char(unordered_map<char, long long>&, double);
int tokens_to_pairs(vector<double>&,vector<vector<double>>&);
void fetch_json_data(unordered_map<char, long long>&, string);
template <typename vectr> void display(vector<vector<vectr>>&);
template <typename empty> void remove_empty(vector<vector<empty>>&);
int fetch_text_data_to_tokens(unordered_map<char, long long>&, vector<double>&, string);
void pairs_to_most_frequent_merge(vector<vector<double>>&, unordered_map<char, long long>&);

struct Frequency
{
    double token1, token2, merge, ct = 0;
};

int main()
{
    unordered_map<char, long long> vocab;
    fetch_json_data(vocab, "vocab.json");

    vector<double> tokens;
    fetch_text_data_to_tokens(vocab, tokens, "test.txt");

    vector<vector<double>> pair;
    tokens_to_pairs(tokens, pair);

    pairs_to_most_frequent_merge(pair, vocab);

    return 0;
}

void fetch_json_data(unordered_map<char, long long>& vcb, string path)
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
        cout << path << " Data Fetched Sucessfully.." << endl;
    }
}

int fetch_text_data_to_tokens(unordered_map<char, long long>& vcb, vector<double>& tk, string path)
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
        cout << "Text Converts To Tokens From " << path << " Sucessfully..." << endl;
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
    cout << pairs.size() << " Pairs Created Sucessfully..." << endl;
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

template <typename empty> void remove_empty(vector<vector<empty>>& vectr)
{
    for (size_t i = 0; i < vectr.size(); ++i) if (vectr[i].empty()) vectr.erase(vectr.begin() + i);
}

char token_to_char(unordered_map<char, long long>& vcb, double tk)
{
    char ch;
    for (const auto& pair : vcb)
    {
        if (pair.second==tk)
        {
            ch = pair.first;
            break;
        }
    }
    return ch;
}

void pairs_to_most_frequent_merge(vector<vector<double>>& pairs, unordered_map<char, long long>& vcb)
{
    
    bool isthat[pairs.size()] = {false};

    vector<Frequency> fre;    

    for (size_t i = 0; i < pairs.size(); ++i)
    {
        if (isthat[i]) continue;
        double ct = 1;
        for (size_t j = i + 1; j < pairs.size(); ++j)
        {
            if (pairs[i][0]==pairs[j][0] && pairs[i][1]==pairs[j][1])
            {
                isthat[j] = true;
                ++ct;
            }
        }
        fre.push_back({pairs[i][0], pairs[i][1], ct});
    }


    for (size_t i = 0; i < fre.size() - 1; ++i)
        for (size_t j = 0; j < fre.size() - i - 1; ++j)
            if (fre[j].ct < fre[j + 1].ct) swap(fre[j], fre[j + 1]);

    long long mul = 1;
    long long num = fre[0].token2;
    
    while(num > 0)
    {
        num /= 10;
        mul *= 10;
    }
    
    fre[0].merge = (fre[0].token1 * mul) + fre[0].token2;
    
    string tk;
    char tk1 = token_to_char(vcb, fre[0].token1);
    char tk2 = token_to_char(vcb, fre[0].token2);
    tk.push_back(tk1);
    tk.push_back(tk2);        

    cout << "Most Frequent : "<< tk1 << " & " << tk2 << " => "<< tk << endl << endl; 

    for (size_t i = 0; i < pairs.size(); ++i)
    {
        if (pairs[i][0]==fre[0].token1 && pairs[i][1]==fre[0].token2)
        {
            erase(pairs[i], fre[0].token1);
            erase(pairs[i], fre[0].token2);
            pairs[i].insert(pairs[i].begin(), fre[0].merge);
            if (i==0)
            {
                erase(pairs[i], fre[0].merge);
                erase(pairs[i + 1], fre[0].token2);
                pairs[i + 1].insert(pairs[i + 1].begin(), fre[0].merge);
            }
            else if(i == (pairs.size() - 1))
            {
                erase(pairs[i], fre[0].merge);
                erase(pairs[i - 1], fre[0].token1);
                pairs[i - 1].insert(pairs[i - 1].begin(), fre[0].merge);                
            }
            else
            {
                erase(pairs[i], fre[0].merge);
                erase(pairs[i - 1], fre[0].token1);
                erase(pairs[i + 1], fre[0].token2);
                pairs[i - 1].insert(pairs[i - 1].end(), fre[0].merge);
                pairs[i + 1].insert(pairs[i + 1].begin(), fre[0].merge);
            }
        }
    }
    
    remove_empty(pairs);

    ofstream merges_file("merges.txt", std::ios::app);
    
    if(!merges_file.is_open()) cout << "Can not open or create merges.txt " << endl;
    else
    {        
        merges_file << tk1 << " " << tk2 << endl;            
        merges_file.close();
    }

    ifstream vocab("vocab.json");
    if (!vocab.is_open()) cout << "vocab.json Can Not Open " << endl;
    else
    {   
        ordered_json data;
        vocab >> data;
        vocab.close();
        
        if (!data.contains(tk))
        {
            long long max_id = -1;
            for (auto& [k, v] : data.items()) max_id = std::max(max_id, v.get<long long>());
            data[tk] = max_id + 1;
        }
        
        std::ofstream out("vocab.json");
        out << data.dump(4);
        out.close();
    }
}