import sys
sys.path.append(".")
from databasemanager import DataBaseManager
from server import Server

server = Server()

server.searchClients()

print(server.proccessCommand(0))
