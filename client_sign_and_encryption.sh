# Sign the file using sha1 digest and PKCS1 padding scheme
openssl dgst -sha1 -sign private_key.pem -out sha1.sign SECRET_FILE

# Dump the signature file
hexdump sha1.sign

# Convert server certificate to PEM and extract their public key
openssl x509 -inform der -in csc-server.crt -pubkey -noout > server_publickey.pem

# Generate a 256 bit (32 byte) random key (use a new key every time when encrypting a file.)
openssl rand -base64 32 > key.bin

# Encrypt the key
openssl rsautl -encrypt -inkey server_publickey.pem -pubin -in key.bin -out key.bin.enc

# Actually Encrypt large file
openssl enc -aes-256-cbc -salt -in SECRET_FILE -out SECRET_FILE.enc -pass file:./key.bin 

# SEND THE FILE TO THE SERVER
