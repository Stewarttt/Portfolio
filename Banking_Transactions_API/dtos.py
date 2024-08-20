#import dataclasses
from dataclasses import dataclass

# Data Transfer Object (DTO) for creating a new users
@dataclass
class CreateUserRequest:
    #string representation for a users ID
    user : str
    #float representation for the initial account balance for a new user
    initial_balance: float
#END CreateUserRequest

# Data Transfer Object (DTO) for transferring funds from one user to another
@dataclass
class TransferFundsRequest:
    #string representation for sending user
    sending : str
    #string representation for receiving user
    receiving : str
    #float representation for the amount being transfered
    amount : float
#END TransferFundsRequest

# Data Transfer Object (DTO) for obtaining transaction history
@dataclass
class TransactionHistoryResponse:
    #list representation of the number of transactions for the given user
    transactions : list
#END TransactionHistoryResponse