from collections import Counter

class Table:
    name = ''   #name of the table
    owner = ''  #name of the owner of the table
    cols = []   #column names and type of data for each one
    info = []   #database info (list of rows)

    def __init__ (self, t_name, t_owner, t_cols):
        self.name = t_name
        self.owner = t_owner
        for i in t_cols: #splitting col name and type of data
            self.cols.append(i.split(' '))

    #prints all info stored on the table
    def print (self):
        print("\n##### Table Info #####")
        print("Name :", self.name)
        print("Owner :", self.owner)
        print("Cols :", self.cols)
        print("Info :", self.info, "\n\n")

    #inserts row into table
    def insert (self, row):
        if len(row) != len(self.cols):
            raise Exception("ERROR: Insert failed, column sizes must match!")
        self.info.append(row)
    
    def delete (self, index_list : list):
        for i in sorted(index_list, reverse=True):  # need to delete from the tail to the head!
            del self.info[i]
            
    # returns the columns (query_vars) of the rows (row_list) 
    def query (self, row_list, query_vars):
        cols = []
        res = []
        
        if isinstance(query_vars, list): #if there are several cols to be displayed
            for var in query_vars:
                cols.append(self.find_col_index(var))
        else:
            cols.append(query_vars)
        
        for r in range(len(row_list)): # iterating the number of rows where conditions were met
            res.append([])
            for c in range(len(cols)):
                res[r].append(self.info[row_list[r]][cols[c]])
        return res

    #finds rows in table according to conditions
    #logic_op 0 for AND 1 for OR
    def find (self, conditions , logic_op):
        line_list = []

        # running the search for the different conditions, appending every row that matches in line_list
        for c in conditions:
            colname = c[0]
            value = c[1]
            op = c[2]

            col_index = self.find_col_index(colname) # use column name to find the column index

            for row in range(len(self.info)):   # running through the list to find rows that match the condition
                if self.info[row][col_index] == value:  # COMPARE MUST BE DONE HERE TODO
                    line_list.append(row)

        if (logic_op == 0): #AND
            and_list = []
            counts = Counter(line_list) 
            for key, value in counts.items():
                if value > 1:   #if value appears more than once, the AND was successful for both conditions
                    and_list.append(key)
            return and_list

        elif (logic_op == 1): #OR
            or_list = []
            or_list = list(set(line_list))  #removing duplicate rows
            return or_list
        else:
            raise Exception("Logic_op : " + logic_op + " in find, is not valid")

    
    #finds the index of the column with colname
    def find_col_index(self, colname):
        col_index = -1
        for x in self.cols: # runs through the cols list
            if colname == x[0]: # col name is in pos 0 of the list
                col_index = self.cols.index(x)
                break
        if col_index == -1: # if column name is not in the list
            raise Exception("Column name is not in the cols list")
        return col_index

    @staticmethod
    def get_table(table_name):
        for t in database:
            if (t.name == table_name):
                return t
        raise Exception("Table " + table_name + " not found!")


        
database = []

def process_command(cmd):
    global database
    cmd_data  = cmd.split()
    print(cmd.split())

    #CREATE TABLE
    if (cmd_data[0].lower() == 'create' and cmd_data[1].lower() == 'table'):

        table_name = cmd_data[2]        # name of the table
        data = cmd.split('(')[1]        # selects part of the command after '('
        data = data.strip(')')          # deletes '(' in the end
        parameters = data.split(', ')   # columns of the table
    
        database.append(Table(table_name, "John", parameters))

    #INSERT INTO
    elif (cmd_data[0].lower() == 'insert' and cmd_data[1].lower() == 'into' and cmd_data[3].lower() == 'values'):
        table = Table.get_table(cmd_data[2]) #cmd_data[2] -> tablename
        row = cmd.split('(')[1].strip(')').split(', ')        # selects part of the command after '(' and deletes '(' in the end
               
        table.insert(row)
    
    #DELETE FROM
    elif (cmd_data[0].lower() == 'delete' and cmd_data[1].lower() == 'from' and cmd_data[3].lower() == 'where'):
        table = Table.get_table(cmd_data[2]) #cmd_data[2] -> tablename
        conditions, op_code = analyse_conditions(cmd.split(' WHERE ')[1])
        row_list = table.find(conditions, op_code)
        table.delete(row_list)

    # SELECT ... FROM
    elif (cmd_data[0] == 'SELECT' and cmd.find(' FROM ') != -1 and cmd.find(' WHERE ') != -1):
        table = Table.get_table(cmd.split(' FROM ')[1].split()[0]) #cmd.split(' FROM ')[1].split()[0] -> tablename
        conditions, op_code = analyse_conditions(cmd.split(' WHERE ')[1])
        row_list = table.find(conditions, op_code)

        # MULT
        if (cmd.find('MULT(') != -1):
            query_var = cmd.split('MULT(')[1].split(')')[0].replace(" ", "") # var inside MULT() and delete spaces
            
        # SUM
        elif (cmd.find('SUM(') != -1):
            query_var = cmd.split('SUM(')[1].split(')')[0].replace(" ", "") # var inside SUM() and delete spaces
        
        # Display specific columns
        else:
            query_var = cmd.split('SELECT ')[1].split(' FROM ')[0].split(', ') #list with variables to be displayed example : ['1stcol', '2ndcol', '3rdcol']
        
        return table.query(row_list, query_var)
        
        
    else:
        raise Exception("Did not recognize command : " + cmd)



# returns list of lists
# example: [['col1name', 'value1', 0], ['col2name', 'value2', 1]]
# 1st attribute is column name
# 2nd attribute is value
# 3rd attribute is the op code 
#   0 for =
#   1 for >
#   2 for <
def analyse_conditions (conditions_raw):
    res = []
    conditions = []
    op_code = -1

    if conditions_raw.find(' AND ') != -1: # 2 conditions AND
        conditions = conditions_raw.split(' AND ')
        op_code = 0
    elif conditions_raw.find(' OR ') != -1: # 2 conditions OR
        conditions = conditions_raw.split(' OR ')
        op_code = 1

    for c in conditions:
        if c.find(' = ') != -1:
            aux = c.split(' = ')
            aux.append(0)
            res.append(aux)

        elif c.find(' > ') != -1:
            aux = c.split(' > ')
            aux.append(1)
            res.append(aux)

        elif c.find(' < ') != -1:
            aux = c.split(' < ')
            aux.append(2)
            res.append(aux)
    return res, op_code


"""
process_command("CREATE TABLE tablename (1stcol int, 2ndcol int, 3rdcol text, 4thcol int)")
process_command("INSERT INTO tablename VALUES (1, 1asd, 2, 4)")
process_command("INSERT INTO tablename VALUES (3, kgmw, 1, 12)")
process_command("INSERT INTO tablename VALUES (1, kgmw, 1, 12)")
process_command("INSERT INTO tablename VALUES (1, kmw, 2, 12)")


#process_command("DELETE FROM tablename WHERE 1stcol = 1 AND 4thcol > 12")
#process_command("SELECT SUM( 1stcol ) FROM tablename WHERE 1stcol = 1 AND 3rdcol > 2")
Table.get_table("tablename").print()
res = process_command("SELECT 1stcol, 2ndcol, 3rdcol FROM tablename WHERE 1stcol = 1 AND 3rdcol > 2")
print(res)
"""