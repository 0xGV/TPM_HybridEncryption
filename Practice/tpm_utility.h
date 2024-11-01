#pragma once


#include <sstream>
#include <string>
#include <vector>
class Tpm_Utility{

	public:

	std::string AES_KEY;
	std::string AES_IV;
	std::string TPM_HANDLE_PATH;
	std::string TPM_PUBKEY_PATH;

	bool aes_is_init = false;
	bool tpm_is_init = false;
	
	int init();
	
	//Function responsible for creating the AES key and init vector.
	int genAES();

	// Function responsible for creating the TPM RSA context and handle.
	int genTPM();
	
	// Function to encrypt the AES key using the TPM public key.
	int encrypt_aes_key();

	// Function to decrypt the AES key using the TPM RSA handle.
	int decrypt_aes_key();
	
	// Function to encrypt certificate files using the AES symmetric key
	int encrypt_certificate(std::string cert_path);
	
	// Function to decrypt certificate files using the AES symmetric key
	int decrypt_certificate(std::string enc_cert_path, std::string dec_cert_path);

	
	Tpm_Utility();
	~Tpm_Utility();

	std::string get_aes_key();
	std::string get_aes_iv();

	void set_aes_key(std::string);
	void set_aes_iv(std::string);

	int load_aes_info();
	bool file_exists(std::string);
	void execute_commands(std::vector<std::ostringstream>);
	private:




};
