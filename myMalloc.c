#define MEMORY_SIZE 64000

#include <stdio.h>
#include <stdint.h>


uint8_t MY_HEAP[MEMORY_SIZE];

void init() {
    //remise à 0 pour éviter d'avoir des fausses méta-données
    for (int i = 0; i < MEMORY_SIZE; i++) {
        MY_HEAP[i] = 0;
    }
}

// revoie 1 si le bloc est libre, 0 sinon
int is_free(uint16_t address) {
    return MY_HEAP[address-1] & 1 == 0;
}

// revoie la taille d'un bloc alloué ou non
uint16_t get_size(uint16_t address) {
    return MY_HEAP[address-2] << 8 + MY_HEAP[address-1] & 254;
}

uint16_t search_for_block(size_t size, uint16_t address) {
    //if (!is_free(address)) search_for_block(size, )
}

void *my_malloc(size_t size) {
    uint16_t address = search_for_block(size, 2);
}

void free(void *pointer) {

}

int main(int argc, char **argv) {
    return 0;
}
