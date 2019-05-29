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
    // store number as number not char
    int n = atoi(argv[1]);
        if(n <= 1 || n > 100)
        {
            printf("number n needs to be between 1 and 100\n");
            return 1;
        }
    char *infile = argv[2];
    char *outfile = argv[3];

    // setup variables for padding of the infile and padding of the outfile
    int inpad = 0;
    int padding = 0;


    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    // error message and return if file fails to open
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
        // printf("sizeof(BITMAPFILEHEADER) %lu\n", sizeof(BITMAPFILEHEADER));
        // printf("")

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    int inbiHeight = abs(bi.biHeight);


    inpad = (bi.biWidth * sizeof(RGBTRIPLE)) % 4;  // make sure this refers to the old width
    // inpad = 4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4;
    int inbiWidth = bi.biWidth;

    printf("infile padding %d\n", inpad);
    printf("inbiWidth used for infile padding %i\n", inbiWidth);
    printf("inbiHeight used for infile padding %i\n", inbiHeight);


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // print width and height of the OUTFILE
    bi.biWidth *= n;
    bi.biHeight *= n;


    printf("bi.biWidth value for outfile is: %i\n", bi.biWidth);
    printf("abs(bi.biHeight) for outfile value is: %i\n", abs(bi.biHeight));



    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);   // this should be 14


    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);    // this should be 40

    // edit here to account for outfile's BITMAPINFOHEADERs resize
    bf.bfSize = bi.biSizeImage = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPFILEHEADER);
    printf("int bf.bfSize FIRST %d\n", bf.bfSize);

    // determine padding for scanlines
    inpad = 4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4;  // code from copy.c no resize here
    // inpad =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;  // make sure this refers to the old width

    // change this to make outpad value
    padding =  bi.biWidth * sizeof(RGBTRIPLE) % 4;

    // printf("infile padding %d\n", inpad);
    printf("outfile padding %d\n", padding);

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    printf("RGBTRIPLE size is: %lu\n", sizeof(RGBTRIPLE));
    printf("bi.biSizeImage value is: %i\n", bi.biSizeImage);
    printf("int bf.bfSize SECOND %d\n", bf.bfSize);


    // save each row to an array and then repeat the array n times ????
    // int row[]

    // iterate over infile's scanlines

    for(int m = 0; m < n; m++)
    {
        for (int i = 0, biHeight = abs(bi.biHeight); i < abs(biHeight); i++)
        {

            // iterate over pixels in scanline
            for (int j = 0; j < inbiWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int l = 0; l < n; l++) {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    printf("j, meaning pixel, is %i\n", j);


                }


            }

            printf("i, showing rows is %i\n", i);

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);



            // then add it back (to demonstrate how)
            for (int k = 0; k <= padding; k++)
            {
                //adding padding back into the image
                fputc(0x00, outptr);
                printf("k is padding: %i\n", k);
            }


        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}





//   this is the original loop structure for duplicating the pixels
// // iterate over infile's scanlines
//     for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
//     {
//         // iterate over pixels in scanline
//         for (int j = 0; j < bi.biWidth; j++)
//         {
//             // temporary storage
//             RGBTRIPLE triple;

//             // read RGB triple from infile
//             fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

//             // write RGB triple to outfile
//             fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

//         }

//         // skip over padding, if any
//         fseek(inptr, padding, SEEK_CUR);



//         // then add it back (to demonstrate how)
//         for (int k = 0; k < padding; k++)
//         {
//             //adding padding back into the image
//             fputc(0x00, outptr);
//             printf("k: %i\n", k);
//         }
//     }
