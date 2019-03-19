#pragma pack(1) /* makes the struct format line with the file */
#include <stdio.h> /* printf() fseek() fwrite() fopen() fread() */
#include <math.h> /*  */
#include <string.h> /* strcmp() */

//global flags
static int debug = 0; /* Whether or not to print debug messages */
//flags for different image handling modes
static int invert = 0;
static int grayscale = 0;

struct BMP_Header {
    char format[2];
    unsigned int size;
    unsigned int reserved1;
    unsigned int offset;
};

struct DIB_Header {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short color_planes;
    unsigned short bit_depth;
    unsigned int compression;
    unsigned int img_size;
    unsigned int horiz_resolution;
    unsigned int vert_resolution;
    unsigned int color_count;
    unsigned int imp_color_count;
};

struct pixel {
    unsigned char B;
    unsigned char G;
    unsigned char R;
};

/**
 * @brief help() prints a command line help message
 **/
void help() {
    printf("correct usage: bmp_edit <option> <file> <flag>\n");
    printf("    options:\n");
    printf("        -invert       invert the image file\n");
    printf("        -grayscale    convert the image to grayscale\n");
    printf("\n");
    printf("    flags:\n");
    printf("        -debug        toggles the debug option to print header information.\n");
    printf("\n");
}

/**
 * @brief invert_pixel() inverts a single pixel
 * @param [in] struct pixel* curr_pixel = the pixel to perform the invert on
 **/
void invert_pixel(struct pixel* curr_pixel) {
    (*curr_pixel).R = ~(*curr_pixel).R;
    (*curr_pixel).G = ~(*curr_pixel).G;
    (*curr_pixel).B = ~(*curr_pixel).B;
}

/**
 * @brief grayscale_pixel() grayscales a single pixel
 * @param [in] struct pixel* curr_pixel = the pixel to perform the transform on
 * @details
 *     Y = 0.2126 * R + 0.7152 * G + 0.0722 * B
 *
 *           |12.92 Y                     Y <= 0.0031308
 *     B=G=R=|1.055 * Y^(1/2.4) - 0.055   Y  > 0.0031308
 *
 **/
void grayscale_pixel(struct pixel* curr_pixel) {
    double rd = ((double)(*curr_pixel).R) / 255;
    double gd = ((double)(*curr_pixel).G) / 255;
    double bd = ((double)(*curr_pixel).B) / 255;

    double Y = 0.2126 *rd + 0.7152 *gd + 0.0722 *bd;
    double val = 0;
    if(Y <= 0.0031308) {
        val = 12.92 * Y * 255;
    } else {
        val = (1.055 * pow(Y, 1.0 / 2.4) - 0.055) * 255;
    }

    (*curr_pixel).R = (unsigned char)(val);
    (*curr_pixel).G = (unsigned char)(val);
    (*curr_pixel).B = (unsigned char)(val);
}

/**
 * @brief print_bmp_header() prints the information contained in a BMP header
 * @param [in] struct BMP_Header * bmp_head = the header to print from
 **/
void print_bmp_header(struct BMP_Header* bmp_head) {
    printf("[BMP_Header] format: %c%c\n", (*bmp_head).format[0], (*bmp_head).format[1]);
    printf("[BMP_Header] size: %d\n", (*bmp_head).size);
    printf("[BMP_Header] offset: %d\n", (*bmp_head).offset);
    return;
}

/**
 * @brief print_dib_header() prints the information contained in a DIB header
 * @param [in] struct DIB_Header * dib_head = the header to print from
 **/
void print_dib_header(struct DIB_Header* dib_head) {
    printf("[DIB Header] size: %d\n", (*dib_head).size);
    printf("[DIB Header] width: %d\n", (*dib_head).width);
    printf("[DIB Header] heigth: %d\n", (*dib_head).height);
    printf("[DIB Header] color_planes: %d\n", (*dib_head).color_planes);
    printf("[DIB Header] bit_depth: %d\n", (*dib_head).bit_depth);
    printf("[DIB Header] compression: %d\n", (*dib_head).compression);
    printf("[DIB Header] img_size: %d\n", (*dib_head).img_size);
    printf("[DIB Header] horiz_resolution: %d\n", (*dib_head).horiz_resolution);
    printf("[DIB Header] vert_resolution: %d\n", (*dib_head).vert_resolution);
    printf("[DIB Header] color_count: %d\n", (*dib_head).color_count);
    printf("[DIB Header] imp_color_count: %d\n", (*dib_head).imp_color_count);
    return;
}

int main(int argc, char** argv) {
    /**** First parse arguments ****/
    // check numbers
    if(argc < 3) {
        printf("[Error] Incorrect number of arguments.");
        help();
        return 1;
    }
    // flags
    int i = 3;
    while(i < argc) {
        if(strcmp(argv[i], "-debug") == 0 || strcmp(argv[i], "-d") == 0) {
            debug = 1;
        } else {
            printf("Invalid flag: %s\n", argv[i]);
            help();
            return 1;
        }
        ++i;
    }
    // options
    if(strcmp(argv[1], "-invert") == 0) {
        invert = 1;
    } else if(strcmp(argv[1], "-grayscale") == 0) {
        grayscale = 1;
    } else {
        printf("Please enter a valid option.");
        help();
        return 1;
    }

    /**** File Opening ****/
    FILE *fp;
    fp = fopen(argv[2], "rb+");
    //detect errors
    if(fp == NULL) {
        printf("Error opening file! Exitting.\n");
        return 1;
    }

    /**** Read Headers ****/
    struct BMP_Header bmp_head;
    struct DIB_Header dib_head;

    // BMP Header
    fread(&bmp_head, sizeof(struct BMP_Header), 1, fp);
    if(debug) {
        print_bmp_header(&bmp_head);
    }
    if(bmp_head.format[0] != 'B' || bmp_head.format[1] != 'M') {
        //printf("Given format: %c%c, is unsupported. Exitting.\n", bmp_head.format[0], bmp_head.format[1]);
        // changed error message to match autograder
        printf("We do not support the file format\n");
        return 1;
    }

    // DIB Head
    fread(&dib_head, sizeof(struct DIB_Header), 1, fp);
    if(debug) {
        print_dib_header(&dib_head);
    }
    if(dib_head.size != 40) {
        //printf("DIB Header format size: %d, is unsupported. Exitting.\n", dib_head.size);
        //changed error message to match autograder
        printf("We do not support the file format\n");
        return 1;
    }
    if(dib_head.bit_depth != 24) {
        //printf("Image bit depth: %d, unsupported. Exitting.", dib_head.bit_depth);
        //changed error message to match autograder
        printf("We do not support the file format\n");
        return 1;
    }

    /**** Perform Functions on Pixels ****/
    fseek(fp, bmp_head.offset, SEEK_SET);

    i = 0;
    while(i < dib_head.height) {
        int j = 0;
        while(j < dib_head.width) {
            // read a pixel
            struct pixel curr_pixel;
            fread(&curr_pixel, sizeof(struct pixel), 1, fp);

            // perform an operation
            if(invert) {
                invert_pixel(&curr_pixel);
            }
            if(grayscale) {
                grayscale_pixel(&curr_pixel);
            }

            //go to the correct location and write the revised pixel in
            fseek(fp, -sizeof(struct pixel), SEEK_CUR);
            fwrite(&curr_pixel, sizeof(struct pixel), 1, fp);
            ++j;
        }
        // handle padding at end of rows
        /*
         * dib_head.width * 3 = number of bytes in row
         * that % 4 = # of bytes above a multipe of 4
         * 4 - that = bytes remaining until a multiple of 4
         *    PROBLEM: if this number is a multuple of 4 then 4 - that = 4...
         * so... we %4 that it will fix it and be 1..3
         */
        fseek(fp, (4 - (dib_head.width*3)%4)%4 , SEEK_CUR);
        ++i;
    }
    return 0;
}
