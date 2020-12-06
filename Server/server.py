import os
import json
import sys
sys.path.append(".")

from fileparser import FileParser
from security import Security
from process_command import process_command as db

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
            commands_content = FileParser.open(file_path = client_path + "/" + config_file["client-command-file"])
            
        for cmd_encrypted in commands_content:
            print(commands_content[cmd_encrypted])
            # pipeline security proccess using RSA encryption
            # cmd_RSA_decrypted = Security.pipelineRSASecurity(commands_content[cmd_encrypted])
            
            #command["db_cmd"], command["db_key"] = FileParser.parse(cmd_RSA_decrypted)
            #if command["db_cmd"] is None: continue

            try:
                #db_manager_method = getattr(DataBaseManager, command["db_cmd"])
                res = db(commands_content[cmd_encrypted], client)
                
                print(res)
            except AttributeError:
                raise NotImplementedError("lol, claramente que tentaste chamar um metodo nao definido smh")
            #db_manager_method(command)
    
        return 1

    def respondToClient(self, client, content):
        client_path = ""
        if client not in self.clients.keys():
            return 0
        
        client_path = self.clients[client]["location"]

        with open('./config.json') as f:
            config_file = json.load(f)
            commands_content = FileParser.write(client_path + "/" + config_file["client-response-file"], content)
        
        return 1
        
