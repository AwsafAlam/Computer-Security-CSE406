

// Initial permut matrix for the datas
// data[] is 64 bit. PI[] is also 64bits. For example, 58th bit in data (data[57]) will be the first bit of transposed_data (transposed_data[0]), In the same way, transposed_data[1] = data[49]

int PI[] = {58, 50, 42, 34, 26, 18, 10, 2,
      60, 52, 44, 36, 28, 20, 12, 4,
      62, 54, 46, 38, 30, 22, 14, 6,
      64, 56, 48, 40, 32, 24, 16, 8,
      57, 49, 41, 33, 25, 17, 9, 1,
      59, 51, 43, 35, 27, 19, 11, 3,
      61, 53, 45, 37, 29, 21, 13, 5,
      63, 55, 47, 39, 31, 23, 15, 7};

	  
	  
// Initial permut made on the key
// Remember, your key is 64 bit long. But you need only 56 bits. How will you shorten the key.
// Use CP_1 array. It means, 57th bit (key[56] will be the first bit of modified_key: modified_key[0] = key[56], modified_key[1] = key[48]) 
int CP_1[] = {57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

		
		
// Permut applied on shifted key to get Ki+1
// For each round, you will need, 48 bit key. But currently you have 56bit keys. Use CP_2 (48 len) arrays.
// Key_in_round_i[0] = modified_key[13], ....
int CP_2[] = {14, 17, 11, 24, 1, 5, 3, 28,
        15, 6, 21, 10, 23, 19, 12, 4,
        26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32};

		

// Expand matrix to get a 48bits matrix of datas to apply the xor with Ki
// You need to expand your leftmost 32 bits to 48 bits. Use E array.
// expanded_bits [0] = leftmost_bit[31], expanded_bits[1] = leftmost_bit[0]
int E[] = {32, 1, 2, 3, 4, 5,
     4, 5, 6, 7, 8, 9,
     8, 9, 10, 11, 12, 13,
     12, 13, 14, 15, 16, 17,
     16, 17, 18, 19, 20, 21,
     20, 21, 22, 23, 24, 25,
     24, 25, 26, 27, 28, 29,
     28, 29, 30, 31, 32, 1};





// PI_2 is used to shorten the result of xoring leftmost extended 48 bits and 48 bits of key for that round
// shorten_result [0] = result_xor [34], ......
int PI_2[] = {35, 38, 46, 6, 43, 40, 14, 45,
		33, 19, 26, 15, 23, 8, 22, 10, 
		12, 11, 5, 25, 27, 21, 16, 31,
		28, 32, 34, 24, 9, 37, 2, 1};

// Permut made after each SBox substitution for each round
// The P box is 8*4 size. So there will be 32 inputs and 32 outputs. The internal wiring is given.
// It means input 1 is connected to 16
// input 2 is connected to 7........
int P[] = {16, 7, 20, 21, 29, 12, 28, 17,
          1, 15, 23, 26, 5, 18, 31, 10,
          2, 8, 24, 14, 32, 27, 3, 9,
          19, 13, 30, 6, 22, 11, 4, 25};

// Final permut for datas after the 16 rounds
// After all rounds end, use this matrix to permute the data again. 

int PI_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25};

// Matrix that determine the shift for each round of keys
// AT first round of encryption, left shift by 1 bit, 2nd round: 1 bit, 3rd round: 2 bit......
int SHIFT[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};