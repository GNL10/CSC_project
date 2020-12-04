import sys
sys.path.append(".")
from databasemanager import DataBaseManager
from server import Server

server = Server()

server.searchClients()

server.respondToClient(0, "lsdjdlfkjsd")
