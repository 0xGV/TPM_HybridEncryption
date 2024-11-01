#! /usr/bin/bash 
#

#Generate RSA private key using openssl
openssl genrsa -out private.pem 2048

#Create a public key from the private key
openssl rsa -in private.pem -pubout -out public.pem

#Create a primary object (Object must be type RSA - Required for importing the RSA keys) 
tpm2_createprimary -c primary.ctx -Grsa2048:aes128cfb 

#Import the RSA key to and create public & private portions to the TPM object.
tpm2_import -C primary.ctx -G rsa -i private.pem -u rsa_obj.pub -r rsa_obj.priv

#Load the TPM object and create a context
tpm2_load -C primary.ctx -u rsa_obj.pub -r rsa_obj.priv -c rsa.ctx

#Create a handle to the new rsa.ctx
tpm2_evictcontrol -c rsa.ctx -o rsa_handle

#Remove unecessary files
rm -rf private.pem primary.ctx parent.ctx rsa_obj.pub rsa_obj.priv rsa.ctx

#List file directory
ls -al

#Create some random data to encrypt
tpm2_getrandom --hex 32 -o dummy.data

#Print md5 hash of the plaintext
echo "md5sum plaintext:" 
md5sum dummy.data

#Perform encryption using openssl and the public.pem
openssl pkeyutl -encrypt -pubin -inkey public.pem -in dummy.data > enc.data

#Print md5sum of the cipher text
echo "md5sum ciphertext:"
md5sum enc.data

#Remove the plaintext file
rm -f dummy.data

#Decrypt the ciphertext using the TPM
tpm2_rsadecrypt -c rsa_handle -o restored.data enc.data

echo "md5sum restored text:"
md5sum restored.data

#Removing text files to test key retrieval & use
rm -f enc.data restored.data

#Listing directories and files again
ls -al

echo "Starting test suite with key retrieved from TPM"
echo "==============================================="
#Retrieve the public key from the TPM and format it into a .pem file to work with openssl.
tpm2_readpublic -c rsa_handle -f pem -o new_public.pem

#Create some new random data to encrypt
tpm2_getrandom --hex 32 -o new_dummy.data

echo "md5sum plaintext:" 
md5sum new_dummy.data

#Perform encryption using openssl and the public.pem
openssl pkeyutl -encrypt -pubin -inkey new_public.pem -in new_dummy.data > new_enc.data

#Print md5sum of the cipher text
echo "md5sum ciphertext:"
md5sum new_enc.data

#Remove the plaintext file
rm -f new_dummy.data

#Decrypt the ciphertext using the TPM
tpm2_rsadecrypt -c rsa_handle -o new_restored.data new_enc.data

echo "md5sum restored text:"
md5sum new_restored.data

rm -f new_enc.data new_restored.data new_public.pem

echo "Starting test suite with Original public key"
echo "==============================================="

#Create some new random data to encrypt
tpm2_getrandom --hex 32 -o dummy.data

echo "md5sum plaintext:" 
md5sum dummy.data

#Perform encryption using openssl and the public.pem
openssl pkeyutl -encrypt -pubin -inkey public.pem -in dummy.data > enc.data

#Print md5sum of the cipher text
echo "md5sum ciphertext:"
md5sum enc.data

#Remove the plaintext file
rm -f dummy.data

#Decrypt the ciphertext using the TPM
tpm2_rsadecrypt -c rsa_handle -o restored.data enc.data

echo "md5sum restored text:"
md5sum restored.data

rm -f enc.data restored.data public.pem

#Cleanup
tpm2_evictcontrol -c rsa_handle
rm -f rsa_handle
