#include<stdio.h>
#include<stdlib.h>

//*********************** Link List Definitions **************************
// Definition of a link in a linked list
struct link {
    int value;
    struct link* next;
};

/**
 * create_link()
 * Allocates a single link and gives it a value
 * 
 * input:
 *   int value: value for this link to contain
 * returns:
 *   pointer to new link
 **/
struct link* create_link(int new_value) {
    struct link* value = malloc(sizeof(struct link));
    (*value).value = new_value;
    (*value).next = NULL;
}

/**
 * append_value()
 * adds a value ontop the end of a current linked list
 *
 * input:
 *   struct link* node: pointer to a node in the list
 *   int value: value to append onto the end
 * returns:
 *   pointer to first node in list
 **/
struct link* append_value(struct link* node, int value) {
    struct link* start = node;
    if(node) {
        while((*node).next) {
            node = (*node).next;
        }
        (*node).next = create_link(value);
        return start;
    } else {
        return create_link(value);
    }
}

/**
 * iterate()
 * iterates through a linked list
 *
 * input:
 *   struct link** node: a pointer to the start/current node passed by reference
 *   int* value: a pointer to the value of the current node
 * return:
 *   int 1 if successfully retrieved values, 0 if no more values
 **/
int iterate(struct link** node, int* value) {
    if((**node).next) {
        *node = (**node).next;
        *value = (**node).value;
        return 1;
    } else {
        return 0;
    }
}

/**
 * kill_list()
 * kills a list and frees the memory
 *
 * input:
 *  struct link* node
 **/
void kill_list(struct link* node) {
    if((*node).next)
        kill_list((*node).next);
    free(node);
}
    

//**************************** IO Functions ******************************
/**
 * safely_get_char
 * input:
 *   char* request: string to print for user before scanning for char
 *   char* value: pointer to location to return value
 *   int retries: number of tries for user to enter char before failing
 * returns:
 *   int: 0 if fails to gets character, 1 if successful
 *
 * about:
 *   safely gets a character from the user, clears the buffer
 *   there is no way that there will be a failure in getting a char
 *   but whatever
 **/
int safely_get_char(char* request, char* value, int retries) {
    // gets an int from scanf safely (checking bounds && input)
    do {
        int ch;
        // get a valid value
        char buffer[255];
        printf("%s", request);
        if(scanf("%s", buffer)) {
            *value = buffer[0];
            while ((ch = getchar()) != '\n' && ch != EOF)
                continue;
            return 1;
        }
        // if we reach here we did not pass valid condition test
        --retries;
        if(retries) {
            //print error only if more retries. Nested if for form of try
            printf("[Error] Please enter a valid character. (%d more %s)\n", retries, retries > 1 ? "tries" : "try");
        }
        while ((ch = getchar()) != '\n' && ch != EOF)
            continue;

    } while(retries > 0);
    //we got here so we never got a good value
    return 0;
}

/**
 * safely_get_int
 * input:
 *   char* request: string to print for	user before scanning for integer
 *   int lower_bound: low bound for valid integer value (inclusive)
 *   int higher_bound: high bound for valid integer value (exclusive)
 *   int* value: pointer to location to return	value
 *   int retries: number of tries for user to enter valid integer before failing
 * returns:
 *   int: 0 if fails to gets character, 1 if successful
 *
 * about:
 *   safely gets an integer from a user, checks it is within bound. If bounds
 *   are the same then it does not check bounds and simply gets any integer
 *   value.
 **/
int safely_get_int(char* request, int lower_bound, int higher_bound, int* value, int retries) {
    int ch;

    // gets an int from scanf safely (checking bounds && input)
    do {
        // get a valid value
        printf("%s", request);
        if(scanf("%d", value)) {
            // if bounds are the same assume no bound
            if(lower_bound == higher_bound) {
                //clear buffer
                while ((ch = getchar()) != '\n' && ch != EOF)
                    continue;
                return 1; // return
            } else {
                //check bounds
                if(*value >= lower_bound && *value < higher_bound) {
                    // clear buffer
                    while ((ch = getchar()) != '\n' && ch != EOF)
                        continue;
                    return 1; // return if valid
                }
            }
        }
        // if we reach here we did not pass valid condition test
        --retries;
        if(retries) {
            //print error only if more retries. Nested if for form of try
            printf("[Error] Please enter an integer from %d to %d inclusive. (%d more %s)\n", lower_bound, higher_bound - 1, retries, retries > 1 ? "tries" : "try");

        }
        // clear buffer for next input
        while ((ch = getchar()) != '\n' && ch != EOF)
            continue;

    } while(retries > 0);
    //we got here so we never got a good value
    return 0;
}

//**************************** Main Program Logic **********************************
int main(int arg, char** argv) {
    char continue_char;

    do {
        //init some vars
        struct link* list = NULL;
        int value = 0;
        int count = 0;

        do {
            if(!safely_get_int("", 0, 0, &value, 3)) {
                printf("Unable to get valid integer. Exitting.\n");
                return 0;
            }
            // could do an infite while and break if == -1
            // but the do ... while makes the break condition more apparent
            if(value != -1) {
                list = append_value(list, value);
                ++count;
            }
        } while(value != -1);

        // save us from stupid stuff if no values
        if(count == 0) {
            printf("No entries! Exitting.\n");
            return 0;
        }

        // iterate through values and get the sum
        int sum = (*list).value;
        while(iterate(&list, &value)) {
            sum += value;
        }

        // print the average
        printf("Average is %d\n", sum / count);

        // kill the list to prevent a memory leak
        kill_list(list);

        if(!safely_get_char("Do you want to enter another set of grade? (y/n): ", &continue_char, 3)) {
            printf("Failed to get char... Exitting");
            return 0;
        }
    } while(continue_char == 'Y' || continue_char == 'y');
    return 1;
}
