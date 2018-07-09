// Katie Pucci (kev19)

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef enum RPS{ROCK = 1, PAPER = 2, SCISSORS = 3, INVALID} RPS;

const char* rps_string(RPS option)
{
    if(option == ROCK)
        return "rock";
    if(option == PAPER)
        return "paper";
    else if(option == SCISSORS)
        return "scissors";
    return "";
}

// Get input from user and store in buffer
void read_line(char* buffer, int length)
{
    fgets(buffer,length,stdin);
    if(strlen(buffer) != 0)
        buffer[strlen(buffer) - 1] = 0;
}

// Compare strings without worrying about case
int streq_nocase(const char* a, const char* b)
{
    for(; *a && *b; a++, b++)
        if(tolower(*a) != tolower(*b))
            return 0;
    return *a == 0 && *b == 0;
}

RPS get_human_opt(const char* option)
{
    // Compare human player's option
    if(streq_nocase(option,"rock"))
        return ROCK;
    if(streq_nocase(option,"paper"))
        return PAPER;
    if(streq_nocase(option,"scissors"))
        return SCISSORS;
    return INVALID;
}

RPS play_human(int num_round)
{
    // Create input buffer for human player's option
    const int buff_len = 50;
    char buff[buff_len];

    // Get human's option
    printf("Round %d! What's your choice? ", num_round);
    read_line(buff, buff_len);
    RPS human_opt = get_human_opt(buff);

    while(human_opt == INVALID)    // While human's input is invalid
    {
        printf("Huh? What's your choice? ");
        read_line(buff, buff_len);      // Get player input
        human_opt = get_human_opt(buff);   // Get player's option
    }

    return human_opt;
}

int rand_in_range(int low, int high)
{
    return rand() % (high - low + 1) + low;
}

RPS play_computer()
{
    int option = rand_in_range(1,3);

    // Compare human player's option
    if(option == 1)
        return ROCK;
    if(option == 2)
        return PAPER;
    if(option == 3)
        return SCISSORS;
    return INVALID;
}

int human_vs_comp(RPS human_opt, RPS comp_opt)
{
    if(human_opt == ROCK)
    {
        if(comp_opt == SCISSORS)
            return 1;
        else if(comp_opt == ROCK)
            return 0;
        else if(comp_opt == PAPER)
            return -1;
    }

    else if(human_opt == PAPER)
    {
        if(comp_opt == ROCK)
            return 1;
        else if(comp_opt == PAPER)
            return 0;
        else if(comp_opt == SCISSORS)
            return -1;
    }

    else if(human_opt == SCISSORS)
    {
        if(comp_opt == PAPER)
            return 1;
        else if(comp_opt == SCISSORS)
            return 0;
        else if(comp_opt == ROCK)
            return -1;
    }

    return -2;
}

bool play_again()
{
    // Create input buffer for human player's option
    const int buff_len = 25;
    char buff[buff_len];

    // Get human's option
    read_line(buff, buff_len);
    if(streq_nocase(buff,"yes") || streq_nocase(buff,"y"))
        return true;
    if(streq_nocase(buff,"no") || streq_nocase(buff,"n"))
        return false;
    return false;
}

bool play_rps(int num_round, int human_wins, int comp_wins)
{
    while((human_wins < 3) && (comp_wins < 3))
    {
        RPS human_opt = play_human(num_round);
        RPS comp_opt = play_computer();

        printf("The computer chooses %s. ", rps_string(comp_opt));
        int round_result = human_vs_comp(human_opt,comp_opt);
        if(round_result == 1)
        {
            human_wins++;
            printf("You won the round! You: %d, Computer: %d\n\n", human_wins, comp_wins);
        }
        else if(round_result == 0)
        {
            printf("It was a tie! You: %d, Computer: %d\n\n", human_wins, comp_wins);
        }
        else if(round_result == -1)
        {
            comp_wins++;
            printf("You lost the round! You: %d, Computer: %d\n\n", human_wins, comp_wins);
        }
        num_round++;
        return play_rps(num_round, human_wins, comp_wins);
    }

    if(human_wins > comp_wins)
        printf("\nYou won the tournament! Play again? ");
    else
        printf("\nYou lost the tournament! Play again? ");

    return play_again();
}

int main()
{
    srand((unsigned int)time(NULL));

    while(play_rps(1,0,0));

    return 0;
}
