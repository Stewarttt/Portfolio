#imported libraries
import requests
import base64
import nacl, nacl.utils, nacl.secret

"""
QUESTION 2 PART A SOURCE-CODE
"""

#contains the pre-shared key used to connect
presharedKeyHex = "4ac033ad58c520f7dc7f56cc9227c54c173a3feb00b96336e1a08b721096e7fb"

#get the key string
key = bytes.fromhex(presharedKeyHex)

#get the number used once
nonce = nacl.utils.random(nacl.secret.SecretBox.NONCE_SIZE)

#contains the plaintext message
plaintext = "Hello, World!".encode("ascii")

#encrypt the message with the followinng nonce
encryptedMessageBytes = nacl.bindings.crypto_secretbox(plaintext, nonce, key)

#concatinate the nonce with the secretbox and convert to base64 encoding
messageEncoded = base64.b64encode(nonce + encryptedMessageBytes)

#remove the bytes tag and convert to a python-string
messageEncoded = str(messageEncoded)[2:-1]

#the link to the course webpage
URL = "https://hash-browns.cs.uwaterloo.ca/api/psk/send"

#contains the body of the request to the server
body = \
    {
    "api_token" : "8bee87064ce76e1e52a43d21c06240b56f00076c222d993862bb46cbe341ead4",
    "to" : "Scholar",
    "message" : messageEncoded
    }

#contains the header for the request
header = {"Content-Type" : "application/json",
          "Accept" : "application/json"}

#send the request and convert the body to JSON format internally
responseData = requests.post(URL, headers = header, json = body)

#return the status code from the server
print("Status Code:", responseData.status_code)

"""
QUESTION 2 PART B SOURCE-CODE
"""

#contains the link for the reply from the server
responseURL = "https://hash-browns.cs.uwaterloo.ca/api/psk/inbox"

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
print("ENCRYPTED-DECODED-MESSAGE:")
print(decodedMessage)

#create a secretbox to decrypt the response
secretbox = nacl.secret.SecretBox(key)

#unencrypt the decoded message
encryptedMessageBytes = secretbox.decrypt(decodedMessage)

#print the decrypted message
print("DECRYPTED-DECODED-MESSAGE")
print(encryptedMessageBytes)
