#imported libraries
import requests
import base64
import nacl, nacl.utils, nacl.secret

"""
QUESTION 1 PART A SOURCE-CODE
"""

#contains the message
message = base64.b64encode("Hello, World!".encode("ascii"))

#remove the bytes tag and convert to a python-string
message = str(message)[2:-1]

#the link to the course webpage
URL = "https://hash-browns.cs.uwaterloo.ca/api/plain/send"

#contains the body of the request to the server
body = \
    {
    "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
    "to" : "Scholar",
    "message" : message
    }

#contains the header for the request
header = {"Content-Type" : "application/json",
          "Accept" : "application/json"}

#send the request and convert the body to JSON format internally
responseData = requests.post(URL, headers = header, json = body)

#return the status code from the server
print("Status Code:", responseData.status_code)

"""
QUESTION 1 PART B SOURCE-CODE
"""

#contains the link for the reply from the server
responseURL = "https://hash-browns.cs.uwaterloo.ca/api/plain/inbox"

#conntains the body of the request to the server
body = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     }

#send the request to the server and convert the body to JSON format internally
responseData = requests.post(responseURL, headers = header, json = body)

#print the responnse from the server
print("RESPONSE:")
print(responseData.text, responseData.status_code, responseData.reason)

#decode the response from base64 to python-string format
decodedMessage = base64.b64decode(responseData.json()[0]['message'])

#print the decoded message to the user
print("DECODED-MESSAGE:")
print(decodedMessage)
