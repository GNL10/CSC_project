import os
import json
import sys
sys.path.append(".")

from fileparser import FileParser
from security import Security
from databasemanager import DataBaseManager

class Server:

    dir_path = os.getcwd()
    clients = {}
    n_clients = 0

    def __init__(self, local = 0):
        print("server working...")

    def searchClients(self):
        clients_path = os.path.join(self.dir_path, "../Clients")
        if os.path.isdir(clients_path):
            x = 0
            while True:
                filename = "/client" + str(x)
                client_path = clients_path + filename
                if not os.path.isdir(client_path): break
                if x in self.clients.keys():
                    x = x + 1
                    continue
                else:
                    self.clients[x] = {
                        "location": client_path
                    }
                    self.n_clients = x
                    x = x + 1
        
    def proccessCommand(self, client):
        client_path = ""
        if client not in self.clients.keys():
            return 0
        
        client_path = self.clients[client]["location"]
        command = {}

        with open('./config.json') as f:
            config_file = json.load(f)
            command["db_cmd"], command["db_key"] = \
                FileParser.parse(path = client_path + "/" + config_file["client-command-file"])
            print(command)
        
        # pipeline security proccess using RSA encryption
        db_method = Security.pipelineSecurity(secured_command = command["db_cmd"])

        try:
            method = getattr(DataBaseManager, db_method)
        except AttributeError:
            raise NotImplementedError("erro")
        method(command)


        return 1
        

