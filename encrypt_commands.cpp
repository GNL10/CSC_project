/*
 @brief: public key encryption
 @para: clear_text -[i] The clear text that needs to be encrypted
                   pri_key -[i] private key
 @return: Encrypted data
**/

/* Deals with data length greater than the maximum length of RSA 
single processing data block */

#include <string.h>
#include <fstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

std::string RsaPubEncrypt(const std::string &clear_text, const std::string &pub_key)
{
	std::string encrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)pub_key.c_str(), -1);
	RSA* rsa = RSA_new();
	// we take the public key in the second format
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
 
	// Get the maximum length of the data block that RSA can process at a time
	int key_len = RSA_size(rsa);
	int block_len = key_len-11; // Because the filling method is RSA_PKCS1_PADDING, so you need to subtract 11 from the key_len
 
	// Apply for memory: store encrypted ciphertext data
	char *sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int ret = 0;
	int pos = 0;
	std::string sub_str;
	// Encrypt the data in segments (the return value is the length of the encrypted data)
	while (pos < clear_text.length()) {
		sub_str = clear_text.substr(pos, block_len);
		memset(sub_text, 0, key_len + 1);
		ret = RSA_public_encrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			encrypt_text.append(std::string(sub_text, ret));
		}
		pos += block_len;
	}
	
	// release memory  
	BIO_free_all(keybio);
	RSA_free(rsa);
	delete[] sub_text;
 
	return encrypt_text;
}

int main()
{
    // original plaintext  
	std::string src_text = "test begin\n this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! \ntest end";
	//src_text = "rsa test";
 
	std::string encrypt_text;
	std::string pub_key;

	// Generate key pair - we already have created a pair
    std::string pub_key;
	/* std::string pri_key;
	OpensslTool::GenerateRSAKey(pub_key, pri_key);
	printf("public key:\n");
	printf("%s\n", pub_key.c_str());
	printf("private key:\n");
	printf("%s\n", pri_key.c_str()); */

    // Public key encryption
	encrypt_text = OpensslTool::RsaPubEncrypt(src_text, pub_key);
	printf("encrypt: len=%d\n", encrypt_text.length());
 
    return 0;
}
