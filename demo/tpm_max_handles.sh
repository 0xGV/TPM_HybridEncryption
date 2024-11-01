

# Create the primary context - defining ownership for future objects.
tpm2_createprimary -c primary.ctx

# Create a new aes256 context - child of the primary context & loading the context to the TPM.
tpm2_create -C primary.ctx -Gaes256 -c key.ctx

# Load the new aes256 context into the TPM and output its context file into key.ctx.
# tpm2_load is unnecessary as you can create & load by specifying -c in the create command. 
#tpm2_load -C primary.ctx -u pub -r priv -c key.ctx

# Register the key.ctx to a persistent handle in the TPM - save the handle info in file "handle".
# This is easier than usig hardcoded address values in my opinion.
#
tpm2_evictcontrol -c key.ctx -o handle


# Remove Unnecessary files - All we need is the info from the 'handle' file 
# to perform ency/decry operations on data. 
