#define MEMORY_SIZE (uint16_t)30
#define NO_SPACE_FOUND MEMORY_SIZE+1

#include <stdio.h>
#include <stdint.h>


uint8_t MY_HEAP[MEMORY_SIZE];


// revoie 1 si le bloc est libre, 0 sinon
int is_free(uint16_t address) {
    return (MY_HEAP[address-1] & 1) == 0;
}

// cf is_free(), mais pour la fin du bloc
int is_free_end(uint16_t address) {
    return (MY_HEAP[address+2] & 1) == 0;
}

// revoie la taille d'un bloc alloué ou non (méta-données comprises)
uint16_t get_size(uint16_t address) {
    return (MY_HEAP[address-2] << 8) + (MY_HEAP[address-1] & 254);
}

// cf get_size, mais pour la fin du bloc
uint16_t get_size_end(uint16_t address) {
    return (MY_HEAP[address+1] << 8) + (MY_HEAP[address+2] & 254);
}

// met les meta-données de l'adresse donnée
void set_metadata(uint16_t address, uint16_t size, uint8_t allocated) {
    // méta-données de début
    MY_HEAP[address-2] = (uint8_t)(size >> 8);
    MY_HEAP[address-1] = (uint8_t)((size & 254)+allocated);
    // méta-données de fin
    MY_HEAP[address+size-4] = (uint8_t)(size >> 8);
    MY_HEAP[address+size-3] = (uint8_t)((size & 254)+allocated);
}

void set_metadata_end(uint16_t address, uint16_t size, uint8_t allocated) {
    // méta-données de la fin
    MY_HEAP[address+1] = (uint8_t)(size >> 8);
    MY_HEAP[address+2] = (uint8_t)((size & 254)+allocated);
    // méta-données du début
    MY_HEAP[address-size+3] = (uint8_t)(size >> 8);
    MY_HEAP[address-size+4] = (uint8_t)((size & 254)+allocated);
}

// cherche un bloc libre de taille acceptable récursivement
uint16_t search_for_free_block(size_t size, uint16_t start_address) {
    if (start_address >= MEMORY_SIZE) return NO_SPACE_FOUND;

    if (!is_free(start_address)){
        return search_for_free_block(size, start_address + get_size(start_address));
    }
    uint16_t current_size = get_size(start_address);
    uint16_t next_address;
    while (current_size-4 < size) {
        next_address = start_address + current_size;
        if (next_address > MEMORY_SIZE) return NO_SPACE_FOUND;
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
    //printf("address : %d\n", address);
    uint16_t initial_size = get_size(address);
    
    set_metadata(address, size+4, 1);

    if (initial_size-size >= 6) {
        set_metadata(address+size+4, initial_size-size-4, 0);
    }

    return (void*)(long)address;
}


void free(void *pointer) {
    //récupère l'adresse du pointeur
    uint16_t address = (uint16_t)(long)pointer;

    //gère les cas limites
    if (address >= MEMORY_SIZE || is_free(address)) {
        return;
    }

    //libère la mémoire
    uint16_t size = get_size(address);
    set_metadata(address, size, 0);

    //Fusionne les blocs postérieurs
    uint16_t next_address = address + size;
    //printf("next_address : %d\n", next_address);
    while (next_address < MEMORY_SIZE && is_free(next_address)) {
        uint16_t next_size = get_size(next_address);
        size += next_size;
        //printf("size : %d\n", size);
        set_metadata(address, size, 0);
        set_metadata(next_address, 0, 0);
        next_address += next_size;
    }

    //Fusionne les blocs antérieurs
    uint16_t size_back = get_size(address);
    //printf("size_back : %d\n", size_back);
    if (address >= 5){
        uint16_t prev_address = address -5;
        address = prev_address + size;
        //printf("prev_address : %d\n", prev_address);
        while (is_free_end(prev_address)) {
            uint16_t prev_size = get_size_end(prev_address);
            size_back += prev_size;
            set_metadata_end(address, size_back, 0);
            //printf("size_back : %d\n", size_back);
            set_metadata_end(prev_address, 0, 0);
            if (prev_size > prev_address){
                return;
            }
            prev_address -= prev_size;
        }
    }
}

// affiche l'entièreté de la mémoire
void print_memory() {
    printf("|");
    for (int i = 0; i < MEMORY_SIZE; i++)
        printf(" %d |", MY_HEAP[i]);
    printf("\n\n");
}

void init() {
    // remise à 0 pour éviter d'avoir des fausses méta-données
    for (int i = 0; i < MEMORY_SIZE; i++) {
        MY_HEAP[i] = 0;
    }
    // premier bloc = libre et fait la taille de toute le mémoire
    set_metadata(2, MEMORY_SIZE, 0);

    // MY_HEAP[0] = (uint8_t)(MEMORY_SIZE >> 8);
    // MY_HEAP[1] = (uint8_t)(MEMORY_SIZE & 254);

    // MY_HEAP[MEMORY_SIZE-2] = (uint8_t)(MEMORY_SIZE >> 8);
    // MY_HEAP[MEMORY_SIZE-1] = (uint8_t)(MEMORY_SIZE & 254);
}

int main(int argc, char **argv) {
    init();
    
    printf("Allocation 1\n");
    void *ptr1 = my_malloc(2);
    print_memory();
    
    printf("Allocation 2\n");
    void *ptr2 = my_malloc(4);
    print_memory();
    
    printf("Allocation 3\n");
    void *ptr3 = my_malloc(6);
    print_memory();
    
    printf("Libération de la mémoire (ptr3)\n");
    free(ptr3);
    print_memory();

    printf("Libération de la mémoire (ptr1)\n");
    free(ptr1);
    print_memory();

    printf("Libération de la mémoire (ptr2)\n");
    free(ptr2);
    print_memory();
    
    return 0;
}

