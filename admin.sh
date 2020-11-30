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

#creating and entering main folder
mkdir $main_folder
cd $main_folder

#creating folders
mkdir $admin
mkdir $server
mkdir $client

#creating specific client folders
cd $client
for ((i=0 ; i <= CLI_NUM; i++))
do
    mkdir "$client$i"
done
cd ..

#creation of a certificate authority
cd $admin
openssl genrsa -des3 -passout pass:$admin_pwd -out $cert_auth_key 2048
openssl req -new -x509 -days 3650 -key $cert_auth_key -passin pass:$admin_pwd -out $cert_auth_crt -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-21/OU=CA-21/CN=CA21"
cd ..

#creating of a certificate for the server
cd $server
openssl genrsa -out $server_pem 1024
openssl req -new -key $server_pem -out $server_csr -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-21/OU=Server/CN=Server"
    
#signing the server certificate
cp $server_csr ../$admin
cd ../$admin 
openssl x509 -req -in $server_csr -out $server_crt -sha1 -CA $cert_auth_crt -CAkey $cert_auth_key -passin pass:$admin_pwd -CAcreateserial -days 3650
cp $server_crt ../$server
cp $cert_auth_crt ../$server
cd ..

#
