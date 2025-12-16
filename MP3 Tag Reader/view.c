#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type.h"
#include "view.h"

OperationType check_operation (const char* argv)
{
    if (strcmp(argv, "-v") == 0)
    {
        return m_view;
    }
    else if (strcmp (argv, "-e") == 0)
    {
        return m_edit;
    }
    else if (strcmp (argv, "--help") == 0 || strcmp (argv, "-h") == 0)
    {
        return m_help;
    }
    else
    {
        return m_unsupported;
    }
}

Status read_and_validate_for_view(const char* argv, TagInfo* mp3tagInfo)
{
    if (argv == NULL)
    {
        printf("INFO: To view please pass like: ./a.out -v mp3filename\n");
        return m_failure;
    }

    mp3tagInfo->fptr = fopen(argv, "r");
    if (mp3tagInfo->fptr == NULL)
    {
        printf("ERROR: Unable to open the file %s.\n", argv);
        printf("INFO: To view please pass like: ./a.out -v mp3filename\n");
        return m_failure;
    }

    char str[4]; 
    fread(str, 1, 3, mp3tagInfo->fptr);
    
    if (strcmp(str, "ID3") == 0)
    {
        printf("-------------------Version ID: v2.3--------------------\n");
        printf("-------------------------------------------------------\n");
    }
    else
    {
        printf("ERROR: The file is not '.mp3' extension file.\n");
        printf("INFO: To view please pass like: ./a.out -v mp3filename\n");
        fclose(mp3tagInfo->fptr);
        return m_failure;
    }

    fclose(mp3tagInfo->fptr);
    return m_success;
}

Status view_and_display_tag(const char *argv, TagInfo* mp3tagInfo)
{
    mp3tagInfo->fptr = fopen(argv, "r");
    if (mp3tagInfo->fptr == NULL)
    {
        printf("ERROR: Unable to open the MP3 file.\n");
        return m_failure;
    }

    fseek(mp3tagInfo->fptr, 10, SEEK_SET);
    Status ret;
    const char *tags[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    const char *labels[6] = {"Title", "Artist", "Album", "Year", "Content", "Comments"};
    uint *sizes[6] = {&mp3tagInfo->title_size, &mp3tagInfo->artist_size, &mp3tagInfo->album_size, &mp3tagInfo->year_size, &mp3tagInfo->content_size, &mp3tagInfo->comment_size};
    char *data_tags[6] = {mp3tagInfo->title_tag, mp3tagInfo->artist_tag, mp3tagInfo->album_tag, mp3tagInfo->year, mp3tagInfo->content_type, mp3tagInfo->comments};

    for (int i = 0; i < 6; i++)
    {
        int count;

        count = fread(mp3tagInfo->frame_Id, 1, 4, mp3tagInfo->fptr);
        if (count < 4)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            fclose(mp3tagInfo->fptr);
            return m_failure;
        }

        mp3tagInfo->frame_Id[4] = '\0';

        if (strcmp(mp3tagInfo->frame_Id, tags[i]) == 0)
        {
            count = fread(sizes[i], 4, 1, mp3tagInfo->fptr);
            if (count < 1)
            {
                printf("ERROR: Unable to read from the MP3 file.\n");
                fclose(mp3tagInfo->fptr);
                return m_failure;
            }

            char *temp = (char *)sizes[i];
            // printf("%d\n",*sizes[i]);
            char swap;
            for (int j = 0; j < (sizeof(uint) / 2); j++)
            {
                swap = temp[j];
                temp[j] = temp[(sizeof(uint)) - j - 1];
                temp[(sizeof(uint)) - j - 1] = swap;
            }

            data_tags[i] = (char *)malloc((*sizes[i]) * sizeof(char));

            fseek(mp3tagInfo->fptr, 3, SEEK_CUR);

            count = fread(data_tags[i], 1, (*sizes[i] - 1), mp3tagInfo->fptr);
            if (count < (*sizes[i] - 1))
            {
                printf("ERROR: Unable to read from the MP3 file.\n");
                free(data_tags[i]);
                fclose(mp3tagInfo->fptr);
                return m_failure;
            }

            data_tags[i][(*sizes[i] - 1)] = '\0';
            printf("%-10s%s\n", labels[i], data_tags[i]);
            free(data_tags[i]);
            data_tags[i] = NULL;
        }
        else
        {
            printf("ERROR: %s Frame ID not found.\n", tags[i]);
            fclose(mp3tagInfo->fptr);
            return m_failure;
        }
    }

    printf("------------------------------------------------------\n");
    fclose(mp3tagInfo->fptr);

    return m_success;
}
