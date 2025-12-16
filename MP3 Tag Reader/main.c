#include <stdio.h>
#include "type.h"
#include "view.h"
#include "edit.h"


int main(int argc, char* argv[])
{
    TagInfo mp3tagInfo;
    E_TagInfo e_mp3tagInfo;
   
    if (argc == 1)	
    {
        printf ("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf ("USAGE:\n");
        printf("INFO: To view please pass like: ./a.out -v mp3filename\n");
        printf("INFO: To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
        printf("INFO: To get help pass like: ./a.out --help or -h\n");
    }
    else
    {
        OperationType ret = check_operation(argv[1]);	

        if (ret == m_view)	
        {
            printf("---------------SELECTED VIEW DETAILS-------------------\n");
            printf("-------------------------------------------------------\n");
            printf("         MP3 TAG READER AND EDITOR FOR ID3v2.3         \n");
            printf("-------------------------------------------------------\n");
            Status ret1 = read_and_validate_for_view(argv[2], &mp3tagInfo);		
            if (ret1 == m_success)	
            {
                Status ret2 = view_and_display_tag (argv[2], &mp3tagInfo);						
                if (ret2 == m_success)		
                {
                    printf ("-------------DETAILS DISPLAYED SUCCESSFULLY---------\n");
                }
            }
        }
        else if (ret == m_edit)	
        {
            printf("---------------SELECTED EDIT DETAILS-------------------\n");
            printf("-------------------------------------------------------\n");
            printf("         MP3 TAG READER AND EDITOR FOR ID3v2.3         \n");
            printf("-------------------------------------------------------\n");
            Status ret1 = read_and_validate_for_edit(argv, &e_mp3tagInfo);		
            if (ret1 == m_success)	
            {
                Status ret2 = edit_tag(argv, &e_mp3tagInfo);						
                if (ret2 == m_success)		
                {
                    printf ("-------------DETAILS EDITED SUCCESSFULLY---------\n");
                }
            } 
        }
        else if (ret == m_help)		
        {
            printf("---------------SELECTED HELP MENU-------------------\n");
            printf ("INFO: For Viewing the Tags -> ./a.out -v mp3filename\n");
            printf ("INFO: For Editing the Tags -> ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
            printf ("Modifier Functions:\n");
            printf ("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
        }
        else if (ret == m_unsupported)	
        {
            printf ("ERROR: Unsupported Operation.\n");
            printf ("USAGE: ./a.out --help or -h for Help menu.\n");
        }
    }

    return 0;
}
