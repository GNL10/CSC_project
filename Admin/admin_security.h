#ifndef ADMIN_SECURITY_H
#define ADMIN_SECURITY_H

#include "admin.h"

class AdminSecurity{

    public:
        AdminSecurity(){
            cout << ":::Admin Security Init:::" << endl;
        }

/*
        string RsaPriDecrypt(const std::string &cipher_text, const std::string &pri_key){
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

        std::string RsaPubEncrypt(const std::string &clear_text, const std::string &pub_key){
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
*/

};

#endif