/**
 * DBConnectionInterfaceUtils.h
 */

#ifndef __BLACK_LIBRARY_CORE_DB_DBCONNECTIONINTERFACEUTILS_H__
#define __BLACK_LIBRARY_CORE_DB_DBCONNECTIONINTERFACEUTILS_H__

#include <DBConnectionInterface.h>

namespace black_library {

namespace core {

namespace db {

std::string GetEntryTypeString(entry_table_rep_t type)
{
    switch (type)
    {
    case BLACK_ENTRY:
        return "black_entry";
    case STAGING_ENTRY:
        return "staging_entry";
    default:
        return "error";
    }
}

std::string GetMediaTypeString(entry_media_rep_t media_type)
{
    switch (media_type)
    {
    case DB_ENTRY_MEDIA_ERROR:
        return "error";
    case DOCUMENT:
        return "document";
    case IMAGE_GALLERY:
        return "image-gallery";
    case VIDEO:
        return "video";
    default:
        return "unknown error";
    }
}

} // namespace db
} // namespace core
} // namespace black_library

#endif
