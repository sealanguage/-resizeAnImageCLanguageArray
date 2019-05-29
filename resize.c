// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
        if(n < 1 || n > 100)
        {
            printf("n should be between 1 and 100\n");
            return 1;
        }
    char *infile = argv[2];
    char *outfile = argv[3];

    int inpad = 0;
    int outpad = 0;



    // printf("char *n: %c\n", *n);
    printf("int *inpad %d\n", inpad);
    printf("int *outpad %d\n", outpad);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
        printf("sizeof(BITMAPFILEHEADER) %lu\n", sizeof(BITMAPFILEHEADER));
        // printf("")

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);



    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    bi.biWidth *= n;
    bi.biHeight *= n;

    printf("bi.biWidth value is: %i\n", bi.biWidth);
    printf("bi.biHeight value is: %i\n", bi.biHeight);

    typedef struct tagBITMAPINFOHEADER {
        DWORD biSize;
        LONG  biWidth;
        LONG  biHeight;
        WORD  biPlanes;
        WORD  biBitCount;
        DWORD biCompression;
        DWORD biSizeImage;
        LONG  biXPelsPerMeter;
        LONG  biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    } BITMAPINFOHEADER;


    // printf("tagBITMAPINFOHEADER : %lu\n",  biWidth);

    // size_t fread(void* ptr, size_t size, size_t blocks, FILE* fp);



    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);



    // if(bi.biSizeImage == (bi.biWidth * bi.biHeight *3))
    // {
    //     // no padding needed
    //     int padding = 0;
    //     int padding2 = 0;
    // }
    // else
    // {
    //     // determine padding for scanlines
    //     padding =  (4 - (bi.biWidth * n * sizeof(RGBTRIPLE)) % 4) % 4;
    //     padding2 =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // }


    // printf("padding and padding2 %i %i\n", padding, padding2);


    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            // prints padding value and buHeight value of outgoing (?) file
            // printf("biHeight: %i\n", biHeight);
            // printf("padding: %i\n", padding);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            // k is the changed padding, shows as k: 0  or k: 1 for *n = 2 on 3x3.bmp
            fputc(0x00, outptr);
            printf("k: %i\n", k);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}




    // int main(int argc, char* argv[])
    // {
    //     int padding2;
    //     int padding;
    //     // ensure proper usage if (argc != 4) { printf("Usage: ./resize n infile outfile\n"); return 1; }

    //     int n = atoi(argv[1]);
    //     if(n < 1 || n > 100)
    //     {
    //         printf("n should be between 1 and 100\n");
    //         return 1;
    //     }