// Katie Pucci (kev19)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node
{
    struct Node* next;
    int value;
} Node;

Node* create_node(int value)
{
    Node* node = malloc(sizeof(Node));
    node->next = NULL;
    node->value = value;
    return node;
}

void print_list(Node* n)
{
    // Step through linked list
    for(; n != NULL; n = n->next)
        printf("%d\n", n->value);

}

void free_list(Node* n)
{
    Node* free_n;  // Holds ref to head node being freed while n maintains ref to next

    while(n != NULL)
    {
        free_n = n;
        n = n -> next;
        free(free_n);
    }
}

// Get input from user and store in buffer
void read_line(const char* message, char* buffer, int length)
{
    printf(message);
    fgets(buffer,length,stdin);
    if(strlen(buffer) != 0)
        buffer[strlen(buffer) - 1] = 0;
}

int main()
{
    Node* n = NULL;
    Node* head_ref;

    const int buff_size = 50;
    char input_buff[buff_size];
    const char* prompt = "Next node value (-1 to exit):\n";
    while(1)
    {
        read_line(prompt, input_buff, buff_size);
        int n_val;
        sscanf(input_buff, "%d", &n_val);

        if(n_val == -1)
            break;
        else
        {
            head_ref = n;
            n = create_node(n_val);
            n->next = head_ref;
        }
    }

    print_list(n);
    free_list(n);
    return 0;
}
