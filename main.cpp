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
    List<test_type> my_list(&new_container);
    
    test_type tmp = {"4321"};
    my_list.push_back(tmp);
    memcpy(tmp.arr, "0987", SIZE);
    my_list.push_back(tmp);
    memcpy(tmp.arr, "5678", SIZE);
    my_list.push_back(tmp);

    List<test_type>::Iterator my_iter = my_list.begin();

    printf("my iter = |%s|\n", (*my_iter).arr);
    my_iter++;
    printf("my iter = |%s|\n", (*my_iter).arr);
}
