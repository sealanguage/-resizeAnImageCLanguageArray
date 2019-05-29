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


    // structs for new bitmapfileheader
    BITMAPFILEHEADER OUTbf = bf;
    BITMAPINFOHEADER OUTbi = bi;

    // write outfile's BITMAPFILEHEADER
    fwrite(&OUTbf, sizeof(BITMAPFILEHEADER), 1, outptr);   // this should be 14

    // write outfile's BITMAPINFOHEADER
    fwrite(&OUTbi, sizeof(BITMAPINFOHEADER), 1, outptr);    // this should be 40

    // edit here to account for outfile's BITMAPINFOHEADERs resize
    OUTbf.bfSize = bi.biSizeImage = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPFILEHEADER);
    printf("int bf.bfSize FIRST %d\n", OUTbf.bfSize);

    // determine padding for scanlines
    // inpad = 4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4;  // code from copy.c no resize here
    inpad =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;  // make sure this refers to the old width

    // change this to make outpad value
    // padding =  bi.biWidth * sizeof(RGBTRIPLE) % 4;

    padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // printf("infile padding %d\n", inpad);
    printf("outfile padding %d\n", padding);

    OUTbi.biSizeImage = ((sizeof(RGBTRIPLE) * OUTbi.biWidth) + padding) * abs(OUTbi.biHeight);
    printf("RGBTRIPLE size is: %lu\n", sizeof(RGBTRIPLE));
    printf("bi.biSizeImage value is: %i\n", OUTbi.biSizeImage);
    printf("int bf.bfSize SECOND %d\n", OUTbf.bfSize);


    // save each row to an array and then repeat the array n times ????

    // int nrows = n;
    // int ncolumns = n;
    int newbi = 0;
    // RGBTRIPLE array[bi.biWidth];



    // int **array = malloc(nrows * sizeof(int *));
    // remember to clear the malloc before finishing !!!!!!!!!!!!!!!



    int outbiWidth = bi.biWidth;
    printf("outbiWidth is %i\n", outbiWidth);

    // iterate over infile's scanlines
    // *** for each row ***
        for (int i = 0, biHeight = abs(bi.biHeight); i < abs(biHeight); i++)
        {

            for (int vertical = 0; vertical < n; vertical++)
            {


                // iterate over pixels in scanline
                // *** for each pixel ***
                for (int j = 0; j < inbiWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    printf("RGBTRIPLE triple is %lu\n", sizeof(RGBTRIPLE));
                    printf("newbi.biWidth is: %i\n", newbi);

                        // *** write RGB triple to array if you can ... ***
                        for (int l = 0; l < n; l++)
                        {
                            // fwrite(&triple, sizeof(RGBTRIPLE), 1, **row_arr);
                            // scanline [l] = triple;
                            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                            // write to the array row_arr[r]

                            printf("j, meaning pixel, is %i\n", j);

                        }


    //              *** for n number of times ***
    //                  *** write array to outfile ***
    //                  *** write outfile padding ***


                //     for(int m = 0; m < nrows; m++)
                //     {
    		          //// write the pizels to the array
    		          //array[m] = malloc(ncolumns * sizeof(int));
    		          //printf("m loop values: %i\n", m);
                //     }

                printf("i, showing rows is %i\n", i);

    //          *** skip over infile padding ***
                // skip over padding, if any
                fseek(inptr, padding, SEEK_CUR);



                // then add it back (to demonstrate how)
                for (int k = 0; k < padding; k++)
                {
                    //adding padding back into the image
                    fputc(0x00, outptr);
                    printf("k is padding: %i\n", k);
                }

                if (vertical < n - 1)
                {
                    fseek(inptr, - (bi.biWidth * 3 + inpad), SEEK_CUR );
                }
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



// make an array syntax

// int main()
// {
//   int arr[5] = { 1, 2, 3, 4, 5 };
//   int *ptr = arr;

//   printf("%p\n", ptr);
//   return 0;
// }


//  use this pseudocode to write the loops

// for each row
//       for each pixel
//          write to array n times
//       for n number of times
//          write array to outfile
//          write outfile padding
//       skip over infile padding

// As I understand it, array is going to hold one new row of RGBTRIPLEs. The declaration would be RGBTRIPLE array[newbi.biWidth]. You do not need sizeof(RGBTRIPLE) because it is already a collection of RGBTRIPLE objects.

// This "new row" will be built horizontally ("write to array n times", which is the j loop), and then written vertically ("for n times...", which is the l loop). You always want to start building it at array[0] and writing it from array[0]. Therefore you need to reset counter3 to 0 before you start the next scanline.