#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

double ascii(string, string);

int main()
{
    string text;
    cout << "Enter :- " ;
    getline(cin, text);
    for (int i = 0; i < text.size(); ++i)
    {
        string key(1, text[i]);
        double value = ascii(key ,"vocab.json");
        if (value==1) break;
        else cout << key << " = " << value << endl;
    }
    return 0;
}

double ascii(string key, string path)
{
    double value;
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Failed to open vocab.json\n";
        return 1;
    }
    else
    {
        json data;
        try
        {
            file >> data;
        }
        catch (const json::parse_error& e)
        {
            cerr << "JSON parse error: " << e.what() << "\n";
            return 1;
        }
        if (data.contains(key)) value = data.at(key).get<double>();
        else cout << key << " Not found in this file " << endl;
        file.close();

    }
    return value;
}