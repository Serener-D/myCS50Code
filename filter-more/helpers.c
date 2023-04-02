#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

RGBTRIPLE generateBlurredPixel(int currentI, int currentJ, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE applySobel(int currentI, int currentJ, int height, int width, RGBTRIPLE image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = (int) round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j] = (RGBTRIPLE)
            {
                average,
                average,
                average
            };
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*bufferImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        int bufferColumnIndex = width - 1;
        for (int j = 0; j < width; j++)
        {
            bufferImage[i][bufferColumnIndex] = (RGBTRIPLE)
            {
                image[i][j].rgbtBlue,
                      image[i][j].rgbtGreen,
                      image[i][j].rgbtRed
            };
            bufferColumnIndex--;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = (RGBTRIPLE)
            {
                bufferImage[i][j].rgbtBlue,
                            bufferImage[i][j].rgbtGreen,
                            bufferImage[i][j].rgbtRed
            };
        }
    }
    free(bufferImage);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*bufferImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE blurredPixel = generateBlurredPixel(i, j, height, width, image);
            bufferImage[i][j] = blurredPixel;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = bufferImage[i][j];
        }
    }
    free(bufferImage);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*bufferImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE blurredPixel = applySobel(i, j, height, width, image);
            bufferImage[i][j] = blurredPixel;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = bufferImage[i][j];
        }
    }
    free(bufferImage);
    return;
}

RGBTRIPLE generateBlurredPixel(int currentI, int currentJ, int height, int width, RGBTRIPLE image[height][width])
{
    // change array argument to pointer
    int blueSum = 0;
    int greenSum = 0;
    int redSum = 0;

    int startingI;
    int lastI;
    if (currentI == 0)
    {
        startingI = currentI;
    }
    else
    {
        startingI = currentI - 1;
    }

    if (currentI == height - 1)
    {
        lastI = currentI + 1;
    }
    else
    {
        lastI = currentI + 2;
    }

    int pixelsCounter = 0;

    for (int i = startingI; i < lastI; i++)
    {
        if (currentJ != 0)
        {
            blueSum += image[i][currentJ - 1].rgbtBlue;
            greenSum += image[i][currentJ - 1].rgbtGreen;
            redSum += image[i][currentJ - 1].rgbtRed;
            pixelsCounter += 1;
        }
        blueSum += image[i][currentJ].rgbtBlue;
        greenSum += image[i][currentJ].rgbtGreen;
        redSum += image[i][currentJ].rgbtRed;
        pixelsCounter += 1;
        if (currentJ != width - 1)
        {
            blueSum += image[i][currentJ + 1].rgbtBlue;
            greenSum += image[i][currentJ + 1].rgbtGreen;
            redSum += image[i][currentJ + 1].rgbtRed;
            pixelsCounter += 1;
        }
    }
    return (RGBTRIPLE)
    {
        (int) round((float) blueSum / (float) pixelsCounter),
        (int) round((float) greenSum / (float) pixelsCounter),
        (int) round((float) redSum / (float) pixelsCounter)
    };
}

RGBTRIPLE applySobel(int currentI, int currentJ, int height, int width, RGBTRIPLE image[height][width])
{
    int kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // change array argument to pointer
    int blueXSum = 0;
    int greenXSum = 0;
    int redXSum = 0;

    int blueYSum = 0;
    int greenYSum = 0;
    int redYSum = 0;

    for (int i = currentI - 1, kernelI = 0, kernelJ = 0; i < currentI + 2; i++, kernelI++)
    {
        if (i < 0 || i >= height)
        {
            continue;
        }
        if (currentJ > 0)
        {
            blueXSum += image[i][currentJ - 1].rgbtBlue * kernelX[kernelI][kernelJ];
            greenXSum += image[i][currentJ - 1].rgbtGreen * kernelX[kernelI][kernelJ];
            redXSum += image[i][currentJ - 1].rgbtRed * kernelX[kernelI][kernelJ];
            blueYSum += image[i][currentJ - 1].rgbtBlue * kernelY[kernelI][kernelJ];
            greenYSum += image[i][currentJ - 1].rgbtGreen * kernelY[kernelI][kernelJ];
            redYSum += image[i][currentJ - 1].rgbtRed * kernelY[kernelI][kernelJ];
        }

        blueXSum += image[i][currentJ].rgbtBlue * kernelX[kernelI][kernelJ + 1];
        greenXSum += image[i][currentJ].rgbtGreen * kernelX[kernelI][kernelJ + 1];
        redXSum += image[i][currentJ].rgbtRed * kernelX[kernelI][kernelJ + 1];
        blueYSum += image[i][currentJ].rgbtBlue * kernelY[kernelI][kernelJ + 1];
        greenYSum += image[i][currentJ].rgbtGreen * kernelY[kernelI][kernelJ + 1];
        redYSum += image[i][currentJ].rgbtRed * kernelY[kernelI][kernelJ + 1];

        if (currentJ < width - 1)
        {
            blueXSum += image[i][currentJ + 1].rgbtBlue * kernelX[kernelI][kernelJ + 2];
            greenXSum += image[i][currentJ + 1].rgbtGreen * kernelX[kernelI][kernelJ + 2];
            redXSum += image[i][currentJ + 1].rgbtRed * kernelX[kernelI][kernelJ + 2];
            blueYSum += image[i][currentJ + 1].rgbtBlue * kernelY[kernelI][kernelJ + 2];
            greenYSum += image[i][currentJ + 1].rgbtGreen * kernelY[kernelI][kernelJ + 2];
            redYSum += image[i][currentJ + 1].rgbtRed * kernelY[kernelI][kernelJ + 2];
        }
    }

    int bluePixel = (int) round(sqrt(blueXSum * blueXSum + blueYSum * blueYSum));
    if (bluePixel > 255)
    {
        bluePixel = 255;
    }
    int greenPixel = (int) round(sqrt(greenXSum * greenXSum + greenYSum * greenYSum));
    if (greenPixel > 255)
    {
        greenPixel = 255;
    }
    int redPixel = (int) round(sqrt(redXSum * redXSum + redYSum * redYSum));
    if (redPixel > 255)
    {
        redPixel = 255;
    }
    return (RGBTRIPLE)
    {
        bluePixel,
        greenPixel,
        redPixel
    };
}
