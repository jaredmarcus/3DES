#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <cmath>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

// Array to hold the 3 sets of 16 keys
string round_keys[16];
string tworound_keys[16];
string threeround_keys[16];

//Initializing Vector
//my first name and last initial, with graduation yr
string iv = "0100101001100001011100100110010101100100010011010011001000110001"; //JaredM21

// The PC1 table 
int pc1[56] = { 57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

// The PC2 table
int pc2[48] = { 14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};

// The initial permutation table 
int initial_permutation[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};

// The expansion table
int expansion_table[48] = {
	32,1,2,3,4,5,4,5,
	6,7,8,9,8,9,10,11,
	12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,
	22,23,24,25,24,25,26,27,
	28,29,28,29,30,31,32,1
};

// The substitution boxes. The should contain values
// from 0 to 15 in any order.
//8 different blocks
//4 rows of 16 digits
int substition_boxes[8][4][16] = {
	{
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},
	{
	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{
	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	}
};

// The permutation table
int permutation_tab[32] = {
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
};

// The inverse permutation table
int inverse_permutation[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};

string stringToBinary(string key) 
{ 
	string binaryString = "";
	for (char& _char : key) {
			binaryString +=bitset<8>(_char).to_string();
	}
	return binaryString;
} 

// Function to convert a number in decimal to binary
string convertDecimalToBinary(int decimal)
{
	string binary;
	while (decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal = decimal / 2;
	}
	while (binary.length() < 4) {
		binary = "0" + binary;
	}
	return binary;
}

// Function to convert a number in binary to decimal
int convertBinaryToDecimal(string binary)
{
	int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for (int i = size - 1; i >= 0; i--)
	{
		if (binary[i] == '1') {
			decimal += pow(2, counter);
		}
		counter++;
	}
	return decimal;
}

string Xor(string a, string b) {
	string result = "";
	int size = b.size();
	for (int i = 0; i < size; i++) {
		if (a[i] != b[i]) {
			result += "1";
		}
		else {
			result += "0";
		}
	}
	return result;
}

void format(int counter) {
	if (counter == 1) {
		for (int i = 0; i < 36; i++) {
			cout << "<>";
		}
		cout << endl;
	}
	else {
		cout << endl;
	}
}

void BinToAscii(string data, int counter) {
	if(counter == 3)
		goto Jump;
	cout << "Converting each 8-Bits to ASCII...." << endl;
	//just a sleep function to make it more interactive
	this_thread::sleep_for(chrono::milliseconds(1000));
	Jump:
	stringstream sstream(data);
	string text = "";
	while (sstream.good())
	{
		bitset<8> bits;
		sstream >> bits;
		char c = char(bits.to_ulong());
		text += c;
	}
	if (counter == 0)
		cout << "Ciphertext: " << text << endl;
	if (counter == 1)
		cout << "Plain text: " << text << endl;
	if (counter == 2)
		cout << "Key:        " << text << endl;
	if (counter == 3)
		cout << text;
}

// Function to do a circular left shift by 1
string shift_left_once(string key_chunk) {
	string shifted = "";
	for (int i = 1; i < 28; i++) {
		shifted += key_chunk[i];
	}
	shifted += key_chunk[0];
	return shifted;
}

// Function to do a circular left shift by 2
string shift_left_twice(string key_chunk) {
	string shifted = "";
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j < 28; j++) {
			shifted += key_chunk[j];
		}
		shifted += key_chunk[0];
		key_chunk = shifted;
		shifted = "";
	}
	return key_chunk;
}

void generate_keys(string key, int counter) {
	// 1. Compressing the key using the PC1 table
	string perm_key = "";
	for (int i = 0; i < 56; i++) {
		perm_key += key[pc1[i] - 1];
	}
	// 2. Dividing the result into two equal halves
	string left = perm_key.substr(0, 28);
	string right = perm_key.substr(28, 28);
	// Generating 16 keys
	for (int i = 0; i < 16; i++) {
		// 3.1. For rounds 1, 2, 9, 16 the key_chunks
		// are shifted by one.
		if (i == 0 || i == 1 || i == 8 || i == 15) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		// 3.2. For other rounds, the key_chunks
		// are shifted by two
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}
		// 4. The chunks are combined
		string combined_key = left + right;
		string round_key = "";
		// 5. Finally, the PC2 table is used to transpose
		// the key bits to how many are needed (64 to 56)
		for (int i = 0; i < 48; i++) {
			round_key += combined_key[pc2[i] - 1];
		}
		if (counter == 1) {
			round_keys[i] = round_key;
			cout << "Key " << i + 1 << ": " << round_keys[i] << endl;
		}
		else if (counter == 2) {
			tworound_keys[i] = round_key;
			cout << "Key " << i + 1 << ": " << tworound_keys[i] << endl;
		}
		else if (counter == 3) {
			threeround_keys[i] = round_key;
			cout << "Key " << i + 1 << ": " << threeround_keys[i] << endl;
		}
		//just a sleep function to make it more interactive
		this_thread::sleep_for(chrono::milliseconds(1000)); 
	}

}

string ECBDES(string _pt, int counter) {
	//1. Applying the initial permutation
	string perm = "";
	for (int i = 0; i < 64; i++) {
		perm += _pt[initial_permutation[i] - 1];
	}
	// 2. Dividing the result into two equal halves 
	string left = perm.substr(0, 32);
	string right = perm.substr(32, 32);
	// The plain text is encrypted 16 times  
	for (int i = 0; i < 16; i++) {
		string right_expanded = "";
		//Part 3 section #
		// 1. The right half of the plain text is expanded
		for (int i = 0; i < 48; i++) {
			right_expanded += right[expansion_table[i] - 1];
		}
		// 2. The result is xored with a key
		string xored;
		if (counter == 0)
			xored = Xor(round_keys[i], right_expanded);
		if (counter == 1)
			xored = Xor(tworound_keys[i], right_expanded);
		if (counter == 2)
			xored = Xor(threeround_keys[i], right_expanded);
		string res = "";
		// 3. The result is divided into 8 equal parts and passed 
		// through 8 substitution boxes. After the passing through a 
		// s-box, each box is reduced from 6 to 4 bits.
		for (int i = 0; i < 8; i++) {
			// Finding row and column indices to lookup the s-box
			string row1 = xored.substr(i * 6, 1) + xored.substr(i * 6 + 5, 1);
			int row = convertBinaryToDecimal(row1);
			string col1 = xored.substr(i * 6 + 1, 1) + xored.substr(i * 6 + 2, 1) + xored.substr(i * 6 + 3, 1) + xored.substr(i * 6 + 4, 1);
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[i][row][col];
			res += convertDecimalToBinary(val);
		}
		// 4. Another permutation is applied
		string perm2 = "";
		for (int i = 0; i < 32; i++) {
			perm2 += res[permutation_tab[i] - 1];
		}
		// 5. The result is xored with the left half
		xored = Xor(perm2, left);
		// 6. The left and the right parts of the plain text are swapped 
		left = xored;
		if (i < 15) {
			string temp = right;
			right = xored;
			left = temp;
		}
	}
	// 4. The halves of the plain text are applied
	string combined_text = left + right;
	string ciphertext = "";
	// The inverse of the initial permuttaion is applied
	for (int i = 0; i < 64; i++) {
		ciphertext += combined_text[inverse_permutation[i] - 1];
	}
	return ciphertext;
}

// Implementing the algorithm for CBC
string CBC_DES(string text, int counter, bool enc = true) {
	if (enc)	//true
	{
		// xor iv
		text = Xor(text, iv);
	}
	//1. Applying the initial permutation
	string perm = "";
	for (int i = 0; i < 64; i++) {
		perm += text[initial_permutation[i] - 1];
	}
	// 2. Dividing the result into two equal halves 
	string left = perm.substr(0, 32);
	string right = perm.substr(32, 32);
	// The plain text is encrypted 16 times  
	for (int i = 0; i < 16; i++) {
		string right_expanded = "";
		//Part 3 section #
		// 1. The right half of the plain text is expanded
		for (int j = 0; j < 48; j++) {
			right_expanded += right[expansion_table[j] - 1];
		}
		// 3. The result is xored with a key
		string xored;

		int index = i;
		if (!enc)
			index = 15 - i;
		
		if (counter == 0)
			xored = Xor(round_keys[index], right_expanded);
		if (counter == 1)
			xored = Xor(tworound_keys[index], right_expanded);
		if (counter == 2)
			xored = Xor(threeround_keys[index], right_expanded);
		string res = "";
		// 4. The result is divided into 8 equal parts and passed 
		// through 8 substitution boxes. After the passing through a 
		// s-box, each box is reduced from 6 to 4 bits.
		for (int k = 0; k < 8; k++) {
			// Finding row and column indices to lookup the
			// s-box
			string row1 = xored.substr(k * 6, 1) + xored.substr(k * 6 + 5, 1);
			int row = convertBinaryToDecimal(row1);
			string col1 = xored.substr(k * 6 + 1, 1) + xored.substr(k * 6 + 2, 1) + xored.substr(k * 6 + 3, 1) + xored.substr(k * 6 + 4, 1);
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[k][row][col];
			res += convertDecimalToBinary(val);
		}
		// 5. Another permutation is applied
		string perm2 = "";
		for (int m = 0; m < 32; m++) {
			perm2 += res[permutation_tab[m] - 1];
		}
		// 6. The result is xored with the left half
		xored = Xor(perm2, left);
		// 7. The left and the right parts of the plain text are swapped 
		left = xored;
		if (i < 15) {
			string temp = right;
			right = xored;
			left = temp;
		}
	}
	// 4. The halves of the plain text are applied
	string combined_text = left + right;
	string ciphertext = "";
	// The inverse of the initial permuttaion is applied
	for (int i = 0; i < 64; i++) {
		ciphertext += combined_text[inverse_permutation[i] - 1];
	}

	if (enc == false)
	{
		ciphertext = Xor(ciphertext, iv);
	}
	return ciphertext;
}

string ECB_encryption(string pt, string key, int counter, int DE) {
	format(0);

	cout << "Plain text: " << pt << endl;
	BinToAscii(pt, 1);
	format(0);

	cout << "Key:        " << key << endl;
	BinToAscii(key, 2);
	format(0);

	// Applying the algorithm
	string ct = ECBDES(pt, DE); //default mode is CBC (Cipher block chaining)
	cout << "Ciphertext: " << ct << endl;
	BinToAscii(ct, 0);
	format(0); format(1); format(0);
	return ct;
}

string ECB_decryption(string ct, int counter) {

	format(0);
	cout << "Plain text: " << ct << endl;
	BinToAscii(ct, 1);
	// Reversing the round_keys array for decryption
	if (counter == 0) {
		int i = 15;
		int j = 0;
		while (i > j)
		{
			string temp = round_keys[i];
			round_keys[i] = round_keys[j];
			round_keys[j] = temp;
			i--;
			j++;
		}
	}
	if (counter == 1) {
		int i = 15;
		int j = 0;
		while (i > j)
		{
			string temp = tworound_keys[i];
			tworound_keys[i] = tworound_keys[j];
			tworound_keys[j] = temp;
			i--;
			j++;
		}
	}
	if (counter == 2) {
		int i = 15;
		int j = 0;
		while (i > j)
		{
			string temp = threeround_keys[i];
			threeround_keys[i] = threeround_keys[j];
			threeround_keys[j] = temp;
			i--;
			j++;
		}
	}
	string ciphertext = ECBDES(ct, counter);
	format(0);
	cout << "Ciphertext: " << ciphertext << endl;
	BinToAscii(ciphertext, 0);
	format(0); format(1); format(0);
	return ciphertext;
}

string CBC_encryption(string pt, string key, int counter, int DE, bool selection) {
	format(0);
	string ct = "";
	
	cout << "Plain text: " << pt << endl;
	BinToAscii(pt, 1);
	format(0);

	cout << "Key:        " << key << endl;
	BinToAscii(key, 2);
	format(0);

	// Applying the algorithm
	ct = CBC_DES(pt, DE, selection);
	cout << "Ciphertext: " << ct << endl;
	BinToAscii(ct,0);
	format(0); format(1); format(0);
	return ct;
}

string CBC_decryption(string ct, int counter) {

	format(0);
	cout << "Plain text: " << ct << endl;
	BinToAscii(ct, 1);
	string ciphertext = CBC_DES(ct, counter, false);
	format(0);
	cout << "Ciphertext: " << ciphertext << endl;
	BinToAscii(ciphertext, 0);
	format(0); format(1); format(0);
	return ciphertext;
}

bool checker(string key){
	bool check;
	if (key.length()== 8)
		for (int i = 0; i < key.size(); i++)
			{
				int uppercaseChar = toupper(key[i]); //Convert character to upper case version of character
				if (uppercaseChar < 'A' || uppercaseChar > 'Z') //If character is not A-Z
				{
					return false;
				}
			}
		check = true;
	if (key.length()!= 8)
		check = false;
	return check;
}

void exitMsg(string ct, string copy){	
	format(0);
	if (ct == copy){
		cout<<"Plain text encrypted and decrypted successfully."<<endl;
		cout<<"CONGRATS!"<<endl;
	}
	else {
		cout<<"Plain text DID NOT encrypt and decrypt successfully."<<endl;
	}
}

int main() {
	string x;
	ifstream inFile;
	inFile.open("AsciiArt.txt");
	if (!inFile) {
		cout << "Unable to open file" << endl;
		exit(1); // terminate with error
	}

	while (getline(inFile, x)) {
		cout << x << endl;
	}
	inFile.close();
	format(0);
	int selection = 0;
	cout << "Mode selection: \n1 - CBC \n2 - ECB" << endl;
	cout << "Mode: "; cin >> selection;
	if(selection == 1){
		cout << "CBC mode selected" << endl;
	}
	if(selection == 2){
		cout << "ECB mode selected" << endl;
	}
	cout << "3DES has 3 keys" << endl;
	string key = "";
	string key1, key2 = key;
	bool check;
	cout << "Choose 3 keys that are 8 characters long" << endl;
	Redo:
	cout << "1st key: "; cin >> key;
		check = checker(key);
		if (check == false){
			cout << "Key is not 8 characters long or contains restricted characters. Characters must be A-Z" << endl;
			goto Redo;
		}
	Redo1:
	cout << "2nd key: "; cin >> key1;
		check = checker(key1);
		if (check == false){
			cout << "Key is not 8 characters long or contains restricted characters. Characters must be A-Z" << endl;
			goto Redo1;
		}
	Redo2:
	cout << "3rd key: "; cin >> key2;
		check = checker(key1);
		if (check == false){
			cout << "Key is not 8 characters long or contains restricted characters. Characters must be A-Z" << endl;
			goto Redo2;
		}
	cout << "Your chosen keys are: " << key << " " << key1 << " " << key2 << endl;
	format(0);
	//64 bit key is converted to a 56 bit key by discarding every 8th bit of the 
	//original key to use as parity bits in the keygen function. 
	//To then pull a 48 bit subkey from the 56 bit key.
	
	cout << "Generating 16 keys (" << key << ") for 16 rounds...." << endl;
	//cout << key << endl;
	//just a sleep function to make it more interactive
	this_thread::sleep_for(chrono::milliseconds(2000));
	//64 bit binary key that is the word Academic
	key = stringToBinary(key);
	// Calling the function to generate 16 keys
	generate_keys(key, 1);
	format(0); format(1); format(0);
	
	cout << "Generating 16 keys (" << key1 << ") for 16 rounds...." << endl;
	//just a sleep function to make it more interactive
	this_thread::sleep_for(chrono::milliseconds(2000));
	//64 bit binary key that is the word Aircraft
	key1 = stringToBinary(key1);
	// Calling the function to generate 16 keys
	generate_keys(key1, 2);
	format(0); format(1); format(0);
	
	cout << "Generating 16 keys (" << key2 << ") for 16 rounds...." << endl;
	//just a sleep function to make it more interactive
	this_thread::sleep_for(chrono::milliseconds(2000));
	//64 bit binary key that is the word Advanced
	key2 = stringToBinary(key2);
	// Calling the function to generate 16 keys
	generate_keys(key2, 3);
	format(0); format(1); format(0);
	
	// 64 bit plain text
	string pt = "";
	cout << "Enter a 8 character word to Encrypt" << endl;
	Error:
		cout << "Plaintext: "; cin >> pt;
			check = checker(pt);
			if (check == false){
				cout << "Plaintext is not 8 characters long or contains restricted characters. Characters must be A-Z" << endl;
				goto Error;
			}
	pt = stringToBinary(pt);	
	//created a copy of the plaintext to test towards the end of the program.
	string copy = pt;
	string ct = "";
	format(0);
	
	bool sel; //selection 
	//true  = CBC 
	//false = ECB
	if(selection == 1){
		sel = true;
		cout << "3DES 1 Encryption" << endl;
		cout << "Using key 1 ("; BinToAscii(key, 3); cout << ") to Encrypt" << endl;
		ct = CBC_encryption(pt, key, 2, 0, sel);

		cout << "3DES 2 Decryption" << endl;
		cout << "Using key 2 ("; BinToAscii(key1, 3); cout << ") to Decrypt" << endl;
		ct = CBC_decryption(ct, 1);

		cout << "3DES 3 Encryption" << endl;
		cout << "Using key 3 ("; BinToAscii(key2, 3); cout << ") to Encrypt" << endl;
		ct = CBC_encryption(ct, key2, 2, 2, sel);
		cout << "Fully Encrypted text is: "; BinToAscii(ct, 3); format(0);

		cout << "3DES 3 Decryption" << endl;
		cout << "Using key 3 ("; BinToAscii(key2, 3); cout << ") to Decrypt" << endl;
		ct = CBC_decryption(ct, 2);

		cout << "3DES  2 Encryption" << endl;
		cout << "Using key 2 ("; BinToAscii(key1, 3); cout << ") to Encrypt" << endl;
		ct = CBC_encryption(ct, key1, 0, 1, sel);

		cout << "3DES 1 Decryption" << endl;
		cout << "Using key 1 ("; BinToAscii(key, 3); cout << ") to Decrypt" << endl;
		ct = CBC_decryption(ct, 0);
		exitMsg(ct, copy);
	}
	if(selection == 2){
		cout << "3DES 1 Encryption" << endl;
		cout << "Using key 1 ("; BinToAscii(key, 3); cout << ") to Encrypt" << endl;
		ct = ECB_encryption(pt, key, 2, 0);

		cout << "3DES 2 Decryption" << endl;
		cout << "Using key 2 ("; BinToAscii(key1, 3); cout << ") to Decrypt" << endl;
		ct = ECB_decryption(ct, 1);

		cout << "3DES 3 Encryption" << endl;
		cout << "Using key 3 ("; BinToAscii(key2, 3); cout << ") to Encrypt" << endl;
		ct = ECB_encryption(ct, key2, 2, 2);
		cout << "Fully Encrypted text is: " << ct << endl;

		cout << "3DES 3 Decryption" << endl;
		cout << "Using key 3 ("; BinToAscii(key2, 3); cout << ") to Decrypt" << endl;
		ct = ECB_decryption(ct, 2);

		cout << "3DES  2 Encryption" << endl;
		cout << "Using key 2 ("; BinToAscii(key1, 3); cout << ") to Encrypt" << endl;
			int i = 15;
			int j = 0;
			while (i > j)
			{
				string temp = tworound_keys[i];
				tworound_keys[i] = tworound_keys[j];
				tworound_keys[j] = temp;
				i--;
				j++;
			}
		ct = ECB_encryption(ct, key1, 0, 1);

		cout << "3DES 1 Decryption" << endl;
		cout << "Using key 1 ("; BinToAscii(key, 3); cout << ") to Decrypt" << endl;
		ct = ECB_decryption(ct, 0);
		exitMsg(ct, copy);
	}
	return 0;
}
