# [SP-Network](https://en.wikipedia.org/wiki/Substitution%E2%80%93permutation_network)

## status:
NOT FINISHED

## usage:
```cpp
  Cipher cipher(133);
  uint8_t array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 8, 9};

  cipher.encrypt(array, 10);
  cipher.decrypt(array, 10)
```