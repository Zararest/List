#pragma once

#define MAX_MEMORY_SIZE 100

typedef int MemoryKey;

struct Simple_list{

    int begin_of_free_space = 0;
    int size_of_free_space = 0;
    Simple_list* next = nullptr;
};

class Memory{

    char* memory_line = nullptr;
    int capacity = 0;
    int memory_size = 0;

    Simple_list* free_space = nullptr;

public: 

    Memory();
    Memory(const Memory& old_memory);
    Memory(Memory&& rv_memory);
    ~Memory();

    Memory& operator =(const Memory& old_memory);
    Memory& operator =(Memory&& rv_memory);

    MemoryKey get_memory(int size);
    void* get_ptr(MemoryKey key);
    void delete_ptr(MemoryKey key);
    void dump_free_space();
    void dump_memory();
};