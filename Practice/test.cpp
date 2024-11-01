#include <cstdio>
#include <iomanip>
#include <openssl/rand.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "tpm_utility.h"
using namespace std;



int main(){
	Tpm_Utility *tpm = new Tpm_Utility();

	tpm->init();
	return 0;
}


/*
int genKey(){

	unsigned char key_buffer[32];
	unsigned char iv_buffer[16];
	ofstream out_file;
	ostringstream output;

	int rc = RAND_load_file("/dev/random", 32);

	if (rc != 32){
		cout << "Error initializing the prng seed" << endl;
	}

	rc = RAND_bytes(key_buffer, sizeof(key_buffer));
	out_file.open(AES_KEY);
	for (int i = 0; i < KEY_LEN; i++){
		output << hex << setw(2) << setfill('0') << (int)key_buffer[i];
	}
	out_file << output.str().c_str();
	out_file.close();

	cout << "AES Key: " << output.str() << endl;
	
	output.clear();
	output.str("");

	rc = RAND_bytes(iv_buffer, sizeof(iv_buffer));
	out_file.open(AES_IV);
	for (int i = 0; i < IV_LEN; i++){
		output << hex << setw(2) << setfill('0') << (int)iv_buffer[i];
	}
	out_file << output.str().c_str();
	out_file.close();

	cout << "AES IV: " << output.str() << endl;
	return 0;
	return 0;
}
*/
