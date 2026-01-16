#include <iostream>
#include "./include/BPE.hpp" //Encoding And Decoding with
using namespace std;

int main()
{
    Tokenize tk; //Class Object
    
    vector<long long> token;
    vector<string> pair;
    
    tk.fit("../data/test.txt", 100); //Training
    tk.encoding("../data/test2.txt", pair, token); // Testing with much of tokens

    for (int i = 0; i < pair.size(); ++i) cout << token[i] << " ";
    cout << endl << endl;
    
    cout << tk.decoding(355) << endl; //Decoding
    
    return 0;
}