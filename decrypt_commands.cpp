/*
 @brief: private key decryption
 @para: cipher_text -[i] encrypted ciphertext
                   pub_key -[i] public key
 @return: decrypted data
**/

/* Deals with data length greater than the maximum length of RSA 
single processing data block */

#include <string.h>
#include <fstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

std::string RsaPriDecrypt(const std::string &cipher_text, const std::string &pri_key)
{
	std::string decrypt_text;
	RSA *rsa = RSA_new();
	BIO *keybio;
	keybio = BIO_new_mem_buf((unsigned char *)pri_key.c_str(), -1);
 
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (rsa == nullptr) {
		unsigned long err = ERR_get_error(); // Get the error number
		char err_msg[1024] = { 0 };
		ERR_error_string(err, err_msg); // Format: error:errId: library: function: reason
		printf("err msg: err:%ld, msg:%s\n", err, err_msg);
		return std::string();
	}
 
	// Get the maximum length of RSA single processing
	int key_len = RSA_size(rsa);
	char *sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int ret = 0;
	std::string sub_str;
	int pos = 0;
	// Decrypt the ciphertext in segments
	while (pos < cipher_text.length()) {
		sub_str = cipher_text.substr(pos, key_len);
		memset(sub_text, 0, key_len + 1);
		ret = RSA_private_decrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			decrypt_text.append(std::string(sub_text, ret));
			printf("pos:%d, sub: %s\n", pos, sub_text);
			pos += key_len;
		}
	}
	// release memory  
	delete[] sub_text;
	BIO_free_all(keybio);
	RSA_free(rsa);
 
	return decrypt_text;
}

int main()
{
	// original plaintext  
	std::string src_text = "test begin\n this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! this is an rsa test example!!! \ntest end";
	//src_text = "rsa test";
 
	std::string decrypt_text;
	std::string pri_key;

	// Generate key pair - we already have created a pair
    // std::string pub_key;
	std::string pri_key;
	/* OpensslTool::GenerateRSAKey(pub_key, pri_key);
	printf("public key:\n");
	printf("%s\n", pub_key.c_str());
	printf("private key:\n");
	printf("%s\n", pri_key.c_str()); */
 
    // Private key decryption
	decrypt_text = OpensslTool::RsaPriDecrypt(encrypt_text, pri_key);
	printf("decrypt: len=%d\n", decrypt_text.length());
	printf("decrypt: %s\n", decrypt_text.c_str());
 
    return 0;
}
