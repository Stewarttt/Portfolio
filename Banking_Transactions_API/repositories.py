#imports modules for users and transactions
from models import User, Transaction

"""
    Name: __init__
    Description: repository for mapping user data, including:
        self.users - storage for all user objects, indexed by user ID.
"""
class UserRepository:
    def __init__(self) :
        #memory storage for user objects
        self.users = {}
    #END __init__

    #gets a user by their user ID
    def get_user(self, user : str) -> User :
        return self.users.get(user)
    #END get_user

    #adds a new user to the repository
    def add_user(self, user : User) :
        self.users[user.user] = user
    #END add_user

    #updates an existing users data
    def update_user(self, user : User) :
        self.users[user.user] = user
    #END update_user

    #gets a users balance from their username
    def get_balance(self, user : User):
        return self.users.get(user).balance
    #END get_balance
#END UserRepository

"""
    Name: __init__
    Description: repository for managing transaction data, including:
        self.transactions - storage for all user transactions, indexed by user ID.
"""
class TransactionRepository :
    #memory storage for user transactions
    def __init__(self) :
        self.transactions = []
    #END __init__

    #retrieves transactions related to a given user
    def get_transactions(self, user : str) :
        return [t for t in self.transactions if t.sending == user or t.receiving == user]
    #END get_transactions

    #adds a new transaction to the system repository
    def add_transaction(self, transaction : Transaction) :
        self.transactions.append(transaction)
    #END add_transaction
#END TransactionRepository