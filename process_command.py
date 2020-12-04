def process_command():
    cmd = "CREATE TABLE tablename (1stcol int, 2ndcol int, 3rdcol text, 4thcol int)"
    cmd_data  = cmd.split()
    print(cmd.split())

    #example commands

    #DELETE FROM tablename WHERE col1name = value1 AND col2name > value2

    #INSERT INTO TABLE (col1name, … , colNname) VALUES (value1, .., valueN)
    #DELETE FROM tablename WHERE col1name =|<|> value1 AND|OR col2name =|<|> value2
    
    # Process the commands depending on the syntax
    if (cmd_data[0].lower() == 'create'): # creating a new table
        if (cmd_data[1].lower() != 'table'): 
            print("ERROR: Did not recognize command:\n", cmd)
            return
        
        table_name = cmd_data[2] # name of the table
        data = cmd.split('(')[1] # selects part of the command after '('
        data = data.strip(')') # deletes '(' in the end
        parameters = data.split(', ') # columns of the table
        for col in parameters:
            print(col.split(' '))

    elif (cmd_data[0].lower() == 'insert'):
        print('Read insert')
    
    # DELETE 
    elif (cmd_data[0].lower() == 'delete'):
        if (cmd_data[1].lower() != 'from' or cmd_data[3].lower() != 'where'): 
            print("ERROR: Did not recognize command:\n", cmd)
            return
        #           table_name    conditions
        delete_row(cmd_data[2], cmd.split('WHERE ')[1])

    elif (cmd_data[0].lower() == 'select'):
        print('Read select')
    else:
        print("ERROR: Did not recognize command:\n", cmd)



def delete_row (table_name, conditions):
    #1st condition
    if conditions.find(' AND ') != -1: # 2 conditions AND
        conditions = conditions.split(' AND ')
        conditions = analyse_conditions(conditions)
        # perform 1st condition
        # perform 2nd condition
        
    elif conditions.find(' OR ') != -1: # 2 conditions OR
        conditions = conditions.split(' AND ')
        conditions = analyse_conditions(conditions)

    else : # 1 condition
        conditions = analyse_conditions(conditions)
    #2nd condition



# returns list of lists
# example: [['col1name', 'value1', 0], ['col2name', 'value2', 1]]
# 1st attribute is column name
# 2nd attribute is value
# 3rd attribute is the op code 
#   0 for =
#   1 for >
#   2 for <
def analyse_conditions (conditions):
    res = []
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
    return res

process_command()