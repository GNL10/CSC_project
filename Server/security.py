import ssl
import os

class Security:

    def __init__(self):
        print("security working...")

    def generate(self):
        print("is secured!!")

    @staticmethod
    def pipelineRSASecurity(secured_command):
        print("secure cmd pipelined...")

        # first: pipeline to RSA security proccess
        # bla bla blade
        decrypted_command = secured_command
        return decrypted_command