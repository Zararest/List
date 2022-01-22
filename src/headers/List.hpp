#pragma once

#include "Memory.h"

template <typename T>
class List{

public:

    struct Node{

        MemoryKey key;
        Node* next = nullptr;
        Node* prev = nullptr;
    };

    struct Iterator{

        Iterator(const Node* new_node){ cur_node = new_node; }
        Iterator(const Iterator& old_iter){ cur_node = old_iter.cur_node; }
        Iterator(Iterator&& rv_iter){ cur_node = rv_iter.cur_node; }
        ~Iterator() = default;

        Iterator& operator =(const Iterator old_iter){ cur_node = old_iter.cur_node; }
        Iterator& operator =(Iterator&& rv_iter){ cur_node = rv_iter.cur_node; }

        T& operator*() const { return cur_node->value; }

        Iterator& operator++() { cur_node = cur_node->next; return *this; }  
        Iterator operator++(int) { Iterator tmp = *this; cur_node = cur_node->next; return tmp; }

        friend bool operator ==(const Iterator& a, const Iterator& b) { return a.cur_node == b.cur_node; };
        friend bool operator !=(const Iterator& a, const Iterator& b) { return a.cur_node != b.cur_node; };  

    private:

        Node* cur_node = nullptr;
    };

    List(Memory* new_container);
    List(const List& old_list);
    List(List&& rv_list);
    ~List();

    List& operator =(const List& old_list) = delete;
    List& operator =(List&& rv_list) = delete;

    Iterator begin(){ return Iterator(root); }
    Iterator end(){

        Node* tmp = root;
        while (tmp->next != nullptr) tmp = tmp->next;
        return tmp;
    }

    void push_back(T new_value);
    void push_front(T new_value);
    T pop_back();
    T pop_front();

private:

    Memory* memory_container;
    Node* root = nullptr;    
};

template <typename T>
List<T>::List(Memory* new_container){

    memory_container = new_container;
}

template <typename T>
List<T>::List(const List<T>& old_list){

    memory_container = old_list.memory_container;
    Node* old_node = old_list.root;
    Node* new_node = nullptr;

    if (old_node == nullptr){ return; }

    new_node = new Node;

    new_node->key = memory_container->get_memory(sizeof(T));
    memcpy(memory_container->get_ptr(new_node->key), memory_container->get_ptr(old_node->key), sizeof(T));

    root = new_node;

    while (old_node->next != nullptr){
        
        new_node->next = new Node;
        new_node->next->prev = new_node;

        new_node = new_node->next;
        old_node = old_node->next;

        new_node->key = memory_container->get_memory(sizeof(T));
        memcpy(memory_container->get_ptr(new_node->key), memory_container->get_ptr(old_node->key), sizeof(T));
    }
}

template <typename T>
List<T>::List(List&& rv_list){

    std::swap(memory_container, rv_list.memory_container);
    std::swap(root, rv_list.root);
}

template <typename T>
List<T>::~List(){

    Node* prev_node = root;
    Node* cur_node = nullptr;

    if (root != nullptr){ 

        cur_node = root->next;
    }

    while (cur_node != nullptr){

        memory_container->delete_ptr(prev_node->key);
        delete prev_node;
        prev_node = cur_node;
        cur_node = cur_node->next;
    }

    if (prev_node != nullptr){ memory_container->delete_ptr(prev_node->key); }
    delete prev_node;
}

template <typename T>
void List<T>::push_back(T new_value){

    Node* cur_node = root;

    if (root == nullptr){ 

        root = new Node;
        root->key = memory_container->get_memory(sizeof(T));
        *((T*)memory_container->get_ptr(root->key)) = new_value; 

        return;
    }

    while (cur_node->next != nullptr){

        cur_node = cur_node->next;
    }

    cur_node->next = new Node;
    cur_node = cur_node->next;
    cur_node->key = memory_container->get_memory(sizeof(T));
    *((T*)memory_container->get_ptr(cur_node->key)) = new_value; 
}

template <typename T>
void List<T>::push_front(T new_value){

    Node* new_root = new Node;
    new_root->next = root;
    new_root->key = memory_container->get_memory(sizeof(T));
    *((T*)memory_container->get_ptr(new_root->key)) = new_value; 

    root = new_root;
}

template <typename T>
T List<T>::pop_back(){

    Node* cur_node = root;
    T* ret_val = nullptr;;

    if (root == nullptr){

        fprintf(stderr, "List is empty\n");
        exit(0);
    }

    while (cur_node->next != nullptr){

        cur_node = cur_node->next;
    }

    if (cur_node->prev != nullptr){ cur_node->prev->next = nullptr; }

    ret_val = (T*)memory_container->get_ptr(cur_node->key);
    memory_container->delete_ptr(cur_node->key);
    delete cur_node;

    return *ret_val;
}

template <typename T>
T List<T>::pop_front(){

    Node* cur_node = root;
    T* ret_val = nullptr;

    if (root == nullptr){

        fprintf(stderr, "List is empty\n");
        exit(0);
    }

    if (cur_node->next != nullptr){

        cur_node->next->prev = nullptr;
    }

    ret_val = (T*)memory_container->get_ptr(root);
    root = cur_node->next;
    memory_container->delete_ptr(cur_node->key);
    delete cur_node;

    return *ret_val;
}
