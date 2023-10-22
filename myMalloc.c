#define MEMORY_SIZE (uint16_t)500
#define NO_SPACE_FOUND -1

#include <stdio.h>
#include <stdint.h>


uint8_t MY_HEAP[MEMORY_SIZE];

void init() {
    // remise à 0 pour éviter d'avoir des fausses méta-données
    for (int i = 0; i < MEMORY_SIZE; i++) {
        MY_HEAP[i] = 0;
    }
    // premier bloc = libre et fait la taille de toute le mémoire
    MY_HEAP[0] = (uint8_t)(MEMORY_SIZE >> 8);
    MY_HEAP[1] = (uint8_t)(MEMORY_SIZE & 254);

    MY_HEAP[MEMORY_SIZE-2] = (uint8_t)(MEMORY_SIZE >> 8);
    MY_HEAP[MEMORY_SIZE-1] = (uint8_t)(MEMORY_SIZE & 254);
}

// revoie 1 si le bloc est libre, 0 sinon
int is_free(uint16_t address) {
    return (MY_HEAP[address-1] & 1) == 0;
}

// revoie la taille d'un bloc alloué ou non (méta-données comrpises)
uint16_t get_size(uint16_t address) {
    return (MY_HEAP[address-2] << 8) + (MY_HEAP[address-1] & 254);
}

uint16_t search_for_free_block(size_t size, uint16_t start_address) {
    if (start_address > MEMORY_SIZE) return NO_SPACE_FOUND;

    if (!is_free(start_address)){
        return search_for_free_block(size, start_address + get_size(start_address));
    }
    uint16_t current_size = get_size(start_address);

    uint16_t next_address = start_address;
    while (current_size-4 < size) {
        next_address += current_size;
        if (!is_free(next_address)) return search_for_free_block(size, next_address);

        current_size += get_size(next_address) - 4;
    }

    return start_address;
}

void *my_malloc(size_t size) {
    // si la taille est impaire, ajouter 1 pour la rendre paire
    size += size % 2;
    uint16_t address = search_for_free_block(size, 2);
    //printf("%d\n", address);
    if (address == NO_SPACE_FOUND) return NULL;

    uint16_t initial_size = get_size(address);

    // méta-données de début
    MY_HEAP[address-2] = (uint8_t)((size+4) >> 8);
    MY_HEAP[address-1] = (uint8_t)((size+4) & 254)+1;
    // méta-données de fin
    MY_HEAP[address+size] = (uint8_t)((size+4) >> 8);
    MY_HEAP[address+size+1] = (uint8_t)((size+4) & 254)+1;

    // méta-données de début du reste
    MY_HEAP[address+size+2] = (uint8_t)((initial_size-size-4) >> 8);
    MY_HEAP[address+size+3] = (uint8_t)((initial_size-size-4) & 254);
    // méta-données de fin du reste
    MY_HEAP[address+initial_size-4] = (uint8_t)((initial_size-size-4) >> 8);
    MY_HEAP[address+initial_size-3] = (uint8_t)((initial_size-size-4) & 254);

    return (void*)(long)address;
}

void free(void *pointer) {

}

void print_memory() {
    printf("|");
    for (int i = 0; i < MEMORY_SIZE; i++)
        printf(" %d |", MY_HEAP[i]);
    printf("\n\n");
}

int main(int argc, char **argv) {
    init();
    my_malloc(10);
    my_malloc(300);
    my_malloc(300);
    // print_memory();

    return 0;
}
