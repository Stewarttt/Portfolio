#imported libraries
import requests
import nacl.hash, nacl.public, nacl.encoding, nacl.signing
import base64

"""
QUESTION 5 PART A SOURCE-CODE
"""

#sets the URL for the set key
URLSet = "https://hash-browns.cs.uwaterloo.ca/api/keyex/set-key"

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

#contains the body of the request
body = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     "public_key" : publicKeyEncoded
     }

#contains the header for the request
header = {"Content-Type" : "application/json",
          "Accept" : "application/json"}

#send a request to the server to upload the key
responseData = requests.post(URLSet, headers = header, json = body)

"""
QUESTION 5 PART B SOURCE-CODE
"""

#sets the URL for the get key
URLGet = "https://hash-browns.cs.uwaterloo.ca/api/keyex/get-key"

#contains the URL for the inbox request
URLInbox = "https://hash-browns.cs.uwaterloo.ca/api/keyex/inbox"

#contnains the body of the message request
bodyMessage = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
     "user" : "Scholar"
     }

#send a request to get a message from the server
responseData = requests.post(URLGet, headers = header, json = bodyMessage)

#get the key for user
#publicKeyData = responseData.json()["public_key"]
#publicKeyDecoded = base64.b64decode(publicKeyData)
#publicKey = nacl.public.PublicKey(publicKeyDecoded)
#print(publicKeyData)
publicKeySigned = base64.b64decode(responseData.json()["public_key"])

publicKey = nacl.public.PublicKey(publicKeySigned)

#create a box for the key
keyBox = nacl.public.Box(privateKey, publicKey)

#contains the body of the message request to get a message
bodyRequest = \
     {
     "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4"
     }

#send a request to get a reply from the user
responseData = requests.post(URLInbox, headers = header, json = bodyRequest)

#decode the reply from the server
decodedResponse = base64.b64decode(responseData.json()[0]["message"])

#decrypt the response
nonce = decodedResponse[:24]
ciphertext = decodedResponse[24:]

keyBox.decrypt(decodedResponse[24:], decodedResponse[:24])

#decrypt the message

