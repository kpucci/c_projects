// Katie Pucci (kev19)

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Get input from user and store in buffer
void read_line(const char* message, char* buffer, int length)
{
    printf(message);
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

// Convert weight depending on planet
float convert_weight(int weight, const char* planet)
{
    // Weight conversion factors
    const float MERCURY_FACTOR = 0.38;
    const float VENUS_FACTOR = 0.91;
    const float MARS_FACTOR = 0.38;
    const float JUPITER_FACTOR = 2.54;
    const float SATURN_FACTOR = 1.08;
    const float URANUS_FACTOR = 0.91;
    const float NEPTUNE_FACTOR = 1.19;

    // Conversions for valid planets
    if(streq_nocase(planet,"mercury"))
        return weight*MERCURY_FACTOR;
    if(streq_nocase(planet,"venus"))
        return weight*VENUS_FACTOR;
    if(streq_nocase(planet,"mars"))
        return weight*MARS_FACTOR;
    if(streq_nocase(planet,"jupiter"))
        return weight*JUPITER_FACTOR;
    if(streq_nocase(planet,"saturn"))
        return weight*SATURN_FACTOR;
    if(streq_nocase(planet,"uranus"))
        return weight*URANUS_FACTOR;
    if(streq_nocase(planet,"neptune"))
        return weight*NEPTUNE_FACTOR;

    // If user enters earth, return -2
    if(streq_nocase(planet,"earth"))
        return -2;

    // If not a planet, return -1
    return -1;
}

int main()
{
    const char* weight_prompt = "Please enter your weight:\n";
    const char* planet_prompt = "Which planet do you want to go to ('exit' to exit)?\n";

    const int buff_len = 100;

    char w_buff[buff_len];
    read_line(weight_prompt, w_buff, buff_len);
    int weight;
    sscanf(w_buff, "%d", &weight);

    while(1)
    {
        char p_buff[buff_len];
        read_line(planet_prompt, p_buff, buff_len);

        if(streq_nocase(p_buff,"exit"))     // Check for program termination input
            break;

        float converted_weight = convert_weight(weight,p_buff);

        if(converted_weight == -2)          // If user typed earth, call them silly
            printf("Silly goose, you're already on planet Earth.\n\n");
        else if(converted_weight == -1)     // If user typed an invalid planet, tell them
            printf("That's not a planet.\n\n");
        else                                // Otherwise, tell them their weight
            printf("You would weigh %.2f on planet %s.\n\n", converted_weight, p_buff);
    }

    return 0;
}
