# Enigma-Machine-Emulator
As the name suggests, the one and only c file in this repository emulates the working of an enigma machine.
It is hardcoded with 5 rotors which the developer may change for more secure sequences.
In addition to this each rotor can be programmed for its initial offset.

# Complexities
The main challenge behind the emulator is the fact that an actual enigma machine use a closed circuit path to ensure that when in ith state; if character c ciphers to character c' then it is guaranteed that character c' would have ciphered to character c.

# NOTE
The emulator does not supports plugboard yet. A minor complication for additional security.
