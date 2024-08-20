// Wait for the DOM content to be fully loaded before executing scripts
document.addEventListener('DOMContentLoaded', () => {
    // Handle form submission for creating a new user
    document.getElementById('createUserForm').addEventListener('submit', async (e) => {
        e.preventDefault(); // Prevent the default form submission behavior
        
        // Extract user ID and initial balance from the form
        const userId = document.getElementById('createUserId').value;
        const initialBalance = parseFloat(document.getElementById('createUserBalance').value);
        
        try {
            // Make a POST request to create a new user
            const response = await fetch('/users', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ user: userId, initial_balance: initialBalance })
            });

            // Parse the JSON response from the server
            const result = await response.json();
            // Display the success or error message to the user
            document.getElementById('createUserMessage').textContent = result.message || result.error;
        }
        catch (error) {
            // Handle any errors that occur during the fetch request
            document.getElementById('createUserMessage').textContent = 'An error occurred';
        }
    });

    // Handle form submission for transferring funds between users
    document.getElementById('transferFundsForm').addEventListener('submit', async (e) => {
        e.preventDefault(); // Prevent the default form submission behavior

        // Extract sending user ID, receiving user ID, and transfer amount from the form
        const sendingUser = document.getElementById('sendingUser').value;
        const receivingUser = document.getElementById('receivingUser').value;
        const amount = parseFloat(document.getElementById('transferAmount').value);

        try {
            // Make a POST request to transfer funds
            const response = await fetch('/transfer', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ sending: sendingUser, receiving: receivingUser, amount: amount })
            });

            // Parse the JSON response from the server
            const result = await response.json();
            // Display the success or error message to the user
            document.getElementById('transferMessage').textContent = result.message || result.error;
        }
        catch (error) {
            // Handle any errors that occur during the fetch request
            document.getElementById('transferMessage').textContent = 'An error occurred';
        }
    });

    // Handle form submission for getting the transaction history of a user
    document.getElementById('transactionHistoryForm').addEventListener('submit', async (e) => {
        e.preventDefault(); // Prevent the default form submission behavior

        // Extract user ID from the form
        const user = document.getElementById('transactionUser').value;

        try {
            // Make a GET request to retrieve the transaction history
            const response = await fetch(`/transactions/${user}`);
            // Parse the JSON response from the server
            const result = await response.json();
            // Display the transaction history or an error message to the user
            document.getElementById('transactionHistory').textContent = JSON.stringify(result.transactions, null, 2) || result.error;
        }
        catch (error) {
            // Handle any errors that occur during the fetch request
            document.getElementById('transactionHistory').textContent = 'An error occurred';
        }
    });

    // Handle form submission for checking the balance of a user
    document.getElementById('balanceForm').addEventListener('submit', async (e) => {
        e.preventDefault(); // Prevent the default form submission behavior

        // Extract user ID from the form
        const user = document.getElementById('balanceUser').value;

        try {
            // Make a GET request to retrieve the user's balance
            const response = await fetch(`/balance/${user}`);
            // Parse the JSON response from the server
            const result = await response.json();
            // Display the balance or an error message to the user
            document.getElementById('balanceMessage').textContent = result.balance ? `Balance: ${result.balance}` : result.error;
        }
        catch (error) {
            // Handle any errors that occur during the fetch request
            document.getElementById('balanceMessage').textContent = 'An error occurred';
        }
    });
});
