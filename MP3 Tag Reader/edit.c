#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type.h"
#include "edit.h"

Status read_and_validate_for_edit(char* argv[], E_TagInfo* e_mp3tagInfo)
{
    if(argv[2] == NULL)
    {
        printf("INFO: To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
        printf("INFO: Modifier Functions:\n");
        printf("-t\tModify Title Tag\n-a\tModify Artist Tag\n-A\tModify Album Tag\n-y\tModify Year Tag\n-m\tModify Content Type Tag\n-c\tModify Comments Tag\n");
        return m_failure;
    }
    else
    {
        if(strcmp(argv[2], "-t") == 0)
        {
            strcpy(e_mp3tagInfo->e_frame_Id, "TIT2");
        }
        else if(strcmp(argv[2], "-a") == 0)
        {
            strcpy(e_mp3tagInfo->e_frame_Id, "TPE1");
        }
        else if(strcmp(argv[2], "-A") == 0)
        {
            strcpy(e_mp3tagInfo->e_frame_Id, "TALB");
        }
        else if(strcmp(argv[2], "-y") == 0)
        {
            strcpy(e_mp3tagInfo->e_frame_Id, "TYER");
        }
        else if(strcmp(argv[2], "-m") == 0)
        {
            strcpy(e_mp3tagInfo->e_frame_Id, "TCON");
        }
        else if(strcmp(argv[2], "-c") == 0)
        {
            strcpy(e_mp3tagInfo->e_frame_Id, "COMM");
        }
        else
        {
            printf ("ERROR: Unsupported Modifier.\n");
            printf ("INFO: Modifier Functions:\n");
            printf ("-t\tModify Title Tag\n-a\tModify Artist Tag\n-A\tModify Album Tag\n-y\tModify Year Tag\n-m\tModify Content Type Tag\n-c\tModify Comments Tag\n");
            return m_failure;
        }
        
        printf("Frame ID to be changed: %s.\n", e_mp3tagInfo->e_frame_Id);
    }
    if(argv[3] == NULL)
    {
        printf("ERROR: Missing tag value. Please provide a value to be updated.\n");
        return m_failure;
    }

    strcpy(e_mp3tagInfo->frame_Id_value, argv[3]); 
    e_mp3tagInfo->frame_Id_size = strlen(argv[3]) + 1;

    if(argv[4] == NULL)
    {
        printf("INFO: To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
        return m_failure;
    }
    else
    {
        e_mp3tagInfo->e_fptr = fopen(argv[4], "r");
        if (e_mp3tagInfo->e_fptr != NULL)
        {
            char str[3];
            fread(str, 1, 3, e_mp3tagInfo->e_fptr);
            if(strcmp(str, "ID3") == 0)
            {
                printf("-------------------Version ID: v2.3--------------------\n");
                printf("-------------------------------------------------------\n");
            }
            else
            {
                printf ("ERROR: The file is not matching with that of a '.mp3' file.\n");
                printf("INFO: To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
                return m_failure;
            }
        }
        else
        {
            printf ("ERROR: Unable to open the %s file.\n", argv[4]);
            printf("INFO: To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
            return m_failure;
        }

        fclose(e_mp3tagInfo->e_fptr);
    }

    return m_success;
}

Status edit_tag(char* argv[], E_TagInfo* e_mp3tagInfo)
{
    e_mp3tagInfo->e_fptr = fopen(argv[4], "r");
    e_mp3tagInfo->temp_fptr = fopen("temp.mp3", "w");

    if(!e_mp3tagInfo->e_fptr || !e_mp3tagInfo->temp_fptr)
    {
        printf("ERROR: Unable to open MP3 file or temporary file.\n");
        return m_failure;
    }

    char str[10];
    if(fread(str, 1, 10, e_mp3tagInfo->e_fptr) < 10)
    {
        printf("ERROR: Unable to read from the MP3 file.\n");
        return m_failure;
    }
    fwrite(str, 1, 10, e_mp3tagInfo->temp_fptr);

    
    for(int i = 0; i < 6; i++)
    {
        if(fread(str, 1, 4, e_mp3tagInfo->e_fptr) < 4)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return m_failure;
        }
        fwrite(str, 1, 4, e_mp3tagInfo->temp_fptr);
        str[4]='\0';
       
        if(strcmp(e_mp3tagInfo->e_frame_Id, str) == 0)
        {

            if(copy_data(e_mp3tagInfo, 1) == m_failure)
            {
                printf("ERROR: Unable to copy updated tag data.\n");
                return m_failure;
            }
            break;
        }
        else 
        {
            if(copy_data(e_mp3tagInfo, 0) == m_failure) 
            {
                printf("ERROR: Unable to copy remaining tag data.\n");
                return m_failure;
            }
        }
    }
    if(copy_remaining_data(e_mp3tagInfo) == m_failure) 
    {
        printf("ERROR: Unable to copy remaining data.\n");
        return m_failure;
    }

    fclose(e_mp3tagInfo->e_fptr);
    fclose(e_mp3tagInfo->temp_fptr);

    e_mp3tagInfo->e_fptr = fopen(argv[4], "w");
    e_mp3tagInfo->temp_fptr = fopen("temp.mp3", "r");

    if(!e_mp3tagInfo->e_fptr || !e_mp3tagInfo->temp_fptr)
    {
        printf("ERROR: Unable to open MP3 file or temporary file.\n");
        return m_failure;
    }

    if(copy_to_src_file(e_mp3tagInfo) == m_failure)
    {
        printf("ERROR: Unable to copy the data to source file\n");
        return m_failure;
    }

    if(remove("temp.mp3") == 0)
    {
        printf("Temporary file deleted successfully\n");
    }
    else
    {
        printf("Error deleting file\n");
    }

    fclose(e_mp3tagInfo->e_fptr);
    fclose(e_mp3tagInfo->temp_fptr);
    return m_success;
}

Status copy_data(E_TagInfo* e_mp3tagInfo, int flag)
{
    uint size;
    if(fread(&size, 4, 1, e_mp3tagInfo->e_fptr) < 1) 
    {
        printf("ERROR: Unable to read size from MP3 file.\n");
        return m_failure;
    }
    printf("%ld----%ld\n",ftell(e_mp3tagInfo->e_fptr),ftell(e_mp3tagInfo->temp_fptr));
    printf("big:%d\n\n",size);
    int temp_size = size;
    char* pos = (char*) &size;
    for(int i = 0; i < sizeof(uint) / 2; i++) 
    {
        char temp = pos[i];
        pos[i] = pos[sizeof(uint) - i - 1];
        pos[sizeof(uint) - i - 1] = temp;
    }

    
    printf("little:%d %d %d\n\n",size,temp_size,e_mp3tagInfo->frame_Id_size);
    if(flag == 1) 
    {
        uint new_size = e_mp3tagInfo->frame_Id_size;
        pos = (char*)&new_size;
        for(int i = 0; i < sizeof(uint) / 2; i++) 
        {
            char temp = pos[i];
            pos[i] = pos[sizeof(uint) - i - 1];
            pos[sizeof(uint) - i - 1] = temp;
        }
        
        fwrite(&new_size, 4, 1, e_mp3tagInfo->temp_fptr);
        
        char flag[3];
        fread(flag, 1, 3, e_mp3tagInfo->e_fptr);
        fwrite(flag, 1, 3, e_mp3tagInfo->temp_fptr);
        
        fwrite(e_mp3tagInfo->frame_Id_value, 1, e_mp3tagInfo->frame_Id_size - 1, e_mp3tagInfo->temp_fptr);
        fseek(e_mp3tagInfo->e_fptr, size - 1, SEEK_CUR);
    } 
    else 
    {
        fwrite(&temp_size, 4, 1, e_mp3tagInfo->temp_fptr);
        char flag[3];
        fread(flag, 1, 3, e_mp3tagInfo->e_fptr);
        fwrite(flag, 1, 3, e_mp3tagInfo->temp_fptr);

        char* data = (char*) malloc(size - 1);
        fread(data, 1, size - 1, e_mp3tagInfo->e_fptr);
        fwrite(data, 1, size - 1, e_mp3tagInfo->temp_fptr);
        free(data);
    }

    return m_success;
}

Status copy_remaining_data(E_TagInfo* e_mp3tagInfo) 
{
   
    char count;
    while((count = fgetc(e_mp3tagInfo->e_fptr)) != EOF) 
    {
        fputc(count, e_mp3tagInfo->temp_fptr);
    }
    return m_success;
}

Status copy_to_src_file(E_TagInfo* e_mp3tagInfo) 
{
    char buffer[1024];
    uint count;
    while((count = fread(buffer, 1, sizeof(buffer), e_mp3tagInfo->temp_fptr)) > 0) 
    {
        fwrite(buffer, 1, count, e_mp3tagInfo->e_fptr);
    }
    return m_success;
}