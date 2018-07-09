# COE 449 Lab 3: malloc and free

## Description
In this lab, you’ll write a pretty simple data structure: a linked list (if you haven’t done a data structures
course, ask your TA or fellow students about it!). You’ll have to use malloc and free to make it!

Here is the Node struct that you will be using. Type it in exactly as you see it here – yes, we put the
name of the struct twice, because to make the struct contain a pointer to another Node, we have to use
the struct Node syntax. It’s C, what do you want?

```c
typedef struct Node
{
 struct Node* next;
 int value;
} Node;
```

### The create_node function
First you will write a function with the following signature:

```c
Node* create_node(int value);
```

It will do the following:
	1. Use malloc to allocate enough memory to hold one Node, and store the pointer to the allocated memory in a Node* variable.
	2. Since malloc gives you memory which may be full of garbage, you are responsible for filling in the Node’s fields with valid values. Set its value member to the value that was passed into this function, and its next member to NULL.
	3. Return the Node*.

To test it, try writing this snippet in main. It should print value = 500, next = (nil):
 ```c
Node* new_node = create_node(500);
printf("value = %d, next = %p\n", new_node->value, new_node->next);
```

### The print_list function
Now you will write a function with the following signature:

```c
void print_list(Node* n);
```

It will print out the values of the linked list, one on each line, until it gets to the end of the linked list.

To test it, try writing this snippet in main. It should print 10, 20, and 30 on separate lines:

```c
Node* a = create_node(10);
Node* b = create_node(20);
Node* c = create_node(30);
a->next = b;
b->next = c;
print_list(a);
```

### The free_list function
Every malloc should be matched with a free, right? Right! So now you need to make a function that
cleans up the memory allocated for the nodes. It should have the following signature:

```c
void free_list(Node* n);
```

It should go through the list, calling free on each of the nodes in the list. It should also do nothing if
the parameter is NULL. But remember the other important rule of the heap: you can’t access memory
that has been freed. So your loop will have to be a little different than before.

Make sure that you actually free all the nodes! Try adding free_list(a); to the end of the snippet above,
and put a printf in your free_list function right before your free to print the value of the node that is
about to be freed. Once you’re sure that you’re freeing all the nodes, remove the printf.

### The driver program
In main, make a simple program that does the following:

1. Make a Node* variable that points to NULL.
2. Loop:
	* Read an integer from the user. (Remember how to do that from lab 2?)
	* If the number is -1, break.
	* Otherwise, add a new node with that value to the beginning of the linked list pointed to by your variable from step 1.
3. Print the list. (It should print the numbers in opposite order).
4. Free the list.

To test this, you could run it and type the numbers in over and over… but there’s an easier way. Just
like you’ve been redirecting the output to a file for your labs with the > character, you can redirect
the input from a file with the < character. For example, if I wrote this:

./lab3 < test_input.txt

And test_input.txt contained this:

10
20
30
-1

Then my lab3 program would behave just like I typed in those 4 lines myself, and it would print 30, 20,
10 as the output. (The input prompts would also show up and look a little weird, but that’s fine.)
