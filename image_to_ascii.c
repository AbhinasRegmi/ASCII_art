#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //lets set a file pointer to open bmp image and create text file to store output ascii
    FILE *fp_image_input;
    FILE *fp_text_output;

    fp_image_input = fopen("image.bmp", "rb");
    fp_text_output = fopen("image_into_ascii.txt", "wb");

    //if required file doesn't exist exit form the programme
    if ( fp_image_input == NULL || fp_text_output == NULL)
    {
        printf("Couldn't find the image.bmp file.\n");
        printf("Press any any to exit..");
        getchar();
        return 1;
    }

    //all images have metadata that describes the image
    //the first 54 bits in bmp image represent its metadata
    //unsigned char is used because it has number range from 0-255
    //0-255 is also the color value range of pixels
    unsigned char metadata[54];

    //store all the metadata from input image source
    fread(metadata, sizeof(unsigned char), 54, fp_image_input);

    //extract width and height from the metadata
    //i have no idea how metadata is stored
    //found this method in a article
    int width = *(int*)&metadata[18];
    int height = abs(*(int*)&metadata[22]);

    //we will change the pixel color value with some ascii character listed below
    char ascii[] = "`~!@#$%^&*,./':";
    int length = sizeof(ascii)/sizeof(char);

    //each pixel in image have three values of (red, blue, green)
    //so we will read three bytes at one to know all the color values of individula pixel
    unsigned char pixel[3];

    //we will run a loop and change each pixel to corrosponding ascii character
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //storing color value of pixel to pixel
            fread(pixel, 3, 1, fp_image_input);

            //during output to text file all color values (red, green, blue) are printed
            //so to represent one pixel with one character average is taken
            unsigned char pixel_average = (pixel[0] + pixel[1] + pixel[2])/3;

            //the pixel color value can range from(0-255)
            //but we have limited ascii characters
            //so a range of pixel color values are represented by single charater
            //example color range (0-17) is represented by `
            int min = 0, max = 17;
            int count = 0;
            
            //matching all the ascii values
            for(int i = 0; i< length; i++){
                if( pixel_average >= min && pixel_average <= max){
                    fwrite(&ascii[count], sizeof(char), 1, fp_text_output);
                    break;
                }
                min = max;
                max += 17;
                count += 1;
            }
        }
        
        //when one pixel width is calculated
        //we have to print out next pixel width to next line so
        fwrite("\n", sizeof(char), 1, fp_text_output);
    }

    //close all the file pointers
    fclose(fp_text_output);
    fclose(fp_image_input);

    return 0;
}


