#! /usr/bin/bash
#
# Must be run with SUDO privileges 

# Setting up dictionary lockout parameters to mitigate dictionary attacks against TPM.
#tpm2_dictionarylockout -s -n 3 -t 120 -l 120 -p insupasswd --clearlockout

# Create the primary context - defining ownership for future objects.
tpm2_createprimary -c primary.ctx

# Create a new aes256 context - child of the primary context & loading the context to the TPM.
tpm2_create -C primary.ctx -Gaes128 -c key.ctx

# Load the new aes256 context into the TPM and output its context file into key.ctx.
# tpm2_load is unnecessary as you can create & load by specifying -c in the create command. 
#tpm2_load -C primary.ctx -u pub -r priv -c key.ctx

# Register the key.ctx to a persistent handle in the TPM - save the handle info in file "handle".
# This is easier than usig hardcoded address values in my opinion.
#
tpm2_evictcontrol -c key.ctx -o handle

# Remove Unnecessary files - All we need is the info from the 'handle' file 
# to perform ency/decry operations on data. 
rm -f primary.ctx key.ctx

#Performing an example encryption 

#Generate some plaintext - using TPM's pRNG.
#tpm2_getrandom --hex 2048 -o 2demo_plaintext.data

#Encrypt the data using handle to the 
#tpm2_encryptdecrypt -c handle -o enc.data2 demo_plaintext.data

# Echo the encrypted data and the plaintext
#echo "Generating md5 hash of the plaintext:"
#md5sum demo_plaintext.data
#echo ""
#echo "Generating md5 hash of the ciphertext" 
#md5sum enc.data
#echo""
#echo "Removing the plaintext file" 
#rm -f demo_plaintext.data
##
# List all files/directories verify that the plaintext has been removed.
#ls -al

#Performing an example decryption
#tpm2_encryptdecrypt -d -c handle -o restored_plaintext.data enc.data
#echo "Generating md5 hash of the restored plaintext"
#md5sum restored_plaintext.data

#Removing persistent handle
#tpm2_evictcontrol -c handle

#rm -f handle



