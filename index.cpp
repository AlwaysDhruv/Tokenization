#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;
using ordered_json = nlohmann::ordered_json;

static unordered_map<long long, string> most;

string byte_to_key(unsigned char);
char token_to_char(unordered_map<unsigned char, long long>&, long long);
int tokens_to_pairs(vector<long long>&,vector<vector<long long>>&);
void fetch_json_data(unordered_map<unsigned char, long long>&);
template <typename vectr> void display(vector<vector<vectr>>&);
template <typename empty> void remove_empty(vector<vector<empty>>&);
int fetch_text_data_to_tokens(unordered_map<unsigned char, long long>&, vector<long long>&, string);
void pairs_to_most_frequent_merge(vector<vector<long long>>&, unordered_map<unsigned char, long long>&);

struct Frequency
{
    long long token1, token2, ct, merge;
};

int main()
{
    unordered_map<unsigned char, long long> vocab;
    fetch_json_data(vocab);

    vector<long long> tokens;
    fetch_text_data_to_tokens(vocab, tokens, "test.txt");

    vector<vector<long long>> pair;
    tokens_to_pairs(tokens, pair);
    
    pairs_to_most_frequent_merge(pair, vocab);

    return 0;
}

template <typename empty> void remove_empty(vector<vector<empty>>& vectr)
{
    for (size_t i = 0; i < vectr.size(); ++i) if (vectr[i].empty()) vectr.erase(vectr.begin() + i);
}

template <typename vectr> void display(vector<vector<vectr>>& vec)
{
    for (auto& values : vec)
    {
        for (auto& value : values) cout << value << " ";
        cout << endl;
    }
}


string byte_to_key(unsigned char b)
{
    if (b >= 32 && b <= 126 && b != '"' && b != '\\')
    {
        return string(1, static_cast<char>(b));
    }

    ostringstream oss;
    oss << "\\u"
        << std::hex << std::setw(4) << std::setfill('0')
        << static_cast<int>(b);

    return oss.str();
}

void fetch_json_data(unordered_map<unsigned char, long long>& vcb)
{
    if (!fs::exists("vocab.json"))
    {
        ordered_json vocab = ordered_json::object();
    
        for (int i = 0; i <= 255; ++i)
        {
            unsigned char b = static_cast<unsigned char>(i);
            string key = byte_to_key(b);
    
            vocab[key] = static_cast<long long>(i);
        }
    
        std::ofstream out("vocab.json");
        out << vocab.dump(4);
        out.close();

        cout << "vocab.json Created Sucessfully..." << endl;

        ifstream file("vocab.json");
        json data;
        file >> data;

        file.close();

        for(auto& [key, values] : data.items())
        {
            if (key.size() == 1)
                vcb[key[0]] = values.get<long long>();
        }

        cout << "Data Fetched Sucessfully.." << endl;

    }
    else
    {
        ifstream file("vocab.json");
        json data;
        file >> data;
        file.close();
        for(auto& [key, values] : data.items())
        {
            if (key.size() == 1)
                vcb[key[0]] = values.get<long long>();
        }

        cout << "Data Fetched Sucessfully.." << endl;
    }
}

int fetch_text_data_to_tokens(unordered_map<unsigned char, long long>& vcb, vector<long long>& tk, string path)
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

int tokens_to_pairs(vector<long long>& tokens ,vector<vector<long long>>& pairs)
{
    int ct = 0;
    vector<long long> pair;
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

char token_to_char(unordered_map<unsigned char, long long>& vcb, long long tk)
{
    char ch;
    bool test = false;
    for (const auto& pair : vcb)
    {
        if (pair.second==tk)
        {
            ch = pair.first;
            test = true;
            break;
        }
    }
    if (test==false)
    {
        for (const auto& pair : most)
        {
            if (pair.first==tk)
            {
                cout << pair.first << " " << pair.second << endl;
                ch = pair.second;
                test = true;
                break;
            }
        }
    }
    return ch;
}

void pairs_to_most_frequent_merge(vector<vector<long long>>& pairs, unordered_map<unsigned char, long long>& vcb)
{
    for (int i = 0; i < 3; ++i)
    {
        display(pairs);
        cout << endl << endl;

        if (pairs.size()!=1)
        {
            bool isthat[pairs.size()] = {false};
            
            vector<Frequency> fre;    
        
            for (size_t i = 0; i < pairs.size(); ++i)
            {
                if (isthat[i]) continue;
                long long ct = 1;
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
            cout << fre[0].token1 << " " << fre[0].token2 << endl;

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
                    
                    most[fre[0].merge] = tk;
        
                    std::ofstream out("vocab.json");
                    out << data.dump(4);
                    out.close();
        
                    ofstream merges_file("merges.txt", std::ios::app);
            
                    if(!merges_file.is_open()) cout << "Can not open or create merges.txt " << endl;
                    else
                    {        
                        merges_file << tk1 << " " << tk2 << endl;            
                        merges_file.close();
                    }    
                }
            }
        }
    }
}