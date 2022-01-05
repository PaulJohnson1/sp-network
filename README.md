# [SP-Network](https://en.wikipedia.org/wiki/Substitution%E2%80%93permutation_network)

## status:
slow but secure

## properties:
confusion and diffusion

## usage:
```cpp
  Cipher encryption(133);
  uint8_t plaintext[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t ciphertext[10];
  for (int32_t i = 0; i < 10; i++) {
    std::cout << std::to_string(plaintext[i]) << " ";
  }
  std::cout << std::endl;

  encryption.encrypt(plaintext, 10, ciphertext);

  for (int32_t i = 0; i < 10; i++) {
    std::cout << std::to_string(ciphertext[i]) << " ";
  }
  std::cout << std::endl;

  encryption.decrypt(ciphertext, 10, plaintext);

  for (int32_t i = 0; i < 10; i++) {
    std::cout << std::to_string(plaintext[i]) << " ";
  }
  std::cout << std::endl;
```