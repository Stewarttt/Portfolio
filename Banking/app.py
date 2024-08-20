#imports flask, services, repostories, and data transfer object libraries
from flask import Flask, request, jsonify, send_from_directory, render_template
from services import BankingService
from repositories import UserRepository, TransactionRepository
from dtos import CreateUserRequest, TransferFundsRequest, TransactionHistoryResponse

#define the flask applicaiton, and use using the web page interface
app = Flask(__name__, static_folder='static', template_folder='templates')
#use the following if you want to run the program using CURL
#app = Flask(__name__)

#an instance of UserRepository for managing user data
user_repo = UserRepository()

#an instance of TransactionRepository for managing transaction data
transaction_repo = TransactionRepository()

#an instance of BankingService that uses the above repositories
banking_service = BankingService(user_repo, transaction_repo)

#serves the main page
@app.route('/')
def index() :
	#returns the rendering template to the caller
    return render_template('index.html')
#END index

"""
Name: create_user
Description: defines an endpoint to create a new user with an given initial balance amount
Method:
	Deserializes request data into a CreateUserRequest Data Transfer Object
	Calls banking_service to create a new user object
	Returns success message if successful or an error message if an exception is raised
"""
@app.route('/users', methods = ['POST'])
def create_user() :
    data = request.json
    try :
        #deserialize request data into the data transfer objects
        req = CreateUserRequest(**data)
        #create a new user by calling the service method
        banking_service.create_user(req.user, req.initial_balance)
		
		#return a success message to the user
        return jsonify({"message" : "User created successfully"}), 201
    except ValueError as e :
		#returns an error response if there is an issue
        return jsonify({"error" : str(e)}), 400
	#END TRY
#END create_user

"""
Name: transfer_funds
Description: define an endpoint to transfer funds between users
Method:
	Deserializes request data into a TransferFundsRequest Data Transfer Object
	Calls banking_service to start a transfer object
	Returns success message if successful or an error message if an exception is raised
"""
@app.route('/transfer', methods = ['POST'])
def transfer_funds() :
    data = request.json
    try :
        #deserialize request data into data transfer objects
        req = TransferFundsRequest(**data)
        #service method call used to perform transfer
        banking_service.transfer_funds(req.sending, req.receiving, req.amount)
        return jsonify({"message" : "Transfer successful"}), 200
    except ValueError as e :
		#returns error response to the caller
        return jsonify({"error" : str(e)}), 400
	#END TRY
#END transfer_funds

"""
Name: get_transaction_history
Description: defines an endpoint to get the transaction history from a user
Method:
	Deserializes request data into a TransactionHistoryResponse Data Transfer Object
	Calls banking_service to get a transaction history list
	Returns success message if successful or an error message if an exception is raised
"""
@app.route('/transactions/<user>', methods = ['GET'])
def get_transaction_history(user) :
    try :
        #gets transaction histories
        transactions = banking_service.get_transaction_history(user)
        #returns the transaction histories as JSON responses
        response = TransactionHistoryResponse(transactions = [t.__dict__ for t in transactions])
        return jsonify(response.__dict__), 200
    except ValueError as e :
		#returns error response to the caller
        return jsonify({"error" : str(e)}), 400
	#END TRY
#END get_transaction_history

"""
Name: get_balance
Description: gets the balance for a specific user
Method:
	Deserializes request data into a TransactionHistoryResponse Data Transfer Object
	Calls banking_service to get a transaction history list
	Returns success message if successful or an error message if an exception is raised
"""
@app.route('/balance/<user>', methods = ['GET'])
def get_balance(user) :
    try :
        #return the balance of the account to the user
        result = banking_service.get_balance(user)
        return jsonify(result), 200
    except ValueError as e :
		#returns error response to the caller
        return jsonify({"error" : str(e)}), 404
	#END TRY
#END get_balance

#serves static files (CSS and JS)
@app.route('/static/<path:filename>')
def serve_static(filename) :
	#starts the javascript file with the CSS format
    return send_from_directory('static', filename)
#END serve_static

#runs if the main program is called directly
if __name__ == '__main__' :
	#starts the Flask program
    app.run(debug = True)
#END IF
