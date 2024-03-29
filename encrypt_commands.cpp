#include <string.h>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

// deals with data length greater than the maximum length of RSA single processing data block
std::string RsaPubEncrypt(const std::string &clear_text, const std::string &pub_key)
{
	std::string encrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)pub_key.c_str(), -1);
	RSA* rsa = RSA_new();

	// use 'PEM_read_bio_RSA_PUBKEY' to be in agreement with the generated key format
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	
	// get the maximum length of the data block that RSA can process at a time
	int key_len = RSA_size(rsa);
	int block_len = key_len-11; // since the filling method is RSA_PKCS1_PADDING, we need to subtract 11 from the key_len
 
	// store encrypted ciphertext data
	char *sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int ret = 0;
	int pos = 0;
	std::string sub_str;

	// encrypt the data in segments (the return value is the length of the encrypted data)
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
    // copy file content into a string
	std::ifstream file(filename.c_str());
	std::stringstream buffer;
	buffer << file.rdbuf();
    std::string src_text = buffer.str();

	std::string encrypt_text;
	std::string pub_key;

    // public key encryption
	encrypt_text = RsaPubEncrypt(src_text, pub_key);

	// saves the encrypted content back to the file
	std::ofstream file("comds_example.txt");
	file << encrypt_text;
    return 0;
}
