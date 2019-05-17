#include <bits/stdc++.h> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#define vi vector<int>
#define vs vector<string>

using namespace std;

ofstream outfile;
ifstream infile;

vector<vi> allComb;
int noOfcomb = 0;

string vec_to_str(vector<char> mat[], int col){
	string tmp;
	for(int i = 0 ; i < mat[0].size() ; i++){
		for (int j = 0; j < col; j++)
		{
			tmp.push_back(mat[j][i]);
		}
	}
	return tmp;
}

int findMatch(string cipher , vs hints){
	int score = 0;
	for (int i = 0; i < hints.size(); i++)
	{
		if (cipher.find(hints[i]) != string::npos) {
			score++;
		}
	}
	return score;
}

vi findColNumber(int cipher_len){
	vi col;
	outfile<<"col: ";
	for(int i = 2 ; i < cipher_len; i++){
		if(cipher_len % i == 0){
			col.push_back(i);
			outfile<<i<<" ";
		}
	}
	outfile<<endl;
	return col;	
}

int main() {
	
	string plaintext, ciphertext;
	vi col_Combinations, key;
	int noOfhints , key_len, col, col_len;
	vs hintList;

	infile.open("transposition-114.txt");
    outfile.open("output.txt");

	infile>>ciphertext;	
	cout<<"Encrypted : "<<ciphertext<<endl;
	cout<<"Length : "<<ciphertext.size()<<endl;

	string secondLine;
	infile>>secondLine;
	secondLine.pop_back();
	cout<<secondLine<<endl; 
	transform(secondLine.begin(), secondLine.end(),secondLine.begin(), ::toupper);
	hintList.push_back(secondLine);
	secondLine.clear();
    getline(infile,secondLine);
	secondLine.erase(remove(secondLine.begin(), secondLine.end(), '\n'), secondLine.end());
	secondLine.erase(remove(secondLine.begin(), secondLine.end(), ' '), secondLine.end());

	stringstream ss(secondLine);
	string token;
	while (getline(ss,token, ','))
	{
		//token.erase(token.begin() , token.begin()+1);
		cout<<token<<endl; 
		transform(token.begin(), token.end(),token.begin(), ::toupper);
		hintList.push_back(token);
	}
	hintList[hintList.size() -1 ].pop_back();
	noOfhints = hintList.size();
	// for (int i = 0; i < noOfhints; i++)
	// {
	// 	string ht;
	// 	infile>>ht;
	// 	transform(ht.begin(), ht.end(),ht.begin(), ::toupper);
	// 	hintList.push_back(ht);	
	// }
	
	outfile<<"Starting decryption of cipher text----------\n\n"<<ciphertext<<endl;
	outfile<<"\nHints : \n";
	for (int i = 0; i < hintList.size(); i++)
		outfile<<"|"<<hintList[i]<<"|"<<endl;

	col_Combinations = findColNumber(ciphertext.length());
	
	bool flag = true;
	for (int lt = 0; lt < col_Combinations.size(); lt++)
	{
		col = col_Combinations[lt];
		col_len = ciphertext.size() / col;
		allComb.clear();
		vector<char> colMatrix[col];

		outfile<<"\nDividing cipher into columns ---------\n\n";
		int k=0;
		for(int i = 0 ; i< col ; i++){
			for (int j = 0; j < col_len; j++)
			{
				colMatrix[i].push_back(ciphertext[k]);
				outfile<<ciphertext[k];
				k++;
				if( k >= ciphertext.length())
					break;
			}
			outfile<<endl;
		}
		
		int a[col];
		for (int i = 0; i < col; i++)
			a[i] = i;
		
		
		do {
			vi item(a , a+col); 
			allComb.push_back(item);
			noOfcomb++;
		} while (next_permutation(a, a + col)); 

		int max  = 0,i = 0;
		for (i = 0; i < allComb.size(); i++)
		{
			vector<char> tmpMatrix[col];
			outfile<<"\n\n--------- For combination : ";
			for (int j = 0; j < allComb[i].size(); j++)
			{
				tmpMatrix[j] = colMatrix[allComb[i][j]];
				outfile<<allComb[i][j];
			}	
			outfile<<"\n\n String generated : \n";
			string new_str = vec_to_str(tmpMatrix , col);
			outfile<<new_str<<endl;

			int match_score = findMatch(new_str , hintList);
			
			if( match_score > max){
				max = match_score;
			}
			if(match_score == hintList.size()){
				plaintext = new_str;
				flag = false;
				key = allComb[i];
				key_len = allComb[i].size();
				break;
			}
		}

		if(!flag){
			cout<<"Key Length :"<<key_len<<endl;
			outfile<<"Key Length :"<<key_len<<endl;
			cout<<"Key combination : ";
			outfile<<"Key combination : ";
			for (int j = 0; j < key.size(); j++)
				cout<<key[j]<<" ";
			for (int j = 0; j < key.size(); j++)
				outfile<<key[j]<<" ";
			
			
			cout<<"\nTotal NO of Combinations: "<<noOfcomb<<endl;
			outfile<<"\nTotal NO of Combinations: "<<noOfcomb<<endl;
			cout<<"NO of Combinations Explored: "<<i<<endl;	
			outfile<<"NO of Combinations Explored: "<<i<<endl;	
			break;
		}
			
	}
	
	transform(plaintext.begin(), plaintext.end(),plaintext.begin(), ::tolower);
	cout<<"\nDecrypted txt: "<<plaintext<<endl;
	outfile<<"\nDecrypted txt: "<<plaintext<<endl;

	// Re encrypting --- 
	transform(plaintext.begin(), plaintext.end(),plaintext.begin(), ::toupper);
	outfile<<"\nDividing plain txt into columns ---------\n\n";
	
	int k=0;
	vector<char> colMatrix[col], tmpMatrix[col];
	for (int j = 0; j < col_len; j++)
	{
		for(int i = 0 ; i< col ; i++){
			colMatrix[i].push_back(plaintext[k]);
			k++;
			if( k >= plaintext.length())
				break;
		}
		if( k >= plaintext.length())
			break;
	}
	outfile<<"Encrypting :\n\n";
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < colMatrix[i].size(); j++)
			outfile<<colMatrix[i][j];
		outfile<<endl;
	}
	
	for (int j = 0; j < key.size(); j++)
	{
		tmpMatrix[key[j]] = colMatrix[j];
	}
	cout<<"\n-------------------------------\nRearranging according to key:\n\n";
	string new_str;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < tmpMatrix[i].size(); j++){
			cout<<tmpMatrix[i][j];
			outfile<<tmpMatrix[i][j];
			new_str += tmpMatrix[i][j];
		}
		outfile<<endl;
		cout<<endl;
	}
	outfile<<"\nEncrypted again : \n";
	cout<<"\nEncrypted again : \n";
	outfile<<new_str<<endl;
	cout<<new_str<<endl;
	int count  = 0;
	for (int i = 0; i < new_str.length(); i++)
	{
		if(new_str[i] != ciphertext[i])
			count++;
	}
	double acc = (new_str.length()-count);
	outfile<<"Accuracy :"<<(acc/new_str.length()) * 100<<"\%"<<endl;
	cout<<"Accuracy :"<<(acc/new_str.length()) * 100<<"\%"<<endl;

	return 0;
}