/*
 * Created by ThankfulBird on 02.11.2018.
 * linked_list.h library created for illustration of linked list implementation in C language.
 * Some advanced functions shall be added soon.
 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

// Library relies on three fundamental data types:
// 1-Boolean as a mimic of boolean data type in other language
// 2-Node as a crucial and substansial element of linked list above which it built on
// 3-NodePtr as a secondary data type
typedef enum Boolean Boolean;
typedef struct Node Node;
typedef Node * NodePtr;

enum Boolean {
    FALSE, TRUE
};

// Node struct consists of 2 variables : 1-value (numeric) 2-Address of the next node
struct Node {
    int value;
    NodePtr next_ptr;
};

// Function declarations
Boolean is_empty(NodePtr node_header);
void insert_node(NodePtr *node_header_ptr, int value);
void remove_node(NodePtr *node_header_ptr, int value);
void print_linked_list(NodePtr node_header);

// A function to control whether the given linked list is empty or not
Boolean is_empty(NodePtr node_header) {
    return (node_header == NULL) ? TRUE : FALSE;
}

// Function for insertion of the node takes 2 arguments 1-Pointer of linked list header(*) 2-Numeric value
// * The reason we use header pointer here is to prevent data loss when add a number which is less than head itself
// In library after every malloc if control used to check whether the address is allocated successfully or not with a very delicate else choice.
void insert_node(NodePtr *node_header_ptr, int value) {
    NodePtr new_node_ptr = (NodePtr)malloc(sizeof(Node));
    if (new_node_ptr != NULL) {
        new_node_ptr->value = value;
        new_node_ptr->next_ptr = NULL;
        NodePtr current_node_ptr = *node_header_ptr;
        NodePtr previous_node_ptr = NULL;
        if (is_empty(*node_header_ptr)) { // If header is empty just make the header new node pointer
            *node_header_ptr = new_node_ptr;
            return;
        } else if (value < (*node_header_ptr)->value) { // If value is greater than the header value shift header left
            new_node_ptr->next_ptr = *node_header_ptr;
            *node_header_ptr = new_node_ptr;
            return;
        }
        while (current_node_ptr != NULL) { // Traverse throughout the linked list and find an appropriate place for value
            if (value < current_node_ptr->value) { // If you find bind the value to the linked list 
                previous_node_ptr->next_ptr = new_node_ptr;
                new_node_ptr->next_ptr = current_node_ptr;
                return;
            }
            previous_node_ptr = current_node_ptr;
            current_node_ptr = current_node_ptr->next_ptr;
        }
        previous_node_ptr->next_ptr = new_node_ptr;
    } else {
        insert_node(node_header_ptr, value);
    }
}

// Function for removal of a node
void remove_node(NodePtr* node_header_ptr, int value) {
    NodePtr current_node_ptr = *node_header_ptr;
    NodePtr previous_node_ptr = NULL;
    if (is_empty(*node_header_ptr)) { // If linked list is empty then exit
        puts("Linked list is empty. Removal denied.");
        return;
    }
    else if (value == (*node_header_ptr)->value) { // If value is equal to header value than shift header right then free the memory
        *node_header_ptr = (*node_header_ptr)->next_ptr;
        free(current_node_ptr);
        return;
    }
    while (current_node_ptr != NULL) { // Traverse throughout the linked list to find the value
        if (value == current_node_ptr->value) { // If you find it then make the current node NULL and then free it
            previous_node_ptr->next_ptr = current_node_ptr->next_ptr;
            free(current_node_ptr);
            return;
        }
        previous_node_ptr = current_node_ptr;
        current_node_ptr = current_node_ptr->next_ptr;
    }
    puts("Key is not found."); // If none of above commands are executed then the value is not included
}

// A simple function to print values in linked list traversing it
void print_linked_list(NodePtr node_header) {
    NodePtr current_node_ptr = node_header;
    if (is_empty(node_header)) {
        puts("Linked list is empty. Cannot display.");
        return;
    }
    printf("%s", "●");
    while(current_node_ptr != NULL) {
        if (current_node_ptr->next_ptr != NULL) {
            printf("[%d]->", current_node_ptr->value);
        } else {
            printf("[%d]~\n", current_node_ptr->value);
        }
        current_node_ptr = current_node_ptr->next_ptr;
    }
}

#endif //LINKED_LIST_H
