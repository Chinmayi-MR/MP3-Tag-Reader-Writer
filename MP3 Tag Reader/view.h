#ifndef VIEW_H
#define VIEW_H

#include "type.h"      

typedef struct _TagInfo
{
    FILE* fptr;
    char frame_Id[4];

    char* title_tag;
    uint title_size;

    char* artist_tag;
    uint artist_size;

    char* album_tag;
    uint album_size;

    char* year;
    uint year_size;

    char* content_type;
    uint content_size;

    char* comments;
    uint comment_size;

} TagInfo;

OperationType check_operation(const char* argv);

Status read_and_validate_for_view(const char* argv, TagInfo* mp3tagInfo);

Status view_and_display_tag(const char *argv, TagInfo* mp3tagInfo);

#endif
