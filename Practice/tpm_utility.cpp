#include "tpm_utility.h"
#include <cstdio>
#include <iomanip>
#include <openssl/rand.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "sys/stat.h"

using namespace std;

const string AES_KEY_PATH = "aes.key";
const string AES_IV_PATH  = "aes_iv";
const string ENC_KEY_PATH = "enc_aes.key";

const int KEY_LEN = 32;
const int IV_LEN  = 16;

Tpm_Utility::Tpm_Utility(){
	if(file_exists(AES_KEY_PATH) && file_exists(AES_IV_PATH)){
		this->aes_is_init = true;
	}

	if(file_exists(TPM_HANDLE_PATH) && file_exists(TPM_PUBKEY_PATH)){
		this->tpm_is_init = true;
	}
	 		
}

int Tpm_Utility::init(){

	if(!this->aes_is_init){
		this->genAES();
	} else {
		this->load_aes_info();
	}

	if(!this->tpm_is_init){
		this->genTPM();
	}

	return 0;
}

int Tpm_Utility::genAES(){
	
	cout << "Generating new AES key and IV" << endl; 
	unsigned char key_buffer[32];
	unsigned char iv_buffer[16];
	ofstream out_file;
	ostringstream output;

	int rc = RAND_load_file("/dev/random", 32);

	if (rc != 32){
		cout << "Error initializing the prng seed" << endl;
	}

	rc = RAND_bytes(key_buffer, sizeof(key_buffer));
	out_file.open(AES_KEY_PATH);
	for (int i = 0; i < KEY_LEN; i++){
		output << hex << setw(2) << setfill('0') << (int)key_buffer[i];
	}
	out_file << output.str().c_str();
	out_file.close();

	cout << "AES Key: " << output.str() << endl;
	
	output.clear();
	output.str("");

	rc = RAND_bytes(iv_buffer, sizeof(iv_buffer));
	out_file.open(AES_IV_PATH);
	for (int i = 0; i < IV_LEN; i++){
		output << hex << setw(2) << setfill('0') << (int)iv_buffer[i];
	}
	out_file << output.str().c_str();
	out_file.close();

	cout << "AES IV: " << output.str() << endl;
	return 0;
}

int Tpm_Utility::load_aes_info(){
	
	cout << "Loading AES info from files" << endl;
	
	ifstream in_file;
	std::string key_input;
	std::string iv_input;

	in_file.open(AES_KEY_PATH);
	getline(in_file,key_input);
	if(sizeof(key_input) != KEY_LEN){
		cout << "Invalid Key length read from file" << endl;
		return -1;
	}
	this->set_aes_key(key_input);	
	cout << "AES Key: " << this->get_aes_key() << " Size: " << sizeof(key_input) << endl;
	in_file.close();

	in_file.open(AES_IV_PATH);
	getline(in_file, iv_input);
	if(sizeof(iv_input) != KEY_LEN){ //for some reason its treating this as len 32 instead of 16....
		cout << "Invalid IV length read from file: " << sizeof(iv_input) << " " <<iv_input  << endl;
		return -1;
	}
	this->set_aes_iv(iv_input);
	cout << "AES IV: " << this->get_aes_iv() << " Size: " << sizeof(iv_input) << endl;
	in_file.close();


	return 0;
}

int Tpm_Utility::genTPM(){
	std::vector<std::ostringstream> command_list;
	std::ostringstream cmd;

	cmd << "openssl genrsa -out private.pem 2048";
	command_list.push_back(cmd);
	cmd.str("");

	cmd << "openssl rsa -in private.pem -pubout -out public.pem";
	command_list.push_back(cmd);
	cmd.str("");
	
	cmd << "tpm2_createprimary -c primary.ctx -Grsa2048:aes256cfb";
	command_list.push_back(cmd);
	cmd.str("");

	cmd << "tpm2_import -C primary.ctx -G rsa -i private.pem -u rsa_obj.pub -r rsa_obj.priv";
	command_list.push_back(cmd);
	cmd.str("");

	cmd << "tpm2_load -C primary.ctx -u rsa_obj.pub -r rsa_obj.priv -c rsa.ctx";
	command_list.push_back(cmd);
	cmd.str("");

	cmd << "tpm2_evictcontrol -c rsa.ctx -o rsa_handle";
	command_list.push_back(cmd);
	cmd.str("");
	
	cmd << "rm -rf private.pem primary.ctx parent.ctx rsa_obj.pub rsa_obj.priv rsa.ctx";
	command_list.push_back(cmd);
	cmd.str("");

	return 0;
}

void Tpm_Utility::set_aes_key(std::string key){
	this->AES_KEY = key;
}

void Tpm_Utility::set_aes_iv(std::string iv){
	this->AES_IV = iv;
}

std::string Tpm_Utility::get_aes_key(){
	return this->AES_KEY;
}

std::string Tpm_Utility::get_aes_iv(){
	return this->AES_IV;
}

bool Tpm_Utility::file_exists(std::string file){
	struct stat buf;
	return (stat (file.c_str(),&buf) == 0);
}

void Tpm_Utility::execute_commands(std::vector<std::ostringstream> commands){
	for(int i : commands){
		FILE* fp
	}

}

