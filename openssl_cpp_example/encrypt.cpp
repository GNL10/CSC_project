#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>

#define KEY_LENGTH  2048
#define PUB_EXP     3
#define PRINT_KEYS 1
#define WRITE_TO_FILE 1


int main() {

    size_t  pri_len;
    size_t  pub_len;
    char   *pri_key;
    char   *pub_key;
    char    msg[KEY_LENGTH/8];
    char   *encrypt = NULL;
    char   *decrypt = NULL;
    char   *err;

    /*
    // generate key pair
    printf("Generate RSA (%d bits) keypair... ", KEY_LENGTH );
    fflush(stdout);
    RSA *keypair = RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);

    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());
    
    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSA_PUBKEY(pub, keypair);
    */

    /*
    BIO *pub = NULL;
    pub = BIO_new(BIO_s_file());
    BIO_read_filename(pub, "public.pem");
    

    BIO *pri = NULL;
    pri = BIO_new(BIO_s_file());
    BIO_read_filename(pri, "private.pem");
    */

    FILE *fp = fopen("public.pem", "r");
    RSA *keypair = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    fclose(fp);

    /*
    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    pri_key = (char *)malloc(pri_len+1);
    pub_key = (char *)malloc(pub_len+1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';
    */

#ifdef PRINT_KEYS
    //printf("\n%s\n%s\n", pri_key, pub_key);
#endif
    printf("RSA SIZE : %d\n ", RSA_size(keypair));
    printf("done.\n");

    // Get the message to encrypt
    printf("Message to encrypt: ");
    fgets(msg, KEY_LENGTH-1, stdin);
    msg[strlen(msg)-1] = '\0';

    // Encrypt the message
    encrypt = (char *)malloc(RSA_size(keypair));
    int encrypt_len;
    err = (char *)malloc(130);
    
    if((encrypt_len = RSA_public_encrypt(strlen(msg)+1, (unsigned char*)msg, (unsigned char*)encrypt,
                                         keypair, RSA_PKCS1_OAEP_PADDING)) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
        //goto free_stuff;
    }

#ifdef WRITE_TO_FILE
    // Write the encrypted message to a file
    FILE *out = fopen("out.bin", "w");
    fwrite(encrypt, sizeof(*encrypt),  RSA_size(keypair), out);
    fclose(out);
    printf("Encrypted message written to file.\n");
    free(encrypt);
    encrypt = NULL;
    /*
    // Read it back
    printf("Reading back encrypted message and attempting decryption...\n");
    encrypt = (char *)malloc(RSA_size(keypair));
    out = fopen("out.bin", "r");
    fread(encrypt, sizeof(*encrypt), RSA_size(keypair), out);
    fclose(out);*/
#endif
    /*
    // Decrypt it
    decrypt = (char *)malloc(encrypt_len);
    if(RSA_private_decrypt(encrypt_len, (unsigned char*)encrypt, (unsigned char*)decrypt,
                           keypair, RSA_PKCS1_OAEP_PADDING) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error decrypting message: %s\n", err);
        //goto free_stuff;
    }
    printf("Decrypted message: %s\n", decrypt);
    */
    //free_stuff:
    RSA_free(keypair);
    //BIO_free_all(pub);
    //BIO_free_all(pri);
    //free(pri_key);
    //free(pub_key);
    free(encrypt);
    //free(decrypt);
    free(err);




    return 0;
}


