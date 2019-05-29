// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy n infile outfile\n");
        return 1;
    }

    // remember filenames
    char n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // int width = *n;
    // atoi(size);
    // size <= 100;
    printf("n is: %i\n", n);


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

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    printf("bi.biSizeImage: %i\n", bi.biSizeImage);
    printf("bi.biWidth: %i\n", bi.biWidth);
    // bi.biWidth -= 1;
    // printf("New bi.biWidth: %i\n", bi.biWidth);
    printf("New bi.biHeight: %i\n", bi.biHeight);
    printf("New abs(bi.biHeight): %i\n", abs(bi.biHeight));

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    int factor = bi.biWidth * n;
    int newRow[factor];
    printf("newRow size: %i\n", bi.biWidth * n);

    double balance[5] = {1000.0, 2.0, 3.4, 7.0, 50.0};

    for (int z = 0; z < bi.biWidth; z++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //instead, write RGB triple to newRow[]
            fwrite("RGBTRIPLE is: %i\n", RGBTRIPLE triple);


            // int newWidth[] = {};   // in the {}, write the values of the RGBTRIPLE by factor times

            // write RGB array triple to outfile
            // fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

    // printf("newRow array: %i\n", newRow[]);



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
        }


        printf("padding: %i\n", padding);
        // printf("positiveinteger: %d\n", n);

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
