#imports repositories and models for users and transactions
from repositories import UserRepository, TransactionRepository
from models import User, Transaction

class BankingService :
    def __init__(self, user_repo : UserRepository, transaction_repo : TransactionRepository) :
        #initialize repositories to manage users and transactions
        self.user_repo = user_repo
        self.transaction_repo = transaction_repo
    #END __init__

    #transfers funds between users
    def transfer_funds(self, sending : str, receiving : str, amount : float) :
        #gets the sending user
        from_user = self.user_repo.get_user(sending)

        #gets the receiving user
        to_user = self.user_repo.get_user(receiving)

        #check if the users are not valid enteries
        if from_user is None :
            #raise an error to notify that the users ID is not in the user dataset
            raise ValueError("Invalid sender ID")
        #END IF

        #check if the users are not valid enteries
        if to_user is None :
            #raise an error to notify that the users ID is not in the user dataset
            raise ValueError("Invalid receiver ID")
        #END IF

        #check if the balance is less then the amount being requested for transfer
        if from_user.balance < amount :
            #raise an error to notify that the sender's balance is not sufficient
            raise ValueError("Insufficient funds")
        #END IF

        #transfer the funds between users
        from_user.balance -= amount
        to_user.balance += amount
        self.user_repo.update_user(from_user)
        self.user_repo.update_user(to_user)

        #record the transaction
        transaction = Transaction(sending, receiving, amount)

        #add the record to the list of all transactions
        self.transaction_repo.add_transaction(transaction)
    #END transfer_funds

    #creates new users with initial balances
    def create_user(self, user : str, initial_balance : float) :
        if self.user_repo.get_user(user) :
            #raise an error if a duplicate user ID is entered
            raise ValueError("User already exists")
        #END IF

        #create a new user object
        new_user = User(user, initial_balance)

        #add the user to the list of all users
        self.user_repo.add_user(new_user)
    #END create_user

    #gets transaction history for a specific user
    def get_transaction_history(self, user : str) :
        #gets the user
        userName = self.user_repo.get_user(user)

        #checks to make sure the user name exists
        if userName is None :
            #raise an error to notify that the users ID is not in the user dataset
            raise ValueError("Invalid receiver ID")
        #END IF

        return self.transaction_repo.get_transactions(user)
    #END get_transaction_history

    #gets the balance for a specific user
    def get_balance(self, user : str) :
        #gets the user
        userName = self.user_repo.get_user(user)

        #checks to make sure the user name exists
        if userName is None :
            #raise an error to notify that the users ID is not in the user dataset
            raise ValueError("Invalid receiver ID")
        #END IF

        #get the account balance
        balance = self.user_repo.get_balance(user)

        #check to make sure there is a value in the balance
        if balance is not None:
            return {'balance': balance}
        #END IF

        #return an error value
        return {'error': 'No Account Balance'}
    #END get_balance
#END BankingService