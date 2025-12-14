#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

int main()
{
    ifstream file("vocab.json");
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
    string c;
    cout << "Enter :- ";
    cin >> c;
    if (data.contains(c)) cout << data[c].dump(4);
    return 0;
}