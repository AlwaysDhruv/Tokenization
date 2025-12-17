#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void fetch_json_data(unordered_map<char, double>&, string);

int main()
{
    unordered_map<char, double> vocab;
    fetch_json_data(vocab, "vocab.json");
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