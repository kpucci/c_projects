// Katie Pucci (kev19)

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#pragma pack(1)

typedef struct BMPHeader {
    unsigned char format_id[2];
    unsigned int file_size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

typedef struct DIBHeader {
    unsigned int DIB_size;
    unsigned int bmp_width;
    unsigned int bmp_height;
    unsigned short num_color_panes;
    unsigned short bits_per_pix;
    unsigned int compression;
    unsigned int bmp_size;
    unsigned int hor_res;
    unsigned int ver_res;
    unsigned int num_colors;
    unsigned int num_imp_colors;
} DIBHeader;

typedef struct Pixel {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Pixel;

// Compare strings without worrying about case
int streq_nocase(const char* a, const char* b)
{
    for(; *a && *b; a++, b++)
        if(tolower(*a) != tolower(*b))
            return 0;
    return *a == 0 && *b == 0;
}

// Get type of edit from cmd line (--invert = 0, --grayscale = 1)
int get_edit_type(const char* input)
{
    const char* invert_type = "--invert";
    const char* gray_type = "--grayscale";

    if(streq_nocase(input, invert_type))
        return 0;
    if(streq_nocase(input, gray_type))
        return 1;
    else return -1;
}

// Calculate padding
int calculate_padding(int width)
{
    int num_bytes = width*3;
    if(num_bytes % 4 != 0)
        num_bytes += 4-(num_bytes % 4);

    return num_bytes - (width*3);
}

// Validate the BMP header by checking its format id
bool verify_bmp_head(unsigned char* format_id)
{
    const int B = 66; // ASCII character code for 'B'
    const int M = 77; // ASCII character code for 'M'

    // Check BM identifier
    if(!(format_id[0] == B && format_id[1] == M))
    {
        printf("ERROR:: File has incorrect format identifier: %c%c\n", format_id[0], format_id[1]);
        return false;
    }
    return true;
}

// Validate the DIB header by checking its size and bits/pixel values
bool verify_dib_head(int dib_size, int bits_per_pix)
{
    if(dib_size != 40)
    {
        printf("ERROR:: File has incorrect DIB header size: %d\n", dib_size);
        return false;
    }
    if(bits_per_pix != 24)
    {
        printf("ERROR:: File has incorrect bits per pixel: %d\n", bits_per_pix);
        return false;
    }
    return true;
}

void invert(Pixel* pix)
{
    pix->b = ~(pix->b);
    pix->g = ~(pix->g);
    pix->r = ~(pix->r);
}

double get_brightness(double b, double g, double r)
{
    return (0.2126 * r) + (0.7152 * g) + (0.0722 * b);
}

double get_grayscale(double brightness)
{
    if(brightness <= 0.0031308)
        return 12.92*brightness;
    else
        return ((1.055*pow(brightness,(1.0/2.4))) - 0.055) * 255;
}

void grayscale(Pixel* pix)
{
    // Normalize pixel BGR values
    double b_normal = ((double)(pix->b))/255.0;
    double g_normal = ((double)(pix->g))/255.0;
    double r_normal = ((double)(pix->r))/255.0;

    // Get brightness
    double brightness = get_brightness(b_normal, g_normal, r_normal);

    // Get grayscale value
    double gray_val = get_grayscale(brightness);

    pix->b = (char)gray_val;
    pix->g = (char)gray_val;
    pix->r = (char)gray_val;
}

void edit_image(FILE* bmp_file, BMPHeader* bmp_head, DIBHeader* dib_head, int type)
{
    // One pixel = 3 bytes
    const int PIXEL_SIZE = 3;

    // Get padding per line
    int padding = calculate_padding(dib_head->bmp_width);

    // Variable for pixel to read in
    Pixel pix;

    // Go to start of pixels
    fseek(bmp_file, bmp_head->offset, SEEK_SET);

    // For each row in bmp
    for(int i=0; i<dib_head->bmp_height; i++)
    {   // For each pixel in row
        for(int j=0; j<dib_head->bmp_width; j++)
        {
            fread(&pix, PIXEL_SIZE, 1, bmp_file);   // Read pixel

            // Transform pixel BGR values
            if(type == 0)
                invert(&pix);
            else if(type == 1)
                grayscale(&pix);

            fseek(bmp_file, PIXEL_SIZE*-1, SEEK_CUR);   // Return to pixel's start location
            fwrite(&pix, PIXEL_SIZE, 1, bmp_file);      // Overwrite original values with transformed values
        }
        fseek(bmp_file, padding, SEEK_CUR); // Skip padding at end of row
    }

}

int main(int argc, char *argv[])
{
    const int BMP_HEAD_SIZE = 14;
    const int DIB_HEAD_SIZE = 40;

    // Open image
    FILE *bmp_file = fopen(argv[2],"rb+");
    if(!bmp_file)
    {
        printf("Couldn't open file\n");
        return 0;
    }

    // Read in BMP header and validate it
    BMPHeader bmp_head;
    fread(&bmp_head, BMP_HEAD_SIZE, 1, bmp_file);
    if(!verify_bmp_head(bmp_head.format_id))
        return 0;

    // Read in DIB header and validate it
    DIBHeader dib_head;
    fread(&dib_head, DIB_HEAD_SIZE, 1, bmp_file);
    if(!verify_dib_head(dib_head.DIB_size, dib_head.bits_per_pix))
        return 0;

    // Get type of edit (--invert or --grayscale)
    int edit_type = get_edit_type(argv[1]);

    // Edit the image
    edit_image(bmp_file, &bmp_head, &dib_head, edit_type);

    // Close file
    fclose(bmp_file);
    return 0;
}
