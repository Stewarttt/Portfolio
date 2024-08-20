Setup and Dependencies:
	Install Flask via pip: pip install Flask

Directory Structure:
	banking_api/
	├── app.py
	├── controllers.py
	├── services.py
	├── repositories.py
	├── models.py
	└── dtos.py

Running the Application:
		Run the Flask application with: python controllers.py
	Your API will be available at http://127.0.0.1:5000
		You can test it using tools like curl, Postman, or httpie as well, if you comment out:
			app = Flask(__name__, static_folder='static', template_folder='templates')
		and replace it with the following:
			app = Flask(__name__)
	This implementation covers basic functionality and can be extended as needed. For a production system, you’d likely add persistent storage, more comprehensive validation, and additional features.

Explanation:
	Models: Define data structures for users and transactions.
	DTOs: Provide structured data formats for requests and responses.
	Repositories: Handle in-memory storage and retrieval of users and transactions.
	Services: Contain the business logic, such as user creation, fund transfers, and transaction history retrieval.
	Controllers: Expose API endpoints, handle HTTP requests, and interact with the service layer to perform operations.
	This code covers all the essential aspects of the problem, including endpoint paths, dependency injection, DTO usage, input validation, and error handling.

Example CURL Commands:
	curl -X POST http://localhost:5000/users -H "Content-Type: application/json" -d "{\"user\": \"user1\", \"initial_balance\": 1000}"
	curl -X POST http://localhost:5000/users -H "Content-Type: application/json" -d "{\"user\": \"user2\", \"initial_balance\": 0}"
	curl -X POST http://localhost:5000/transfer -H "Content-Type: application/json" -d "{\"sending\": \"user1\", \"receiving\": \"user2\", \"amount\": 2000}"
	curl -X POST http://localhost:5000/transfer -H "Content-Type: application/json" -d "{\"sending\": \"user1\", \"receiving\": \"user2\", \"amount\": 200}"
	curl -X GET http://localhost:5000/transactions/user1
	curl -X POST http://localhost:5000/transfer -H "Content-Type: application/json" -d "{\"sending\": \"invalid_user\", \"receiving\": \"user2\", \"amount\": 100}"
