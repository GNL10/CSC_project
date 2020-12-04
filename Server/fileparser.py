
class FileParser:

    def __init__(self):
        print("defining a file parse")

    @staticmethod
    def parse(path):
        # f = open(path, "r")
        db_cmd = "create"
        db_key = (1,3)
        return db_cmd, db_key