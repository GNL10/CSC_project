import os
import shutil
import ssl
import sys
sys.path.append(".")

from security import Security

class Admin:

    clients = {}
    n_clients = 0
    dir_path = os.getcwd()
    securityAPI = Security()

    def __init__(self, local = 0, n_clients = 1):
        self.n_clients = n_clients
        self.createClient()
        self.configClient()

    def createClient(self):
        for x in range(self.n_clients):
            file_dir_location = os.path.join(self.dir_path, "../Clients")
            if x == 0 and os.path.isdir(file_dir_location):
                shutil.rmtree(file_dir_location)
            try:
                client_dir = os.path.join(file_dir_location, "client" + str(x))
                os.makedirs(client_dir)
                self.clients[x] = {
                    "location": client_dir
                }
            except OSError:
                print ("Falha a criar a pasta de cliente %d", x)
    
    def configClient(self):
        if ssl.OPENSSL_VERSION == 0: return
        for x in range(self.n_clients):
            self.clients[x]["private-rsa-key-path"] = ""
            self.clients[x]["public-rsa-key-path"] = ""

    def cleanAllClients(self):
        directory = os.path.join(self.dir_path, "../Clients")
        if os.path.isdir(directory):
            shutil.rmtree(directory)
    
    def cleanClient(self, client):
        client_path = os.path.join(self.dir_path, "../Clients/client" + str(client))
        if os.path.isdir(client_path):
            shutil.rmtree(client_path)

