#include <iostream>
#include "./include/BPE.hpp"
using namespace std;

int main()
{
    Encoding en;
    
    vector<long long> tk;
    vector<string> pair;
    
    en.fit("../data/test.txt", 100);
    en.encoding("../data/test2.txt", pair, tk);

    for (int i = 0; i < pair.size(); ++i) cout << tk[i] << " ";
    cout << endl << endl;
    
    cout << en.decoding(355) << endl;
    
    return 0;
}