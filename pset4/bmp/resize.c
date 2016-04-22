/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a BMP by factor n.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int n;


// Prototypes
int padding(int width);
int biSizeImage(int width, int height);
int bfSize(int biSizeImage);


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember arguments
    n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    
    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    
    if (n < 1 || n > 100)
    {
        printf("n must be an integer between 1 and 100.\n");
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER infileBf;
    fread(&infileBf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER infileBi;
    fread(&infileBi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (infileBf.bfType != 0x4d42 || infileBf.bfOffBits != 54 || infileBi.biSize != 40 || 
        infileBi.biBitCount != 24 || infileBi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }


//==============================================================================    
    // TODO: Set headers for the resized file
    // Clues: https://youtu.be/g8LEbJapnj8 @ 2:30
    // Reference: https://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx
    // https://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx
    
    BITMAPFILEHEADER outfileBf;
    BITMAPINFOHEADER outfileBi;
    
    // Initialize with infile's values
    // Make a true copy. Explicitly declare each property in each header.
    outfileBf.bfType = infileBf.bfType; 
    outfileBf.bfSize = infileBf.bfSize; 
    outfileBf.bfReserved1 = infileBf.bfReserved1; 
    outfileBf.bfReserved2 = infileBf.bfReserved2; 
    outfileBf.bfOffBits = infileBf.bfOffBits; 
    
    outfileBi.biSize = infileBi.biSize; 
    outfileBi.biWidth = infileBi.biWidth; 
    outfileBi.biHeight = infileBi.biHeight; 
    outfileBi.biPlanes = infileBi.biPlanes; 
    outfileBi.biBitCount = infileBi.biBitCount; 
    outfileBi.biCompression = infileBi.biCompression; 
    outfileBi.biSizeImage = infileBi.biSizeImage; 
    outfileBi.biXPelsPerMeter = infileBi.biXPelsPerMeter; 
    outfileBi.biYPelsPerMeter = infileBi.biYPelsPerMeter; 
    outfileBi.biClrUsed = infileBi.biClrUsed; 
    outfileBi.biClrImportant = infileBi.biClrImportant; 
    
    // ==== HEIGHT 
    // biHeight: The height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
    outfileBi.biHeight = infileBi.biHeight * n;
    
    // ==== WIDTH
    // biWidth: The width of the bitmap, in pixels
    outfileBi.biWidth = infileBi.biWidth * n;

    // ==== IMAGE SIZE 
    // biSizeImage: 
        // The size of the image
        // Including pixels and padding
        // Units: bytes
    outfileBi.biSizeImage = biSizeImage(outfileBi.biWidth, outfileBi.biHeight);

    // ==== FILE SIZE 
    // bfSize: The size of the file in bytes.
    outfileBf.bfSize = bfSize(outfileBi.biSizeImage);
    
//==============================================================================

    
    
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&outfileBf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outfileBi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int infilePadding = padding(infileBi.biWidth);
    int outfilePadding = padding(outfileBi.biWidth);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(infileBi.biHeight); i < biHeight; i++)
    {
        // Do each line n times
        for (int y = 0; y < n; y++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < infileBi.biWidth; j++)
            {
                // Read the pixel from infile
                RGBTRIPLE triple;
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // Write the pixel n times
                for (int z = 0; z < n; z++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // For infile, skip over padding, if any
            fseek(inptr, infilePadding, SEEK_CUR);
    
            // For outfile, write the padding, if any
            for (int k = 0; k < outfilePadding; k++)
            {
                fputc(0x00, outptr);
            }
            // Go back to the beginning of the scanline    
            fseek(inptr, -(infileBi.biWidth * 3 + infilePadding ), SEEK_CUR);
        }
        // Go to the end of the scanline
        fseek(inptr, infileBi.biWidth * 3 + infilePadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

int padding(int width)
{
    return (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
}

int bfSize(int biSizeImage)
{
    return (14 + 40 + biSizeImage);
}

int biSizeImage(int width, int height)
{
    // Pixels
    int numberOfPixels = abs(width) * abs(height);
    int pixelBytes = numberOfPixels * sizeof(RGBTRIPLE);
    
    // Pads: Number of pads * bytes per pad
    int paddingBytes = abs(height) * padding(width);
    
    return pixelBytes + paddingBytes;
}