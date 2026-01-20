#include <iostream>
#include "./include/BPE.hpp"
using namespace std;

int main()
{
    Tokenize tk; //Class Object
    
    vector<long long> token;
    vector<string> pair;
    
    tk.fit("../data/test.txt", 100);
    tk.encoding("../data/test2.txt", pair, token);

    for (int i = 0; i < pair.size(); ++i) cout << token[i] << " ";
    cout << endl << endl;
    
    cout << tk.decoding(355) << endl;
    
    return 0;
}