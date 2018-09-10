//
//  sorting.cpp
//  assignment01
//
//  Created by Joon on 2018/09/09.
//  Copyright © 2018 penubo. All rights reserved.
//

#include "sorting.hpp"

/*
 * Helper function
 *
 * check weather the given indicies are valid values for the list.
 *
 * @exception std::invalid_argument
 * if index_to (exclusive) is bigger than size of the list or
 * index_from (inclusive) is smaller than 0 or bigger than index_to,
 * throw exception.
 *
 */
void check_range(LinkedList* list, int index_from, int index_to) {
    if(list_cnt(list) < index_to) {
        throw std::invalid_argument("index_to is overrange");
    } else if (index_from > index_to || index_from < 0) {
        throw std::invalid_argument("index_from is overrange");
    }
}

/*
 * exchange sort
 *
 * exchange sort is executed by comparing between one pivot (from left) with last nodes.
 * for example below is the very first iteration of exchange sort.
 *
 *  O   O   O   O   O   O   O   O   O   O           (p) : pivot
 * (p) (c)
 *  O   O   O   O   O   O   O   O   O   O           (c) : current
 * (p)     (c)
 *  O   O   O   O   O   O   O   O   O   O
 * (p)          . . . . . . . . . . . .(c)
 *
 * if current is smaller than pivot during this iteration exchange pivot to current.
 *
 * After each iteration, it decides the smallest node at position of the pivot and
 * move the pivot to the right. do iteration until the pivot get to the tail.
 *
 * exchange sort sorts the list in ascending order.
 *
 * {@code} node_pivoted
 * the pivot node starts from head of the list. After each iteration, pivot will be
 * the smallest node among the list except nodes already decided by previous iteration.
 *
 * {@code} node_current
 * the current node is the very node to be examined to find smaller node than pivot.
 *
 */

void exchange_sort(LinkedList* list) {
    
    Node* node_pivoted = list -> head;
    Node* node_current = node_pivoted -> next;
    
    for (; node_pivoted != list -> tail; node_pivoted = node_pivoted -> next) {
        for (node_current = node_pivoted -> next; node_current != NULL; node_current = node_current -> next) {
            if (node_current -> data < node_pivoted -> data) {
                list_exchange_data(node_current, node_pivoted);
            }
        }
    }
}

/*
 * Helper function for merge_sort
 *
 * merge two lists, which is supposed to be sorted already, to an one list.
 *
 * first of all, make copies of two pre-sorted lists for easy modification.
 * Each comparation of nodes in two lists, set the smaller node in original list.
 * If the node, weather left or right, gets to the end of copied list, set nodes in
 * left side to original list. Note here we don't need to care for nodes in the right
 * since they are already in right position no matter we compared them or not. We have
 * to care, However, nodes in left because positions doesn't fit into original list.
 *
 * Note that {@code} index_from is inclusive whereas {@code} index_to is exclusive.
 *
 * {@code} node_left
 * reference of a node in the left list which already sorted and also copied.
 *
 * {@code} node_right
 * reference of a node in the right list which already sorted and also copied.
 *
 * {@code} node_current
 * reference of a node in original list which is being sorted. Basically, we rearrange every node
 * in original list from {@code} index_from to {@code} index_to.
 *
 */

void merge(LinkedList* list, int index_from, int index_to) {
    
    int n = index_to - index_from;          // size of the list which is to be merged.
    
    Node* node_left = list_copy(list, index_from, index_from + n / 2) -> head;
    Node* node_right = list_copy(list, index_from + n / 2, index_to) -> head;
    Node* node_current = list_search_index(list, index_from);
    
    while (node_left != NULL && node_right != NULL) {
        if (node_left -> data <= node_right -> data) {
            list_exchange_data(node_current, node_left);
            node_left = node_left -> next;
        } else {
            list_exchange_data(node_current, node_right);
            node_right = node_right -> next;
        }
        node_current = node_current -> next;
    }
    
    while (node_left != NULL) {
        list_exchange_data(node_current, node_left);
        node_left = node_left -> next;
        node_current = node_current -> next;
    }
    
}

/*
 * merge sort
 *
 * idea of merge sort is divide and conquer. Hence we first divide the list and
 * merge each pre-ordered lists into a whole sorted list using recursion technique.
 *
 * @exception std::invalid_argument
 * if index_from or index_to value is not valid throw exception.
 *
 * Note that {@code} index_from is inclusive whereas {@code} index_to is exclusive.
 *
 * stopping point of recursion is when {@code} n is smaller or equal to 1.
 *
 */

void merge_sort(LinkedList* list, int index_from, int index_to) {
    
    check_range(list, index_from, index_to); // check validation of indicies range.
    
    int n = index_to - index_from;          // size of the list.
    
    if (n <= 1) { return; }     // stopping point of recursion.
    
    // divide the list first.
    merge_sort(list, index_from, index_from + n / 2);
    merge_sort(list, index_from + n / 2, index_to);
    
    // merge after division.
    merge(list, index_from, index_to);
}

/*
 * overloading function of merge_sort above.
 */
void merge_sort(LinkedList* list) {
    merge_sort(list, 0, list_cnt(list));
}

/*
 * quick sort
 */
void quick_sort(LinkedList* list, int index_from, int index_to) {
    
    int n = index_to - index_from;

    if (n <= 1) { return; }
    
    Node* node_pivot = list_search_index(list, index_from);
    Node* node_left = node_pivot -> next;
    Node* node_right = list_search_index(list, index_to - 1);
    
    int index_left = index_from + 1;
    int index_right = index_to - 1;
    
    bool is_left_bigger = (node_left -> data > node_pivot -> data) ? true : false;
    bool is_right_smaller = (node_right -> data < node_pivot -> data) ? true : false;
    
    while (index_left <= index_right) {
        
        if (is_left_bigger && is_right_smaller) {
            list_exchange_data(node_left, node_right);
            is_left_bigger = false;
            is_right_smaller = false;
        }
        
        if (!is_left_bigger) {
            node_left = node_left -> next;
            index_left++;
            if (node_left != NULL && node_left -> data > node_pivot -> data) {
                is_left_bigger = true;
            }
        }
        
        if (!is_right_smaller) {
            index_right--;
            node_right = node_right -> prev;
            if (node_right != NULL && node_right -> data < node_pivot -> data) {
                is_right_smaller = true;
            }
        }
    }
    
    list_exchange_data(node_pivot, node_right);
    
    quick_sort(list, index_from, index_right);
    quick_sort(list, ++index_right, index_to);
}

void quick_sort(LinkedList* list) {
    quick_sort(list, 0, list_cnt(list));
}
