/**
 * BlackLibraryDBConnectionInterfaceUtils.h
 */

#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBCONNECTIONINTERFACEUTILS_H__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBCONNECTIONINTERFACEUTILS_H__

#include <BlackLibraryDBConnectionInterface.h>

namespace black_library {

namespace core {

namespace db {

std::string GetEntryTypeString(db_entry_type_rep_t type)
{
    switch (type)
    {
    case BLACK_ENTRY:
        return "black_entry";
    case STAGING_ENTRY:
        return "staging_entry";
    default:
        return "Error";
    }
}

std::string GetMediaTypeString(db_entry_media_type_rep_t type)
{
    switch (type)
    {
    case DOCUMENT:
        return "document";
    case IMAGE_GALLERY:
        return "image-gallery";
    case VIDEO:
        return "video";
    default:
        return "Error";
    }
}

} // namespace db
} // namespace core
} // namespace black_library

#endif
