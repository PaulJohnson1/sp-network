// https://en.wikipedia.org/wiki/Substitution%E2%80%93permutation_network

#include <iostream>
#define ROUNDS 100

class BitField {
public:
  uint8_t *array;

  BitField(uint8_t *array) {
    this->array = array;
  }

  inline bool at(uint32_t i) {
    return array[i >> 3] & (1 << (i & 7));
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
    seed = ((int64_t)seed * 934487 + 1123) % 2147483647;

    return seed;
  }
};

class Cipher {
private:
  int32_t seed;
  uint8_t *sBoxEncrypt;
  uint8_t *sBoxDecrypt;
  uint32_t *pBox;

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
    pBox = new uint32_t[size];
    for (int32_t i = 0; i < size; i++) {
      pBox[i] = i;
    }

    PRNG shufflePrng(seed);
    for (uint32_t i = 0; i < size; i++) {
      uint32_t index = shufflePrng.next() % size;
      uint32_t temp = pBox[i];
      pBox[i] = pBox[index];
      pBox[index] = temp;
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
    delete[] pBox;
  }

  inline void encrypt(uint8_t *array, uint32_t size) {
    BitField bits(array);
    generatePBoxes(size << 3);

    for (uint32_t i = 0; i < ROUNDS; i++) {
      for (uint32_t i = 0; i < size; i++) {
        array[i] = sBoxEncrypt[array[i]];
      }

      // apply the fisher yates shuffle on the bit level to distribute the ciphertext into more sboxes
      /*
      "A P-box is a permutation of all the bits: it takes the outputs of all the S-boxes of one round, permutes the bits, and feeds them into the S-boxes of the next round. A good P-box has the property that the output bits of any S-box are distributed to as many S-box inputs as possible."
      */
      for (int32_t i = 0; i < size << 3; i++) {
        uint8_t temp = bits.at(pBox[i]);
        bits.set(pBox[i], bits.at(i));
        bits.set(i, temp);
      }
    }
  }

  inline void decrypt(uint8_t *array, uint32_t size) {
    BitField bits(array);
    generatePBoxes(size << 3);

    for (uint32_t i = 0; i < ROUNDS; i++) {
      for (int32_t i = (size << 3) - 1; i >= 0; i--) {
        uint8_t temp = bits.at(pBox[i]);
        bits.set(pBox[i], bits.at(i));
        bits.set(i, temp);
      }

      for (uint32_t i = 0; i < size; i++) {
        array[i] = sBoxDecrypt[array[i]];
      }
    }
  }
};

int main() {
  Cipher thing(133);
  uint8_t arraylol[10] = {1,2,3,4,5,6,7,8,9,10};
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