#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include "../utils/json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;
using ordered_json = nlohmann::ordered_json;


struct Frequency
{
    long long token1, token2, ct, merge;
};

class Tokenize
{
	private:
		
		vector<long long> tokens;
	    vector<string> input_pair;
	    vector<vector<string>> merges;
		vector<vector<long long>> pair;
		unordered_map<string, long long> most;
		unordered_map<string, long long> vocab;
	
		string path = "../model/merges.txt";
		string vocablury = "../model/vocab.json";
	
	public:
		
		void decoding(vector<long long>& tk, vector<string>& out)
		{
			vocab.clear();
			fetch_json_data(vocab);
	
			for (const auto& pair : vocab)
			{
				for (size_t i = 0; i < tk.size(); ++i)
				{
					if (pair.second==tk[i])
					{
						out.push_back(pair.first);
						break;
					}
				}			
			}
		}

		string decoding(long long tk)
		{
			string token;	
			bool flage = false;		
			
			vocab.clear();
			fetch_json_data(vocab);
			
			for (const auto& pair : vocab)
			{
				if (pair.second==tk)
				{
					token = pair.first;
					flage = true;
					break;
				}
			}
			
			if (flage==false) throw runtime_error("Token id not found");
			
			return token;
		}
		
		void fit(string input, long long train)
		{
		    fetch_json_data(vocab);
	    
		    fetch_text_data_to_tokens(vocab, tokens, input);
		
		    tokens_to_pairs(tokens, pair);
	
		    pairs_to_most_frequent_merge(pair, vocab, train);
		}
		
		template <typename empty> void remove_empty(vector<vector<empty>>& vectr)
		{
		    for (size_t i = 0; i < vectr.size(); ++i)
		        if (vectr[i].empty()) vectr.erase(vectr.begin() + i);
		}
	
		void preprocess_to_pairs(vector<vector<long long>>& vectr)
		{
		    for (size_t i = 0; i < vectr.size(); ++i)
		        if (vectr[i].size() <= 1)
		        {
		            if (vectr.size() - 1) vectr[i].push_back(vectr[i][0]);
		            else vectr[i].push_back(vectr[i + 1][0]);
		        }
		}
		
		string byte_to_key(unsigned char b)
		{
		    if (b >= 32 && b <= 126 && b != '"' && b != '\\') return string(1, static_cast<char>(b));
		
		    if (b < 128) return string(1, static_cast<char>(b));
		
		    char first  = static_cast<char>(0xC0 | (b >> 6));
		    char second = static_cast<char>(0x80 | (b & 0x3F));
		
		    return string{first, second};
		}
		
		int fetch_text_data_to_tokens(unordered_map<string, long long>& vcb, vector<long long>& tk, string path)
		{
		    ifstream file(path);
		    if (!file.is_open()) cout << path << " Can Not Open " << endl;
		    else
		    {
		        unsigned char byte;
		        while (file.read(reinterpret_cast<char*>(&byte), 1))
		        {
		            if (byte == '\n' || byte == '\r')
		                continue;
		            string key = byte_to_key(byte);
		            auto it = vcb.find(key);
		            
		            if (it == vcb.end())
		                throw runtime_error("Byte not found in vocab");
		            tk.push_back(it->second);
		        }
		        cout << "Text Converts To Tokens From " << path << " Sucessfully..." << endl;
		    }
		    return tk.size();
		}
		
		template <typename pairing> int tokens_to_pairs(vector<pairing>& tokens ,vector<vector<pairing>>& pairs)
		{
		    int ct = 0;
		    vector<pairing> pair;
		    for (size_t i = 0; i < tokens.size(); ++i)
		    {
		        if (i!=(tokens.size() - 1))
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
		        else
		        {
		            pairs.push_back(pair);
		            pair.clear();
		            pair.push_back(tokens[i - 1]);
		            pair.push_back(tokens[i]);
		            pairs.push_back(pair);
		            pair.clear();
		        }
		    }
		    cout << pairs.size() << " Pairs Created Sucessfully..." << endl;
		    return pairs.size();
		}
		
		string token_to_char(unordered_map<string, long long>& vcb, long long tk)
		{
		    string ch;
		    bool test = false;
		    for (const auto& pair : vcb)
		    {
		        if (pair.second==tk)
		        {
		            if (pair.second==32)
		            {
		                ch = "Ġ";
		                test = true;
		                break;                
		            }
		            else
		            {
		                ch = pair.first;
		                test = true;
		                break;
		            }
		        }
		    }
		    if (test==false)
		    {
		        for (const auto& pair : most)
		        {
		            if (pair.second==tk)
		            {
		                if (pair.second==32)
		                {
		                    ch = "Ġ";
		                    test = true;
		                    break;                
		                }
		                else
		                {
		                    ch = pair.first;
		                    test = true;
		                    break;
		                }
		            }
		        }
		    }
		    return ch;
		}
		
		void fetch_json_data(unordered_map<string, long long>& vcb)
		{
	        fs::path dir_path = "../model";
	        
	        fs::create_directories(dir_path);
		
		    if (!fs::exists(vocablury))
		    {
		        ordered_json vocab = ordered_json::object();
		    
		        for (size_t i = 0; i <= 255; ++i)
		        {
		            unsigned char b = static_cast<unsigned char>(i);
		            string key = byte_to_key(b);
		    
		            vocab[key] = static_cast<long long>(i);
		        }
		    
		        ofstream out(vocablury);
		        out << vocab.dump(4);
		        out.close();
		
		        cout << "vocab.json Created Sucessfully..." << endl;
		
		        ifstream file(vocablury);
		        ordered_json data;
		        file >> data;
		
		        file.close();
		
		        for(auto& [key, values] : data.items()) vcb[key] = values.get<long long>();
		
		        cout << "Data Fetched Sucessfully.." << endl;
		
		    }
		    else
		    {
		        if (fs::file_size(vocablury)!=0)
		        {
		            ifstream file(vocablury);
		            ordered_json data;
		            file >> data;
		            file.close();
		            for(auto& [key, values] : data.items()) vcb[key] = values.get<long long>();
		            cout << "Data Fetched Sucessfully.." << endl;
		        }
		        else
		        {
		            ordered_json vocab = ordered_json::object();
		        
		            for (size_t i = 0; i <= 255; ++i)
		            {
		                unsigned char b = static_cast<unsigned char>(i);
		                string key = byte_to_key(b);
		        
		                vocab[key] = static_cast<long long>(i);
		            }
		        
		            ofstream out(vocablury);
		            out << vocab.dump(4);
		            out.close();
		    
		            cout << "vocab.json Created Sucessfully..." << endl;
		    
		            ifstream file(vocablury);
		            ordered_json data;
		            file >> data;
		    
		            file.close();
		    
		            for(auto& [key, values] : data.items()) vcb[key] = values.get<long long>();
		    
		            cout << "Data Fetched Sucessfully.." << endl;
		        }
		    }
		}
	
		void encoding(string input_file, vector<string>& pa, vector<long long>& tokens_ids)
		{
	    	ifstream file(input_file);
	    	
	    	if (fs::exists(input_file) && fs::exists(path) && fs::exists(vocablury))
	    	{
		   		string line;
	    		while(getline(file, line))
	    		{
	    		    for (size_t i = 0; i < line.size(); ++i) 
	    		    	if(line[i]==' ') input_pair.push_back("Ġ");
	    		    	else input_pair.push_back(string(1, line[i]));
	    		    line.clear();
	    		}

				line.clear();
	    		vector<string> chars;
		
	    		ifstream merge(path);
	    		
	    		while(true)
	    		{
	    			while(getline(merge, line))
	    			{
	    			    for (size_t i = 0; i < input_pair.size(); ++i)
	    			    {
	    			    	string me = "";
	   			            for (size_t j = 0; j < line.size(); ++j)
	   			            {
	   			                if (line[j]==' ')
	   			                {
	   			                    chars.push_back(me);
	   			                    me.clear();
	   			                    continue;
	   			                }
	   			                else me += line[j];
	   			            }
	   			            chars.push_back(me);
	   			            me.clear();
	   			            break;
	    			    }
	    			    merges.push_back(chars);
	    			    chars.clear();
	    			}
					
					if (merges.size() > 0)
					{
						for (size_t i = 0; i < merges.size(); ++i)
						{
							int fg = 0;
							for (size_t j = 0; j < input_pair.size() - 1; ++j)
							{
								if (merges[i][0]==input_pair[j] && merges[i][1]==input_pair[j + 1])
								{
									input_pair[j] = input_pair[j] + input_pair[j + 1];
									input_pair.erase(input_pair.begin() + j + 1);
									fg = 1;
								}
							}
							if(fg==1) i = 0;
						}
						break;
					}
					else break;
	    		}
				ifstream f(vocablury);
		
				json data = json::parse(f);
		
				for (size_t i = 0; i < input_pair.size(); ++i)
				{
					if (input_pair[i]=="Ġ")
					{
						pa.push_back(" ");
						tokens_ids.push_back(data[" "]);
					}
					else
					{
						pa.push_back(input_pair[i]);
						tokens_ids.push_back(data[input_pair[i]]);					
					}
				}
	    	}
	    	else cout << input_file << " or " << path << " or " << vocablury << " Doesn't Exist!!" << endl;
		}
	
		void pairs_to_most_frequent_merge(vector<vector<long long>>& pairs, unordered_map<string, long long>& vcb, long long n)
		{
		    for (size_t i = 0; i < n; ++i)
		    {
		        try
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
		                               
		            	            
		            ifstream vocab(vocablury);
		            
		            if (!vocab.is_open()) cout << "vocab.json Can Not Open " << endl;
		            else
		            {   
		            	string tk1 = token_to_char(vcb, fre[0].token1);
		            	string tk2 = token_to_char(vcb, fre[0].token2);
		            	string tk = tk1 + tk2;
		                
		                ordered_json data;
		                vocab >> data;
		                vocab.close();
		                
		                if (!data.contains(tk))
		                {
		                    long long max_id = -1;
		
		                    for (auto& [k, v] : data.items()) max_id = max(max_id, v.get<long long>());
		                    
		                    data[tk] = max_id + 1;
		        
		                    most[tk] = max_id + 1;
		            
		                    fre[0].merge = max_id + 1;
		        
		                    ofstream out(vocablury);
		                    out << data.dump(4);
		                    out.close();
		        
		                    ofstream merges_file(path, ios::app);
		            
		                    if(!merges_file.is_open()) cout << "Can not open or create merges.txt " << endl;
		                    else
		                    {        
		                        merges_file << tk1 << " " << tk2 << endl;            
		                        merges_file.close();
		                    }    
		        
				            cout << i + 1 << ". " << tk1 << " : " << fre[0].token1 << " & " << tk2 << " : " << fre[0].token2 << " => "<< tk << " : " << fre[0].merge << endl; 
				            
				            if (pairs.size() != 1)
				            {
				                for (size_t i = 0; i < pairs.size(); ++i)
				                {
				                    if (pairs[i][0]==fre[0].token1 && pairs[i][1]==fre[0].token2)
				                    {
				                        std::erase(pairs[i], fre[0].token1);
				                        std::erase(pairs[i], fre[0].token2);
				                        pairs[i].insert(pairs[i].begin(), fre[0].merge);
				                        if (i==0)
				                        {
				                            std::erase(pairs[i], fre[0].merge);
				                            auto it = find(pairs[i + 1].begin(), pairs[i + 1].end(), fre[0].token2);
				                            if (it != pairs[i + 1].end()) pairs[i + 1].erase(it);
				                            pairs[i + 1].insert(pairs[i + 1].begin(), fre[0].merge);
				                        }
				                        else if(i == (pairs.size() - 1))
				                        {
				                            std::erase(pairs[i], fre[0].merge);
				                            std::erase(pairs[i - 1], fre[0].token1);
				                            pairs[i - 1].insert(pairs[i - 1].begin(), fre[0].merge);
				                        }
				                        else
				                        {
				                            std::erase(pairs[i], fre[0].merge);
				                            std::erase(pairs[i - 1], fre[0].token1);
				                            std::erase(pairs[i + 1], fre[0].token2);
				                            pairs[i - 1].insert(pairs[i - 1].end(), fre[0].merge);
				                            pairs[i + 1].insert(pairs[i + 1].begin(), fre[0].merge);
				                        }
				                    }
				                }
				                remove_empty(pairs);
				                preprocess_to_pairs(pairs);
				            }
				            else break;
		                }
		                else cout << i + 1 << " Already Exist In " << path << "!!" << endl;
		            }
		        }
		        catch (const runtime_error& e)
		        {
		            cerr << "Error: " << e.what() << endl;
		        }
		    }
		}	
};
#endif