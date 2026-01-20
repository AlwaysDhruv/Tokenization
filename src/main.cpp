#include <iostream>
#include "./include/BPE.hpp"

using namespace std;

int main()
{
    Tokenize tk;
    
    //Encoding
    
    vector<long long> token;
    vector<string> pair;
    
    tk.fit("../data/test.txt", 100);
    tk.encoding("../data/test2.txt", pair, token);

    for (int i = 0; i < pair.size(); ++i) cout << token[i] << " ";
    cout << endl << endl;
    
    //Decoding
    
    cout << tk.decoding(355) << endl << endl;
    
    pair.clear();

    tk.decoding(token, pair);

    for (int i = 0; i < pair.size(); ++i) cout << pair[i] << " ";
    cout << endl << endl;
    
    return 0;
}