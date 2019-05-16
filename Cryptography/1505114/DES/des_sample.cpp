/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>

typedef long long int ll;
typedef unsigned long long int ull;

#define dbg printf("in\n")
#define nl printf("\n")
#define N 64
#define N2 56

#define pb push_back

using namespace std;

//===================================================================
// variables
//===================================================================

string keys[17];
string tempR, current_round_key, e, tempKey;
string oldL, oldR, newL, newR;
string key, plain_text, ciphered_text, deciphered_text;
vector<string> text_chunks;

int PI[N] = {58, 50, 42, 34, 26, 18, 10, 2,
             60, 52, 44, 36, 28, 20, 12, 4,
             62, 54, 46, 38, 30, 22, 14, 6,
             64, 56, 48, 40, 32, 24, 16, 8,
             57, 49, 41, 33, 25, 17, 9, 1,
             59, 51, 43, 35, 27, 19, 11, 3,
             61, 53, 45, 37, 29, 21, 13, 5,
             63, 55, 47, 39, 31, 23, 15, 7};

int CP_1[N2] = {57, 49, 41, 33, 25, 17, 9,
                1, 58, 50, 42, 34, 26, 18,
                10, 2, 59, 51, 43, 35, 27,
                19, 11, 3, 60, 52, 44, 36,
                63, 55, 47, 39, 31, 23, 15,
                7, 62, 54, 46, 38, 30, 22,
                14, 6, 61, 53, 45, 37, 29,
                21, 13, 5, 28, 20, 12, 4};

int CP_2[48] = {14, 17, 11, 24, 1, 5, 3, 28,
                15, 6, 21, 10, 23, 19, 12, 4,
                26, 8, 16, 7, 27, 20, 13, 2,
                41, 52, 31, 37, 47, 55, 30, 40,
                51, 45, 33, 48, 44, 49, 39, 56,
                34, 53, 46, 42, 50, 36, 29, 32};

int E[48] = {32, 1, 2, 3, 4, 5,
             4, 5, 6, 7, 8, 9,
             8, 9, 10, 11, 12, 13,
             12, 13, 14, 15, 16, 17,
             16, 17, 18, 19, 20, 21,
             20, 21, 22, 23, 24, 25,
             24, 25, 26, 27, 28, 29,
             28, 29, 30, 31, 32, 1};

int PI_2[32] = {35, 38, 46, 6, 43, 40, 14, 45,
                33, 19, 26, 15, 23, 8, 22, 10,
                12, 11, 5, 25, 27, 21, 16, 31,
                28, 32, 34, 24, 9, 37, 2, 1};

int PI_1[N] = {40, 8, 48, 16, 56, 24, 64, 32,
               39, 7, 47, 15, 55, 23, 63, 31,
               38, 6, 46, 14, 54, 22, 62, 30,
               37, 5, 45, 13, 53, 21, 61, 29,
               36, 4, 44, 12, 52, 20, 60, 28,
               35, 3, 43, 11, 51, 19, 59, 27,
               34, 2, 42, 10, 50, 18, 58, 26,
               33, 1, 41, 9, 49, 17, 57, 25};

int P[32] = {16, 7, 20, 21, 29, 12, 28, 17,
             1, 15, 23, 26, 5, 18, 31, 10,
             2, 8, 24, 14, 32, 27, 3, 9,
             19, 13, 30, 6, 22, 11, 4, 25};

int SHIFT[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

//===================================================================
// helper functions
//===================================================================
void init() {
  oldL.resize(32), oldR.resize(32);
  newL.resize(32), newR.resize(32);
  current_round_key.resize(48), e.resize(48), tempR.resize(32);
}

string make64Bits(string s) {

  string str = s;
  while (str.length() % 8 != 0)
    str.pb('~');

  return str;
}

void group64BitText(string str) {
  text_chunks.clear();

  int n = str.length();
  string s = "";

  for (int i = 0; i < n; i++) {
    s.pb(str[i]);

    if ((i + 1) % 8 == 0)
      text_chunks.pb(s), s = "";
  }
}

string to8BitBinary(int n) {
  string s = "";
  while (n) {
    if (n % 2)
      s.pb('1');
    else
      s.pb('0');

    n /= 2;
  }

  while (s.length() < 8)
    s.pb('0');

  reverse(s.begin(), s.end());
  return s;
}

string strToBit(string s) {
  string bset;
  int n = s.length(), k;
  unsigned char ch;

  for (int i = 0; i < n; i++) {
    ch = s[i];
    k = ch;
    bset += to8BitBinary(k);
  }
  //cout<<"Bin : "<<s<<" "<<bset<<endl;
  return bset;
}

string bitToStr(string b) {
  int p, sum;
  string str = "";

  for (int i = 64 - 1; i >= 0; i -= 8) {
    sum = 0, p = 1;
    for (int j = i; j > i - 8; j--) {
      if (b[j] == '1')
        sum += p;

      p *= 2;
    }
    //cout<<sum<<" ";
    str.pb(char(sum));
  }

  reverse(str.begin(), str.end());
  return str;
}

void make56BitKey() {
  string bset = strToBit(key);
  for (int i = 0; i < N2; i++) {
    keys[0].pb(bset[CP_1[i] - 1]);
  }
}

//===================================================================
// encryption-decryption functions
//===================================================================

void generateKeys() {
  bitset<28> kL, kR;

  for (int i = 1; i <= 16; i++) {

    //divide the 56-bit key into two parts
    int k = 27;
    for (int j = 0; j < 28; j++)
      kL[k--] = keys[0][j] - 48;

    k = 27;
    for (int j = 28; j < 56; j++)
      kR[k--] = keys[0][j] - 48;

    //circular shift the two parts(each of 28bits)
    kR = (kR << SHIFT[i - 1]) | (kR >> (28 - SHIFT[i - 1]));
    kL = (kL << SHIFT[i - 1]) | (kL >> (28 - SHIFT[i - 1]));

    //using the 28bits make 56bit again
    tempKey.clear();
    for (int j = 28 - 1; j >= 0; j--) {
      if (kL[j])
        tempKey.pb('1');
      else
        tempKey.pb('0');
    }

    for (int j = 28 - 1; j >= 0; j--) {
      if (kR[j])
        tempKey.pb('1');
      else
        tempKey.pb('0');
    }

    //transform
    // cout<<"\nKey "<<i<<" :";
    for (int j = 0; j < 48; j++){
      keys[i].pb(tempKey[CP_2[j] - 1]);
    //   cout<<tempKey[CP_2[j] - 1];
    }
    // cout<<endl;
    
  }
}

void encrypt(string s) {
  string original = strToBit(s);
cout<<original<<endl;
  string transposed_data;
  transposed_data.resize(64);

  //transpose data
  for (int i = 0; i < 64; i++)
    transposed_data[i] = original[PI[i] - 1];

  //L, R for the first iteration
  for (int i = 0; i < 32; i++)
    oldL[i] = transposed_data[i], oldR[i] = transposed_data[i + 32];

  for (int i = 1; i <= 16; i++) {
    newL = oldR;
    cout<<"\n--------------------\nIteration: "<<i<<endl;
    //---------------------------------------------------------------
    //keys at each round
    //transpose key for the current round
    for (int j = 0; j < 48; j++)
      current_round_key[j] = keys[i][j];
    //---------------------------------------------------------------

    //---------------------------------------------------------------
    //using expansion array we expand from 32-bits to 48bits
    for (int j = 0; j < 48; j++){
      e[j] = oldR[E[j] - 1];
    }
    cout<<"Expanded :"<<e<<endl;

    //xor of key and e
    cout<<"Key :"<<current_round_key<<endl;

    for (int j = 0; j < 48; j++) {
      if ((e[j] - 48) ^ (current_round_key[j] - 48))
        e[j] = '1';
      else
        e[j] = '0';
    }
    cout<<"XORed :"<<e<<endl;

    //transpose
    for (int j = 0; j < 32; j++){
      tempR[j] = e[PI_2[j] - 1];
    }
    cout<<"PI_2 :"<<tempR<<endl;
    cout<<"P_BOX :";
	//p-box
    for (int j = 0; j < 32; j++) {
		cout<<(tempR[P[j] - 1] - 48);
      int k = (tempR[P[j] - 1] - 48) ^(oldL[j] - 48);
      if (k)
        newR[j] = '1';
      else
        newR[j] = '0';
    }
    cout<<"\nXOR oldR and new :"<<newR<<endl;

    //---------------------------------------------------------------

    oldL = newL, oldR = newR;
    cout<<"\nLi: "<<oldL<<"\nRi: "<<oldR<<endl;

  }

  string res;
  for (int j = 0; j < 32; j++)
    res.pb(oldR[j]);

  for (int j = 0; j < 32; j++)
    res.pb(oldL[j]);

  //transpose
  string ret;
  for (int j = 0; j < N; j++)
    ret.pb(res[PI_1[j] - 1]);

	cout<<ret<<endl;
  string str = bitToStr(ret);

  ciphered_text += str;
}

int idx = 0;

void decrypt(string s) {
  string original = strToBit(s);
  string transposed_data;

  transposed_data.resize(64);

  //transpose data
  for (int i = 0; i < 64; i++)
    transposed_data[i] = original[PI[i] - 1];

  //L, R for the first iteration
  for (int i = 0; i < 32; i++)
    oldL[i] = transposed_data[i], oldR[i] = transposed_data[i + 32];

  for (int i = 16; i > 0; i--) {
    newL = oldR;

    //---------------------------------------------------------------
    //keys at each round
    //transpose key for the current round
    for (int j = 0; j < 48; j++)
      current_round_key[j] = keys[i][j];
    //---------------------------------------------------------------

    //---------------------------------------------------------------
    //using expansion array we expand from 32-bits to 48bits
    for (int j = 0; j < 48; j++)
      e[j] = oldR[E[j] - 1];

    //xor of key and e
    for (int j = 0; j < 48; j++) {
      if ((e[j] - 48) ^ (current_round_key[j] - 48))
        e[j] = '1';
      else
        e[j] = '0';
    }

    //transpose
    for (int j = 0; j < 32; j++)
      tempR[j] = e[PI_2[j] - 1];

    //p-box
    for (int j = 0; j < 32; j++) {
      int k = (tempR[P[j] - 1] - 48) ^(oldL[j] - 48);
      if (k)
        newR[j] = '1';
      else
        newR[j] = '0';
    }

    //---------------------------------------------------------------

    oldL = newL, oldR = newR;
  }

  string res;
  for (int j = 0; j < 32; j++)
    res.pb(oldR[j]);

  for (int j = 0; j < 32; j++)
    res.pb(oldL[j]);

  //transpose
  string ret;
  for (int j = 0; j < N; j++)
    ret.pb(res[PI_1[j] - 1]);

  string str = bitToStr(ret);
  deciphered_text += str;
}

int main() {
  freopen("input.txt", "r", stdin);
  //freopen("out.txt", "w", stdout);

  cin >> key;
  getline(cin, plain_text);
  getline(cin, plain_text);

  init();
  make56BitKey();
  generateKeys();

  //---------------------------------------------
  //encryption
  plain_text = make64Bits(plain_text);
  group64BitText(plain_text);
  
  ciphered_text = "";
  for (string s : text_chunks)
    encrypt(s);

  cout << "ciphered : " << ciphered_text << endl;
  //---------------------------------------------

  //---------------------------------------------
  //decryption
  ciphered_text = make64Bits(ciphered_text);
  group64BitText(ciphered_text);

  deciphered_text = "";
  for (string s : text_chunks)
    decrypt(s);

  cout << "deciphered : " << deciphered_text;
  //---------------------------------------------

  return 0;
}


// http://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm