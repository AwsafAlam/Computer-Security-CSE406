#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "DES.h"

#define vi vector<int>

using namespace std;

vi data_frame[1000] , ciphered[1000], decrypted[1000];
vi gen_key, decrypt_key[16];
// open a file in write mode.
ofstream outfile;
ifstream infile;

void char_to_binary(int n, int k,int l) 
{ 
    // ascii of binary number 
    int binaryNum[1000]; 
    memset(binaryNum,0,sizeof(binaryNum));
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 

    while(i < 8)
        i++;
    
    // printing binary array in reverse order 
    for (int j = i - 1; j >= 0; j--){
        outfile << binaryNum[j]; 
        data_frame[k].push_back(binaryNum[j]);
        l++;
    } 

} 


string binary_to_char(vi b_str[], int row) 
{
    outfile<<"\ntext data ASCII :\n";

    string ciph;
    for (int i = 0; i < row; i++)
    {
        int k=0, base = 1, sum = 0;
        for (int j = b_str[i].size()-1; j >=0 ; j--)
        {
            sum += b_str[i][j] * base;
            base *= 2;
            k++;
            if(k%8 ==0){
                outfile<<sum<<" ";
                char a = sum;
                ciph.push_back(a);
                sum=0;
                base = 1;
            }
        }
    }
    outfile<<endl;
    return ciph;    
} 

vi applyTransposition(vi b_str, int T[], int size){
    vi modified;
    for (int i = 0; i < size; i++)
    {
        modified.push_back(b_str[ T[i] -1]);
    }
    return modified;    
}

vi leftCircularRotate(vi key, int i){
    int ite = SHIFT[i];
    vi fsthalf , secondhalf;
    for (int i = 0; i < 28; i++)
    {
        fsthalf.push_back(key[i]);
    }
    for (int i = 28; i < 56; i++)
    {
        secondhalf.push_back(key[i]);
    }

    for (int i = 0; i < ite; i++)
    {
        int item = fsthalf.front();
        fsthalf.erase(fsthalf.begin(),fsthalf.begin()+1);
        fsthalf.push_back(item);
        
        item = secondhalf.front();
        secondhalf.erase(secondhalf.begin(),secondhalf.begin()+1);
        secondhalf.push_back(item);
    }
    for (int i = 0; i < 28; i++)
    {
        fsthalf.push_back(secondhalf[i]);
    }
    return fsthalf;
}


vi XOR(vi a, vi b){
    vi tmp;
    int i =0;
    
    for(int i = 0; i < b.size(); i++)
    {
        if(a[i] == b[i])
            tmp.push_back(0);
        else
            tmp.push_back(1);
        // tmp.push_back(a[i]^b[i]);
    }
    return tmp;
}

vi encryption(int row ,bool flag){
    
    /// Apply transpose PI[] to data ------------
    vi new_data = applyTransposition(data_frame[row] , PI , 64);
    vi Li;
    vi Ri;
    for (int i = 0; i < 32; i++){
        Li.push_back(new_data[i]);
        Ri.push_back(new_data[i + 32]);
    }
    
    /// Start iterations ----
    for (int i = 0; i < 16; i++)
    {
        outfile<<"\n.......... Starting = ";
        outfile<<i+1<<"th iteration ................\n";
        vi new_key;
        if(flag)
            new_key = decrypt_key[i];
        else
            new_key = decrypt_key[15-i];
        
        vi temp = Li;
        Li = Ri;
        
        outfile<<"Expanded: ";
        vi expanded = applyTransposition(Ri, E, 48);
        for (int i = 0; i < expanded.size(); i++)
            outfile<<expanded[i];

        // outfile<<"\n-- Apply swap Li = Ri-1 -----------------"<<endl;
        

        outfile<<"\nXORed: ";
        vi xor_ed = XOR(expanded,new_key);
        for (int i = 0; i < xor_ed.size(); i++)
            outfile<<xor_ed[i];

        outfile<<"\nPI_2: ";
        expanded.clear();
        expanded = applyTransposition(xor_ed , PI_2 , 32);
        for (int i = 0; i < expanded.size(); i++)
            outfile<<expanded[i];

        outfile<<"\nP_BOX : ";
        xor_ed.clear();
        xor_ed = applyTransposition(expanded,P,32);
        for (int i = 0; i < xor_ed.size(); i++)
            outfile<<xor_ed[i];

        outfile<<"\nXOR oldR and new: ";
        
        expanded.clear();
        expanded = XOR(xor_ed , temp);
        for (int i = 0; i < expanded.size(); i++)
            outfile<<expanded[i];

        Ri.clear();
        Ri = expanded;
        // ============================================
        outfile<<"\n\nLi :";
        for (int i = 0; i < Li.size(); i++)
            outfile<<Li[i];

        outfile<<"\nRi :";
        for (int i = 0; i < Ri.size(); i++)
            outfile<<Ri[i];

    }
    
    /// Swap L and R of data ----
    // vi temp = Li;
    // Ri = Li;
    // Li = temp;

    /// Apply transpose PI_1[] to data ---
    new_data.clear();
    for (int i = 0; i < 64; i++)
    {
        if(i<32)
            new_data.push_back(Li[i]);
        else
            new_data.push_back(Ri[i-32]);
    }
    Li.clear();
    Li = applyTransposition(new_data , PI_1 , 64);
        
    return Li;
}


void generate_key(vi key){
    /// Apply transpose CP_1[] to key ------------
    vi new_key = applyTransposition(key,CP_1,56);

    for (int i = 0; i < 16; i++)
    {
        //outfile<<"\n--  circular Left shift key ------"<<endl;
        vi temp = leftCircularRotate(new_key,i);
        // new_key.clear();
        // new_key = temp;
        
        // outfile<<"\n--  Apply transpose CP_2[] to key -----------------"<<endl;
        vi transposed = applyTransposition(temp,CP_2,48);
        outfile<<"Key "<<i+1<<" : ";
        for (int j = 0; j < transposed.size(); j++)
        {
            outfile<<transposed[j];
            decrypt_key[i].push_back(transposed[j]);
        }
        outfile<<endl;
    }
}

int main(int argc, char const *argv[])
{

    string plaintext,key;
    int char_sz = 8;
    outfile.open("out.txt");
    infile.open("input.txt");
    
    // cout<<"enter key: \n";
    getline(infile,key);

    // cout<<"Enter message : \n";
    getline(infile,plaintext);
    
    /// Padding data string ----------
    while(plaintext.length() %char_sz != 0 ){
        plaintext += "~";
    }
    outfile<<"\ndata string after padding: "<<plaintext<<endl;

    /// Converting key to binary -----
    outfile<<"\nconverting key to binary: "<<endl;
    for(int i = 0; i < key.length(); i++)
        char_to_binary(key[i], 0, 0);

    for(int i = 0; i < data_frame[0].size() ; i++)
        gen_key.push_back(data_frame[0][i]);

    outfile<<endl;
    data_frame[0].clear();
    
    generate_key(gen_key);
    
    int block_sz = 64;
    int total_bits = plaintext.length()*8;
    int row_no = total_bits/block_sz;
    
    /// Printing data block -----------
    outfile<<"\ndata block <ascii code of 64 bits per row>:\n";
    int k = 0;
    for(int i = 0; i < plaintext.length(); i++)
    {
        for(int j = 0; j < char_sz; j++)
        {
            char_to_binary(plaintext[i],k,j*8);
            if(j != char_sz-1)
                i++;
            if(i == plaintext.length())
                break;
        }
        k++;
        outfile<<endl;
    }
    outfile<<"\nRows : "<<k<<endl;

    
    for (int i = 0; i < row_no; i++)
    {
        ciphered[i] = encryption(i , true);
    }
    
    /// Printing data block -----------
    outfile<<"\nCiphered binary data <after encryption>:\n";
    for (int i = 0; i < row_no; i++)
    {
        for (int j = 0; j < ciphered[i].size(); j++)
        {
            outfile<<ciphered[i][j];
        }
        outfile<<endl;
    }
    /// Binary to char -----------
    string cipheredtxt = binary_to_char(ciphered, row_no);
    outfile<<"\nCiphered text data <after encryption>:\n"<<cipheredtxt<<endl;

    /// Decryption ----
    outfile<<".................. Starting Decryption ....................\n";
    for (int i = 0; i < row_no; i++)
    {
        decrypted[i] = encryption(i , false);
    }

    /// Printing data block -----------
    outfile<<"\nDeciphered binary data <after decryption>:\n";
    for (int i = 0; i < row_no; i++)
    {
        for (int j = 0; j < decrypted[i].size(); j++)
        {
            outfile<<decrypted[i][j];
        }
        outfile<<endl;
    }
    /// Binary to char -----------
    string decipheredtxt = binary_to_char(decrypted, row_no);
    outfile<<"\ndeCiphered text data <after decryption>:\n"<<decipheredtxt<<endl;

    // close the opened file.
    outfile.close();

    return 0;
}