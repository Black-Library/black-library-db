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

std::string GetMediaTypeString(DBEntryMediaType media_type)
{
    switch (media_type)
    {
    case DBEntryMediaType::DBEntryMediaTypeError:
        return "error";
    case DBEntryMediaType::Document:
        return "document";
    case DBEntryMediaType::ImageGallery:
        return "image-gallery";
    case DBEntryMediaType::Video:
        return "video";
    default:
        return "unknown error";
    }
}

} // namespace db
} // namespace core
} // namespace black_library

#endif
