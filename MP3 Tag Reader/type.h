#ifndef TYPE_H
#define TYPE_H

typedef unsigned int uint;

typedef enum
{
    m_success,
    m_failure
} Status;

typedef enum
{
    m_view,
    m_edit,
    m_help,
    m_unsupported
} OperationType;

#endif
