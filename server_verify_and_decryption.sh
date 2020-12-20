# RECEIVE THE FILE FROM THE CLIENT

# Verify the signature of file
openssl dgst -sha1 -verify public_key.pem -signature sha1.sign myfile.txt

# IF Verified OK:

openssl rsautl -decrypt -inkey server_secretkey.pem -in key.bin.enc -out key.bin

openssl enc -d -aes-256-cbc -in SECRET_FILE.enc -out SECRET_FILE -pass file:./key.bin 