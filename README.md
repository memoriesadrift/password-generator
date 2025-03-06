# Libsodium Password Generator

A basic CLI password generator using libsodium for secure rng.  
Supports options for memorable mnemonic passwords based on BIP-39 as well as all the common requirements placed on passwords.

# Requirements
[Libsodium installed](https://doc.libsodium.org/installation)

# Usage
```
# compile
> make

# generate a password with [c]aps [s]pecial characters and [n]umbers
# with [l]ength 15
> ./main.x -csn -l 15
Generated Password: 
DF7&^88*1A7VLR#

# generate a [m]nemonic password with [l]ength 15
> ./main.x -m -l 15
Generated Password: 
glue rhythm leopard muffin rate disagree illness price ship taxi fat hurry ahead anger exit 
```
