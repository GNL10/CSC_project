#!/bin/bash

# number of clients
CLI_NUM=4

admin_pwd="admin"

#folders
main_folder="output"
admin="admin"
server="server"
client="client"

#certificates and keys
cert_auth_key="my-ca.key"
cert_auth_crt="my-ca.crt"

server_pem="csc-server.pem"
server_csr="csc-server.csr"
server_crt="csc-server.crt"

#create and entering main folder
mkdir $main_folder
cd $main_folder

#create folders
mkdir $admin
mkdir $server
mkdir $client

#create a Certificate Authority (CA)
cd $admin
#generates a key pair for the 2048-bit RSA chiper algorithm and encrypts it with the symetric cipher 
#algorithm 3DES and a password. This will be the public and private key of the CA.
openssl genrsa -des3 -passout pass:$admin_pwd -out $cert_auth_key 2048
#sign the public key with the CA private key. 
#as this is a root certificate (i.e. self-signed) the private signature key is the public key pair to be signed. 
openssl req -new -x509 -days 3650 -key $cert_auth_key -passin pass:$admin_pwd -out $cert_auth_crt -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-21/OU=CA-21/CN=CA21"
cd ..

cd $server
#generates a key pair for the server 
openssl genrsa -out $server_pem 1024
#generates a certificate request
openssl req -new -key $server_pem -out $server_csr -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-21/OU=Server/CN=Server"
#copy certificate request to admin
cp $server_csr ../$admin
cd ../$admin 
#install the root certificate in the server
chmod 444 $cert_auth_crt
cp $cert_auth_crt ../$server
#sign server certificate with the CA certificate
openssl x509 -req -in $server_csr -out $server_crt -sha1 -CA $cert_auth_crt -CAkey $cert_auth_key -passin pass:$admin_pwd -CAcreateserial -days 3650
cp $server_crt ../$server

cd ../$client
for ((i=0 ; i <= CLI_NUM; i++))
do
    #create specific client folders
    mkdir "$client$i"

    client_pem="csc-client${i}.pem"
    client_csr="csc-client${i}.csr"
    client_crt="csc-client${i}.crt"

    cd $client$i
    #generates a key pair for the client
    openssl genrsa -out $client_pem 1024
    #generates a certificate request
    openssl req -new -key $client_pem -out $client_csr -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-21/OU=Server/CN=Server"
    #sign client certificate with the CA certificate
    cp $client_csr ../../$admin
        
    cd ../../$admin
    openssl x509 -req -in $client_csr -out $client_crt -sha1 -CA $cert_auth_crt -CAkey $cert_auth_key -passin pass:$admin_pwd -CAcreateserial -days 3650
    cp $client_crt ../$client/$client$i
    #install the root certificate in the client
    cp $cert_auth_crt ../$client/$client$i
    #install the server certificate in the client
    cd ../$server
    cp $server_crt ../$client/$client$i

    #returns to the initial directory of the loop
    cd ../$client
done

cd ..
