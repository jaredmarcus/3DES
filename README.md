<div align="center">
  <img src="/resources/Uni_logo.png" alt="Norwich University Logo" style="width: 250px; min-width: 250px;>
</div>

<div align="center">
  <h1 style="font-size: 3em; color: #e74c3c; text-align: center; text-shadow: 3px 3px #bdc3c7;">3DES Final Project - ECB/CBC</h1>
  <h4 style="font-size: 1.5em; color: #2980b9; text-align: center; margin-top: -10px;">Norwich University - MA399 </h4>
</div>

# Purpose
The original purpose of this repo was to dedicate a space to showcase my final project, as at the time of creation, nothing of this caliber was in existence (April of 2020). This final project was a C++ program that will take plaintext and showcase the rounds of encryption all the way back down to decryption. 

*Note 3DES is an outdated cryptographic method, please refrain from using this to encrypt your data.

## Language:
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

### Explanation:
For this final project I took it upon myself to create a working version of a 3DES implementation that takes in 3 user inputted keys, computes the 16 round keys for each key, then takes in a user plaintext to encrypt and then decrypt. The key part of this implementation is that I got 2 modes/methods of encryption ECB and CBC. Cipher Block Chaining (CBC) is a more encrypted method rather than Encrypted Codebook(ECB) for the fact that CBC has an initialized vector (IV) which is encrypted with the plaintext, rather than ECB just encrypting normally. The IV is xored with the plaintext, while the ECB doesn’t use an IV for encryption or decryption. A good way to view ECB and CBC is through this figure to the right. ECB doesn’t allow for a completely encrypted plaintext/file allowing it to be exploited which was the downfall of 3DES and DES.

![CBCvsECB](resources/CBCvsECB.png)

This project taught me a lot about the algorithm with the fact that it was very time consuming and wasn’t something that I could just do in a couple of hours like projects from MA318. I started off this project with a basic implementation of DES taking in a pre-set plaintext as well as key to encrypt and decrypt with. From there I built on the addition of the creating the 16 round keys for 2 additional keys and using that for the 3DES algorithm. I had issues at first when I first converted the project to 3DES regarding encryption. Encryption for 3DES uses the 16 keys straight through, decryption uses the key in reverse. For example, the original 1st round key would go to 16, and the 2nd would go to 15 and so on just switching the places of the round keys. The way the algorithm works is 3DES = K3 Enc(K2 Dec(K1 Enc (DES))). I had issues because I completely forgot to reverse the 16 round keys back to their original form for the 2nd key in the Decryption of 3DES, particularly the phase after I encrypted the plaintext because during 3DES encryption, key 2 is actually decrypting which means the keys were reverse. Upon this fix moving forward to add in interactive features to the program came with long hours. The program is fully working accepting user inputs of 8-character words for the 3 keys, the plaintext, as well as a mode selection for ECB or CBC with proper error handling. This was such a great project that I am going to publish it on a GitHub repository to gain further insight with addons that I can have later such as encryption of files, not just 8-character words. 

![Algorithm](resources/algorithm.png)
- **Encryption** - 3DES = K1 Encryption(K2 Decryption(K3 Encyption(DES)))
- **Decryption** - 3DES = K3 Decryption(K2 Encryption(K1 Decryption(DES)))

### Resources:

[3DES Explanation](https://www.youtube.com/watch?v=2O4dsChgcg8)


