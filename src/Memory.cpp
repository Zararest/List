#include "headers/Memory.h"

#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>

Simple_list* copy_simple_list(Simple_list* root){

    Simple_list* new_list = new Simple_list;
    Simple_list* old_node = root;
    Simple_list* new_node = new_list;

    while (old_node->next != nullptr){

        new_node->begin_of_free_space = old_node->begin_of_free_space;
        new_node->size_of_free_space = old_node->size_of_free_space;
        new_node->next = new Simple_list; 

        old_node = old_node->next;
        new_node = new_node->next;
    }

    new_node->begin_of_free_space = old_node->begin_of_free_space;
    new_node->size_of_free_space = old_node->size_of_free_space;

    return new_list;
}

void delete_simple_list(Simple_list* root){

    Simple_list* cur_node = root;
    Simple_list* node_to_delete = nullptr;

    while (node_to_delete != root){

        node_to_delete = root;

        while (cur_node->next != nullptr){

            if (cur_node->next->next == nullptr){

                node_to_delete = cur_node->next;
                cur_node->next = nullptr;
            } else{

                cur_node = cur_node->next;
            }
        }

        delete node_to_delete;
    }
}

void check_empty_nodes(Simple_list* root){

    Simple_list* cur_node = root;
    Simple_list* node_to_delete = nullptr;

    if (root == nullptr){ return; }

    while (cur_node->next != nullptr){

        if (cur_node->next->size_of_free_space == 0){

            node_to_delete = cur_node->next;
            cur_node->next = cur_node->next->next;
            delete node_to_delete;
        }

        cur_node = cur_node->next;
    }
}

Memory::Memory(){

    free_space = new Simple_list;
    free_space->begin_of_free_space = 0;
    free_space->size_of_free_space = MAX_MEMORY_SIZE;
}

Memory::Memory(const Memory& old_memory){

    capacity = old_memory.capacity;
    memory_line = new char[capacity];
    memcpy(memory_line, old_memory.memory_line, capacity);

    free_space = copy_simple_list(old_memory.free_space);
}

Memory::Memory(Memory&& rv_memory){

    std::swap(memory_line, rv_memory.memory_line);
    std::swap(free_space, rv_memory.free_space);
    capacity = rv_memory.capacity;
}

Memory::~Memory(){

    delete_simple_list(free_space);
    delete[] memory_line;
}

Memory& Memory::operator =(const Memory& old_memory){

    if (this == &old_memory){ return *this; }

    capacity = old_memory.capacity;
    delete[] memory_line;
    memory_line = new char[capacity];
    memcpy(memory_line, old_memory.memory_line, capacity);

    delete_simple_list(free_space);
    free_space = copy_simple_list(old_memory.free_space);

    return *this;
}

Memory& Memory::operator =(Memory&& rv_memory){

    if (this == &rv_memory){ return *this; }

    std::swap(memory_line, rv_memory.memory_line);
    std::swap(free_space, rv_memory.free_space);
    capacity = rv_memory.capacity;

    return *this;
}

MemoryKey generate_key(int pos, int size){

    return size + pos * 100;
}

MemoryKey Memory::get_memory(int size){

    if (size >= MAX_MEMORY_SIZE){ return -1; }

    Simple_list* cur_free_memory = free_space;
    int new_pos = 0, old_capacity = capacity;

    while (cur_free_memory->size_of_free_space < size){

        cur_free_memory = cur_free_memory->next;
        assert(cur_free_memory != nullptr);
    }

    new_pos = cur_free_memory->begin_of_free_space;

    if (new_pos == memory_size){ //позиции начинаются с 1

        memory_size += size;
        cur_free_memory->begin_of_free_space = memory_size;
    } else{

        cur_free_memory->size_of_free_space -= size;
    }
    
    check_empty_nodes(cur_free_memory);

    while (memory_size > capacity){

        if (capacity == 0){

            capacity = 4;
        }

        capacity *= 2;
    }

    char* new_memory_line = new char[capacity];
    memcpy(new_memory_line, memory_line, old_capacity);
    delete[] memory_line;
    memory_line = new_memory_line;

    return generate_key(new_pos, size);
}

void* Memory::get_ptr(MemoryKey key){

    return memory_line + (key / 100);
}

void Memory::delete_ptr(MemoryKey key){

    Simple_list* prev_node = nullptr;
    Simple_list* cur_node = free_space;
    int cur_memory_pos = key / 100, cur_memory_size = key % 100;  

    while (cur_node->begin_of_free_space < cur_memory_pos){

        prev_node = cur_node;
        cur_node = cur_node->next;
        assert(cur_node != nullptr);
    }

    cur_node = new Simple_list;
    cur_node->begin_of_free_space = cur_memory_pos;
    cur_node->size_of_free_space = cur_memory_size;

    if (prev_node == nullptr){
        
        cur_node->next = free_space;
        free_space = cur_node;
    } else{

        cur_node->next = prev_node->next;
        prev_node->next = cur_node;
    }
}

void Memory::dump_free_space(){

    Simple_list* cur_node = free_space;

    while (cur_node != nullptr){

        printf("[pos: %i, sz: %i]-->", cur_node->begin_of_free_space, cur_node->size_of_free_space);
        cur_node = cur_node->next;
    }

    printf("null\n");
}

void Memory::dump_memory(){

    printf("memory: |");

    for (int i = 0; i < memory_size; i++){
        
        putc(memory_line[i], stdout);
    }

    printf("|\n");
}

