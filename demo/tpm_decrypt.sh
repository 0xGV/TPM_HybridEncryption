#! /usr/bin/bash
#
#

tpm2_encryptdecrypt -d -c key.ctx -o restored_plaintext.data enc.data

#
echo "Printing restored planitext:"
md5sum restored_plaintext.data
