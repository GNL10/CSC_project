#!/bin/bash

# number of clients
CLI_NUM=4

admin_pwd="admin"

#folders
main_folder="output"
admin="admin"
server="server"
client="client"
build="build"

cd $build
make
cd ..

#delete previous main folder
rm -r $main_folder

#create and enter main folder
mkdir $main_folder
echo "Created : $main_folder"

#create folders
mkdir $main_folder/$admin
mkdir $main_folder/$server
mkdir $main_folder/$client

echo "Created : $main_folder/$admin"
echo "Created : $main_folder/$server"
echo "Created : $main_folder/$client"

for ((i=0 ; i <= CLI_NUM; i++))
do
    #create specific client folders
    mkdir "$main_folder/$client/$client$i"
    echo "Created : $main_folder/$client/$client$i"
    cp build/bin/client $main_folder/$client/$client$i
done

cp build/bin/admin $main_folder/$admin
cp Admin/admin.sh $main_folder/$admin
cp build/bin/server $main_folder/$server