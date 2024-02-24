#imported libraries
import requests
import nacl.hash, nacl.public, nacl.encoding
import base64

"""
QUESTION 4 PART A SOURCE-CODE
"""

#sets the URL for the post request page
URL = "https://hash-browns.cs.uwaterloo.ca/api/pke/get-key"

#contains the body of the request
body = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     "user" : "Scholar"
     }

#contains the header for the request
header = {"Content-Type" : "application/json",
          "Accept" : "application/json"}

#send the request and convert the body to JSON format internally
responseData = requests.post(url = URL, json = body, headers = header)

#get the public key string
publicKeyData = responseData.json()["public_key"]

#decode the public key string
publicKeyString = base64.b64decode(publicKeyData)

#derive a fingerprint by passing the key through a cryptographic hash function
fingerprint = nacl.hash.blake2b(publicKeyString)

#return the hash value of the public-key to the shell
print("Hash: " + str(fingerprint))

"""
QUESTION 4 PART B SOURCE-CODE
"""

#sets the URL for the post request page
URLMessage = "https://hash-browns.cs.uwaterloo.ca/api/pke/send"

#sets the URL for the set key
URLSet = "https://hash-browns.cs.uwaterloo.ca/api/pke/set-key"

#generate a private key
privateKey = nacl.public.PrivateKey.generate()
#obtain the matching public key
publicKey = privateKey.public_key

#encode the public key
publicKeyEncoded = publicKey.encode(nacl.encoding.Base64Encoder)

#remove formatting to convert to a python string
publicKeyEncoded = str(publicKeyEncoded)[2:-1]

#contains the message to be send to the user
plaintextMessage = b"Hello, World!"

#define an encryption funnction
boxFunction = nacl.public.Box(privateKey, nacl.public.PublicKey(publicKeyData, nacl.encoding.Base64Encoder))

#encrypt the message
encryptedMessage = boxFunction.encrypt(plaintextMessage)

#convert the binary to a python string
encryptedMessage = str(base64.b64encode(encryptedMessage))[2:-1]

#contains the body of the request for the key setting
bodySet = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     "public_key" : publicKeyEncoded
     }

#contains the body of the request for the message sending
bodyMessage = \
        {
        "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
        "to" : "Scholar",
        "message" : encryptedMessage
        }

#send the request to set the keys and convert the body to JSON format internally
responseData = requests.post(URLSet, json = bodySet, headers = header)

#send the request to upload the message and convert the body to JSON format internally
responseData = requests.post(URLMessage, json = bodyMessage, headers = header)

#return the result of the message request to the shell
print("Status Code (Message):", responseData.status_code)

"""
QUESTION 4 PART C SOURCE-CODE
"""

#set the URL to get a reply from the server
URLReply = "https://hash-browns.cs.uwaterloo.ca/api/pke/inbox"

#contains the body of the request to get a reply from the server
bodyReply = \
          {
          "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4"
          }

#send the request to get a message from the server
responseData = requests.post(URLReply, json = bodyReply, headers = header)

#decode the response data
codedResponse = base64.b64decode(responseData.json()[0]["message"])

#decrypt the servers message to memory
response = boxFunction.decrypt(codedResponse)

#return the response from the server to the shell
print("Servers Message: " + str(response))
