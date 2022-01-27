#include <iostream>
#include "src/headers/List.hpp"
#include <cstring>

#define NUM_OF_KEY 10
#define SIZE 5

struct test_type{

    char arr[SIZE] = {"1234"};
};

int main(int, char**) {
    
    Memory new_container;
    List<int> my_list(&new_container);
    
    for (int i = 0; i < NUM_OF_KEY; i++){

        my_list.push_back(i);
    }

    my_list.dump_graphviz("../bin/output.dot");
}
