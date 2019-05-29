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
        if(n < 1 || n > 100)
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

    // print to show size of FILE header and info header, 14 and 40 always
    // printf("infile BITMAPFILEHEADER: %lu\n", sizeof(BITMAPFILEHEADER));
    // printf("infile BITMAPINFOHEADER: %lu\n", sizeof(BITMAPINFOHEADER));

    inpad = (bi.biWidth * sizeof(RGBTRIPLE)) % 4;  // make sure this refers to the old width
    printf("bi.biwidth used for infile padding %i\n", bi.biWidth);
    printf("infile padding %d\n", inpad);



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
    // check50 does not pass third test, doesn't resize small.bmp when n is 1 when I change this line
    // bi.biSizeImage = bi.biWidth * bi.biHeight *3;
    // bf.bfSize *= n;


    printf("bi.biWidth value is: %i\n", bi.biWidth);
    printf("bi.biHeight value is: %i\n", bi.biHeight);


    // this should be reading the infile bfSize
    // printf("bf.bfSize infile value is: %i\n", bf.bfSize);


    // size_t fread(void* ptr, size_t size, size_t blocks, FILE* fp);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);   // this should be 14


    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);    // this should be 40

    // edit here to account for outfile's BITMAPINFOHEADERs resize


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



    // determine padding for scanlines
    // padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;  // code from copy.c no resize here

    // change this to make outpad value
    // padding = (4 - (bi.biWidth * n * sizeof(RGBTRIPLE)) % 4) % 4;

    padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    printf("int padding %d\n", padding);


    // inpad =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;  // make sure this refers to the old width


    // int paddingtest =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // int bisizeimagetest = bi.biWidth * bi.biHeight *3;
    // int bfsizetest = bisizeimagetest + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    printf("RGBTRIPLE size is: %lu\n", sizeof(RGBTRIPLE));

    // bf.Size outfile value is total size of file in bytes including pixels, padding and headers
    // bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + padding;
    // bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    printf("bi.biSizeImage value is: %i\n", bi.biSizeImage);

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

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);


        // printf("char *n: %c\n", *n);

        // printf("int paddingtest %i\n", paddingtest);
        // printf("int bisizeimagetest %d\n", bisizeimagetest);
        // printf("int bfsizetest %d\n", bfsizetest);
        printf("int bf.bfSize %d\n", bf.bfSize);


        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            //adding padding back into the image
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





// a way of adjusting for padding


// for (int a = 0; a < height; a++) {
//     for (int b = 0; b < width*3; b++) {
//         if (bmpArray[a*(width*3 + padding)+b] < 127) {
//             bmpArray[a*(width*3 + padding)+b] = 0;
//         } else {
//             bmpArray[a*(width*3 + padding)+b] = 255;
//         }
//     }
//     for (int pad = 0; pad < padding; pad++) {
//         bmpArray[a*(width*3 + padding) + 3*width + pad] = 0x00;
//     }
// }
