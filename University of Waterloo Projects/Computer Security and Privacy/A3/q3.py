#imported libraries
import requests
import nacl, nacl.signing
import base64

"""
QUESTION 3 PART A SOURCE-CODE
"""

#sets the URL for the post request page
URL = "https://hash-browns.cs.uwaterloo.ca/api/signed/set-key"

#generates a secret signature key annd its associated public verification key
signingKey = nacl.signing.SigningKey.generate()
verificationKey = signingKey.verify_key

#encode the verification key for transmission
verificationKeyEncode = verificationKey.encode(encoder = nacl.encoding.Base64Encoder)

#convert the base64 format to a python-string format
verificationKeyEncode = str(verificationKeyEncode)[2:-1]

#contains the body of the request
body = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     "public_key" : verificationKeyEncode
     }

#contains the header for the request
header = {"Content-Type" : "application/json",
          "Accept" : "application/json"}

#sennd a request to the server to upload the verification key
responseData = requests.post(URL, headers = header, json = body)

#return the status code from the server
print("Status Code:", responseData.status_code)

"""
QUESTION 3 PART B SOURCE-CODE
"""

#set the URL of the response page
responseURL = "https://hash-browns.cs.uwaterloo.ca/api/signed/send"

#sign and encode the message to be sent to the website
message = base64.b64encode(signingKey.sign("Hello, World!".encode("ascii")))

#convert the base64 format to a python-string format
message = str(message)[2:-1]

#contains the body for the message
body = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     "to" : "Scholar",
     "message" : message
     }

#send a message to the server 
responseData = requests.post(responseURL, headers = header, json = body)

#print the response from the server
print("RESPONSE:")
print(responseData.text, responseData.status_code, responseData.reason)
