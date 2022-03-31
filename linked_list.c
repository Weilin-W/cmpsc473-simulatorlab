#include <stdlib.h>
#include "linked_list.h"

// Creates and returns a new list
// If compare is NULL, list_insert just inserts at the head
list_t* list_create(compare_fn compare)
{
    /* IMPLEMENT THIS */
    list_t* linked_list = (list_t*)malloc(sizeof(list_t));
    linked_list->count = 0;
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->compare = compare;
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
        list->count += 1;
        return new_node;
    }
    //Compare function
    if(list->compare == NULL){
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
        list->count += 1;
        return new_node;
    }
    while(temp != NULL){
        if(list->compare(temp->data,data) == -1){
            temp = temp->next;
        }else if(list->compare(temp->data,data) == 0 || list->compare(temp->data,data) == 1){
            //Head
            if(temp == list->head){
                new_node->next = list->head;
                list->head->prev = new_node;
                list->head = new_node;
                list->count += 1;
                return new_node;
            //Middle
            }else{
                new_node->next = temp;
                temp->prev->next = new_node;
                new_node->prev = temp->prev;
                temp->prev = new_node;
                list->count += 1;
                return new_node;
            }
        }
    }
    if(temp == NULL){
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
        list->count += 1;
        return new_node;
    }
    return new_node;
}

// Removes a node from the list and frees the node resources
void list_remove(list_t* list, list_node_t* node)
{
    /* IMPLEMENT THIS */
    list_node_t* temp = list->head;
    //
    while(temp != NULL){
        if(temp == node){
            if(temp == list->head){
                list->head = temp->next;
                list->count -= 1;
                free(node);
                return;
            }else if(temp == list->tail){
                temp->prev = list->tail;
                list->count -= 1;
                free(node);
                return;
            }else if(temp->next != NULL && temp->prev != NULL){
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                list->count -= 1;
                free(node);
                return;
            }
        }
        temp = temp->next;
    }
}
