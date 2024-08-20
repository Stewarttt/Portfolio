#Represents a users information
class User :
    """
    Creates a new User object:
        userID - the ID of the user being created
        startingAccountBalance - the amount of money in the users starting account
        """
    def __init__(self, userID, startingAccountBalance) :
        #contains the identifier for the user
        self.user = userID  
        #account balance is set to the initial balance in the user account
        self.balance = startingAccountBalance  
        #list of past user transactions
        self.transactions = []
    #END __init__
#END User

#Used for containing financial transactions between users
class Transaction :
    """
    Creates a new transaction object:
        sender - the ID of the user sending funds
        receiver - the ID of the user getting funds
        amount - the amount being sent between users
    """
    def __init__(self, sender, receiver, amount) :
        #contains the ID of the user sending funds
        self.sending = sender
        #contains the ID of the user receiving funds
        self.receiving = receiver
        #contains the amount being transfered between users
        self.amount = amount
    #END __init__
#END Transaction