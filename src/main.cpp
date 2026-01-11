#include <iostream>
#include "./include/Encoding.hpp"
using namespace std;

int main()
{
    Encoding en;
    
    vector<long long> tk;
    vector<string> pair;
    
    //en.fit("../data/test.txt", 5);
    en.encoding("../data/test2.txt", pair, tk);
    return 0;
}