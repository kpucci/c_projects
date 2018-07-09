// Katie Pucci (kev19)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#define COMPRESS "-c"
#define UNCOMPRESS "-d"

unsigned long (*compressBound)(unsigned long length);
int (*compress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);
int (*uncompress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);

// Compare strings without worrying about case
int streq_nocase(const char* a, const char* b)
{
    for(; *a && *b; a++, b++)
        if(tolower(*a) != tolower(*b))
            return 0;
    return *a == 0 && *b == 0;
}

// Dynamically load functions from libz.so
int load_functions(void)
{
    void* lib = dlopen("libz.so", RTLD_NOW);
    if(lib == NULL)
    {
        printf("Couldn't load the library\n");
        return 1;
    }

    compressBound = dlsym(lib, "compressBound");
    if(compressBound == NULL)
    {
        printf("Couldn't load compressBound function\n");
        return 1;
    }

    compress = dlsym(lib, "compress");
    if(compress == NULL)
    {
        printf("Couldn't load compress function\n");
        return 1;
    }

    uncompress = dlsym(lib, "uncompress");
    if(compress == NULL)
    {
        printf("Couldn't load uncompress function\n");
        return 1;
    }
}

int main(int argc, char *argv[])
{
    // If argc < 3, complain and exit
    if(argc < 3)
    {
        printf("Too few arguments\n");
        return 0;
    }

    if(load_functions() == 1)
    {
        return 1;
    }

    // Open the file argv[2] for binary reading
    FILE* input_file = fopen(argv[2],"rb");
    if(!input_file)
    {
        printf("Couldn't open file\n");
        return 1;
    }

    // if argv[1] is "-c"
    if(streq_nocase(argv[1],COMPRESS))
    {
        // Get size of file
        fseek(input_file, 0, SEEK_END);     // Go to end of file
        unsigned long uncompressed_size = ftell(input_file);     // Get size
        fseek(input_file, 0, SEEK_SET);     // Go back to the beginning of the file
        void* uncompressed_buffer = calloc(sizeof(char)*uncompressed_size,1);

        // Read the entire file into an uncompressed buffer
        fread(uncompressed_buffer, sizeof(char)*uncompressed_size, 1, input_file);

        // malloc a compressed buffer using compressBound to figure out what size it should be
        unsigned long compressed_size = compressBound(uncompressed_size);
        void* compressed_buffer = calloc(sizeof(char)*compressed_size, 1);

        // use compress() to compress the uncompressed buffer into the compressed buffer
        compress(compressed_buffer, &compressed_size, uncompressed_buffer, uncompressed_size);

        // Create pointers for fwrite
        void* uncompressed_size_ptr = &uncompressed_size;
        void* compressed_size_ptr = &compressed_size;

        // Output data
        fwrite(uncompressed_size_ptr,sizeof(unsigned long),1,stdout);
        fwrite(compressed_size_ptr,sizeof(unsigned long),1,stdout);
        fwrite(compressed_buffer,sizeof(char)*compressed_size,1,stdout);

    }
    else if(streq_nocase(argv[1],UNCOMPRESS))
    {
        // Get the sizes from the compressed file
        void* uncompressed_size_buffer = calloc(sizeof(unsigned long), 1);
        void* compressed_size_buffer = calloc(sizeof(unsigned long), 1);
        fread(uncompressed_size_buffer, sizeof(unsigned long),1,input_file);
        fread(compressed_size_buffer, sizeof(unsigned long),1,input_file);

        unsigned long uncompressed_size = *(unsigned long*)uncompressed_size_buffer;
        unsigned long compressed_size = *(unsigned long*)compressed_size_buffer;

        // Read in compressed data
        void* compressed_buffer = calloc(sizeof(char)*compressed_size, 1);
        fread(compressed_buffer,sizeof(char)*compressed_size,1,input_file);

        // Uncompress data and write
        void* uncompressed_buffer = calloc(sizeof(char)*uncompressed_size,1);
        uncompress(uncompressed_buffer, &uncompressed_size, compressed_buffer, compressed_size);
        fwrite(uncompressed_buffer,sizeof(char)*compressed_size,1,stdout);
    }
    else
    {
        printf("Invalid argument\n");
        return 1;
    }

    fclose(input_file);
}
