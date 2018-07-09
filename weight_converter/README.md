# COE 449 Lab 2: console I/O

## Description
Today you’ll get some practice with reading from the console and printing things out.

Here’s some code to get you started. It contains a couple useful functions:

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
```

```c
// prints a message, reads a line of input, and chops off the newline.
// call it like
//    read_line("Type something in: ", buffer, sizeof(buffer));
void read_line(const char* message, char* buffer, int length)
{
    printf(message);
    fgets(buffer, length, stdin);
    if(strlen(buffer) != 0)
        buffer[strlen(buffer) - 1] = 0;
}
```

```c
// sees if two strings are the same, ignoring case.
// call it like
//    if(streq_nocase(one, two))
int streq_nocase(const char* a, const char* b)
{
    for(; *a && *b; a++, b++)
        if(tolower(*a) != tolower(*b))
            return 0;
    return (*a == 0 && *b == 0);
}
```

```c
int main()
{
    return 0;
}
```

Since I’m assuming you already know how to code a simple interactive console program, I won’t patronize you by explaining how that works. But how we read stuff from the console is a liiiiiiiittle weird.

I gave you the read_line function I showed in class. It’s just fgets with a couple extra features.

Look back through the slides/your notes to see how I used fgets previously. You’ll have to make a char array in main to hold the line. How long should it be? 100 is the typical “idk” size.

## Reading a Number
First we’ll read a number, the user’s weight (sorry if that’s sensitive info…).

1. Use the read_line function to ask the user for their weight.
2. Use sscanf to parse the number out of the string. You use it like this:

```c
char input[100];
// ... use fgets to read a line into 'input'
int weight;
sscanf(input, "%d", &weight); // DON'T FORGET THE &!! or it'll crash.
```

How this works is by handing off a pointer to the weight variable to sscanf. Then it can look for an integer (the "%d" tells it to do that), and put that value into weight indirectly.

Test it out, see if it works. (You can print out the weight to make sure, but take that print out before you continue.)

## Making a Function
Before main, make a function which returns a float and takes an int for the weight and a const char* for the name of the planet.

This function takes the name of a planet and a weight, and returns what you’d weigh on that planet, or -1 if it’s not a planet.

Here is a table of relative gravity strengths on the seven non-earth planets in our solar system:

| Planet	| Gravity |
| --------- | ------- |
| Mercury	|   0.38  |
| Venus		|   0.91  |
| Mars		|   0.38  |
| Jupiter	|   2.54  |
| Saturn	|   1.08  |
| Uranus	|   0.91  |
| Neptune	|   1.19  |

Use streq_nocase to check which planet it is, e.g.

```c
if(streq_nocase(planet, "mars")) {
    // it's mars.
}
```

The “case-insensitivity” of this function means they can type e.g. “mars”, “Mars”, “MARS” and it’ll all work the same way.

Remember, if the planet is not on this list, return -1.

## LoopyDoop
Make an infinite loop. (true is not a thing in C by default, so you can write while(1) to make an infinite loop.)

In that loop, you’ll be reading a line of input from the user and then using streq_nocase to see what they typed in. The “case-insensitivity” of this function means they can type e.g. “exit”, “Exit”, “EXIT” and it’ll all work the same way.

So, do this in the loop:
	1. Ask them what planet they want to visit.
	2. If they typed exit, use break; to exit the loop.
	3. If they typed earth, call them silly.
	4. Otherwise,
		* Call the function you made to get the scaled weight.
		* If it returned a value less than 0, that means it’s not a planet, so say so.
		* Otherwise, tell them how much they’d weigh there. ("%.2f" would be a nice format.)

Here’s how it looks when I interact with my program:

Uh, how much do you weigh? 250
What planet do you wanna go to ('exit' to exit)? mars
You'd weigh 95.00 there.
What planet do you wanna go to ('exit' to exit)? JUPITER
You'd weigh 635.00 there.
What planet do you wanna go to ('exit' to exit)? pluto
That's not a planet.
What planet do you wanna go to ('exit' to exit)? earth
uh, you're already there, buddy
What planet do you wanna go to ('exit' to exit)? exit
