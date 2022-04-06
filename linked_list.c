#include <stdlib.h>
#include "linked_list.h"
#include <stdio.h>
// Creates and returns a new list
// If compare is NULL, list_insert just inserts at the head
list_t* list_create(compare_fn compare)
{
    /* IMPLEMENT THIS */
    list_t* linked_list = (list_t*)malloc(sizeof(list_t));
    if(compare == NULL){
        linked_list->compare = NULL;
    }else{
        linked_list->compare = compare;
    }
    linked_list->count = 0;
    linked_list->head = NULL;
    linked_list->tail = NULL;
    return linked_list;
}

// Destroys a list
void list_destroy(list_t* list)
{
    /* IMPLEMENT THIS */
    int index = 0;
    while(index < list->count){
        list_node_t* temp = list->head->next;
        free(list->head);
        list->head = temp;
        index++;
    }
    list->count = 0;
    //At last node, sets current node to NULL and free current node
    free(list);
}

// Returns head of the list
list_node_t* list_head(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->head;
}

// Returns tail of the list
list_node_t* list_tail(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->tail;
}

// Returns next element in the list
list_node_t* list_next(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->next;
}

// Returns prev element in the list
list_node_t* list_prev(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->prev;
}

// Returns end of the list marker
list_node_t* list_end(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->tail;
}

// Returns data in the given list node
void* list_data(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->data;
}

// Returns the number of elements in the list
size_t list_count(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->count;
}

// Finds the first node in the list with the given data
// Returns NULL if data could not be found
list_node_t* list_find(list_t* list, void* data)
{
    /* IMPLEMENT THIS */
    list_node_t* temp = list->head;
    while(temp != NULL){
        if(temp->data == data){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Inserts a new node in the list with the given data
// Returns new node inserted
list_node_t* list_insert(list_t* list, void* data)
{
    /* IMPLEMENT THIS */
    list_node_t* new_node = (list_node_t*)malloc(sizeof(list_node_t));
    //current node
    list_node_t* temp = list->head;
    
    //Getting new node setup
    new_node->data = data;
    //Deal with empty list
    if(list->count == 0){
        new_node->next = NULL;
        new_node->prev = NULL;
        list->head = new_node;
        list->tail = new_node;
        list->count = list->count + 1;
        return new_node;
    }
    //Compare function
    if(list->compare == NULL){
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
        list->head->prev = NULL;
        list->count = list->count + 1;
        return new_node;
    }
    while(temp != NULL){
        if(list->compare(temp->data,data) == -1){
            temp = temp->next;
        }else if(list->compare(temp->data,data) == 0 || list->compare(temp->data,data) == 1){
            //At the beginning
            if(temp == list->head){
                new_node->next = list->head;
                list->head->prev = new_node;
                list->head = new_node;
                list->head->prev = NULL;
                list->count = list->count + 1;
                return new_node;
            //Middle
            }else{
                new_node->next = temp;
                temp->prev->next = new_node;
                new_node->prev = temp->prev;
                temp->prev = new_node;
                list->count = list->count + 1;
                return new_node;
            }
        }
    }
    //Insert at the end
    if(temp == NULL){
        new_node->prev = list->tail;
        list->tail->next = new_node;
        new_node->next = NULL;
        list->tail = new_node;
        list->count = list->count + 1;
        return new_node;
    }
    return new_node;
}

// Removes a node from the list and frees the node resources
void list_remove(list_t* list, list_node_t* node)
{
    /* IMPLEMENT THIS */
    //Remove only node
    if(list->count == 1){
        list->head = NULL;
        list->tail = NULL;
    }
    //Remove head
    else if(node == list->head){
        node->next->prev = NULL;
        list->head = node->next;
    //Remove tail
    }else if(node == list->tail){
        node->prev->next = NULL;
        list->tail = node->prev;
    //Remove in-between nodes
    }else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    //Decrement and free
    list->count = list->count - 1;
    free(node);
}
