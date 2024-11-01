#! /usr/bin/bash
#
#Must be run with SUDO privileges

tpm2_getrandom --hex 32 -o demo_plaintext.data 
tpm2_encryptdecrypt -c handle -o enc.data demo_plaintext.data

# Echo the encrypted data and the plaintext
#
echo "Printing the demo_plaintext.data to stdout"
cat demo_plaintext.data
echo ""
echo "Printing the enc.data to stdout" 
cat enc.data
echo""
echo "Removing the plaintext file" 
rm -f demo_plaintext.data

