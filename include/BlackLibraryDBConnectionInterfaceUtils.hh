/**
 * BlackLibraryDBConnectionInterfaceUtils.hh
 */

#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBCONNECTIONINTERFACEUTILS_HH__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBCONNECTIONINTERFACEUTILS_HH__

#include <BlackLibraryDBConnectionInterface.hh>

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

} // namespace db
} // namespace core
} // namespace black_library

#endif
