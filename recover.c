#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef uint8_t BYTE;
bool is_jpeg(BYTE buffer[]);

int main(int argc, char *argv[])
{

    char *infile = argv[1];
    if (argc != 2)
    {
        printf("Usage: recover infile\n");
        return 1;
    }

    FILE *inptr = fopen(infile, "r");

    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 4;
    }
    int eof = 0;
    int jpg_count = 0;
    int n_blocks;
    char filename[8];
    bool start = true;
    FILE *image;


    while (eof == 0)
    {

        BYTE buffer[512];
        n_blocks = fread(buffer, sizeof(BYTE), 512, inptr);
        bool new_jpeg = is_jpeg(buffer);

        if (new_jpeg && start)
        {
            sprintf(filename, "%03i.jpg", jpg_count);
            image = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), n_blocks, image);
            jpg_count++;
            start = false;

        }
        else if (new_jpeg && !start)
        {
            sprintf(filename, "%03i.jpg", jpg_count);
            image = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), n_blocks, image);
            jpg_count++;

        }
        else if (!start)
        {
            fwrite(buffer, sizeof(BYTE), n_blocks, image);
        }

        if (n_blocks < 512)
        {
            eof = 1;
        }
    }
}

bool is_jpeg(BYTE buffer[])
{
    BYTE signatures[3] = {0xff, 0xd8, 0xff};
    bool is = false;
    for (int i = 0; i < 4; i++)
    {
        if (i < 3)
        {
            is = (signatures[i] == buffer[i]);
        }
        else
        {
            BYTE buffer_4 = (buffer[3] & 0xf0);
            is = (buffer_4 == 0xe0);
        }
        if (!is)
        {
            return false;
        }
        else
        {
            continue;
        }
    }
    return true;
}
