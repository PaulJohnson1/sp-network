// https://en.wikipedia.org/wiki/Substitution%E2%80%93permutation_network

#include <iostream>

class BitField {
public:
  uint8_t *array;
  uint32_t length;

  BitField(uint8_t *array, uint32_t length) {
    this->array = array;
    this->length = length;
  }

  inline bool at(uint32_t i) {
    return array[i >> 2] & (1 << (i & 7));
  }

  inline void set(uint32_t i, bool value) {
    if (value) {
      array[i >> 3] |= 1 << (i & 7);
    } else {
      array[i >> 3] &= ~(1 << (i & 7));
    }
  }
};

class PRNG {
public:
  uint32_t seed;

  PRNG(uint32_t seed) {
    this->seed = seed;
  }

  inline uint32_t next() {
    seed = (seed * 934487 + 1123) % 2147483647;

    return seed;
  }
};

class Cipher {
private:
  int32_t seed;
  uint8_t *sBoxEncrypt;
  uint8_t *sBoxDecrypt;
  uint32_t *pBoxEncrypt;
  uint32_t *pBoxDecrypt;

  inline void generateSBoxes() {
    sBoxEncrypt = new uint8_t[256];
    sBoxDecrypt = new uint8_t[256];
  
    for (int32_t i = 0; i < 256; i++) {
      sBoxEncrypt[i] = i;
    }

    PRNG shufflePrng(seed);
    for (int32_t i = 0; i < 256; i++) {
      uint8_t index = shufflePrng.next() & 255;
      uint8_t temp = sBoxEncrypt[i];
      sBoxEncrypt[i] = sBoxEncrypt[index];
      sBoxEncrypt[index] = temp;
    }

    for (int32_t i = 0; i < 256; i++) {
      sBoxDecrypt[sBoxEncrypt[i]] = i;
    }
  }

  inline void generatePBoxes(uint32_t size) {
    pBoxEncrypt = new uint32_t[size];
    for (int32_t i = 0; i < size; i++) {
      pBoxEncrypt[i] = i;
    }

    PRNG shufflePrng(seed);
    for (uint32_t i = 0; i < size; i++) {
      uint32_t index = shufflePrng.next() % size;
      uint32_t temp = pBoxEncrypt[i];
      pBoxEncrypt[i] = pBoxEncrypt[index];
      pBoxEncrypt[index] = temp;
    }

    for (int32_t i = 0; i < size; i++) {
      pBoxDecrypt[pBoxEncrypt[i]] = i;
    }
  }

public:
  Cipher(int32_t seed) {
    this->seed = seed;
    generateSBoxes();
  }

  ~Cipher() {
    delete[] sBoxEncrypt;
    delete[] sBoxDecrypt;
  }

  inline void encrypt(uint8_t *array, uint32_t size) {
    BitField bits(array, size);

    for (uint32_t i = 0; i < size; i++) {
      array[i] = sBoxEncrypt[array[i]];
    }
  }

  inline void decrypt(uint8_t *array, uint32_t size) {
    BitField bits(array, size);

    for (uint32_t i = 0; i < size; i++) {
      array[i] = sBoxEncrypt[array[i]];
    }
  }
};

int main() {
  Cipher thing(133);
  uint8_t arraylol[10] = {1, 2, 3, 4, 5, 6, 7, 8, 8, 9};
  for (int32_t i = 0; i < 10; i++) {
    std::cout << std::to_string(arraylol[i]) << " ";
  }
  std::cout << std::endl;

  thing.encrypt(arraylol, 10);

  for (int32_t i = 0; i < 10; i++) {
    std::cout << std::to_string(arraylol[i]) << " ";
  }
  std::cout << std::endl;

  thing.decrypt(arraylol, 10);

  for (int32_t i = 0; i < 10; i++) {
    std::cout << std::to_string(arraylol[i]) << " ";
  }
  std::cout << std::endl;
} 