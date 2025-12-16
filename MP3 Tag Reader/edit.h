#ifndef EDIT_H
#define EDIT_H

#include "type.h"      

typedef struct TagInfo
{
    FILE* e_fptr;
    char e_frame_Id [5];

    FILE *temp_fptr;
    char frame_Id_value[100];
    uint frame_Id_size;

} E_TagInfo;

Status read_and_validate_for_edit(char* argv[], E_TagInfo* e_mp3tagInfo);

Status edit_tag(char* argv[], E_TagInfo* e_mp3tagInfo);

Status copy_data(E_TagInfo* e_mp3tagData, int flag);

Status copy_remaining_data(E_TagInfo* e_mp3tagData);

Status copy_to_src_file(E_TagInfo* e_mp3tagInfo);

#endif
