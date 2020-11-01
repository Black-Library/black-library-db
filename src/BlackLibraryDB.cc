/**
 * BlackLibraryDB.cc
 */

#include <BlackLibraryDB.hh>
#include <SQLiteDB.hh>

namespace black_library {

namespace core {

namespace db {

BlackLibraryDB::BlackLibraryDB(const std::string &database_url) :
    database_connection_interface_(nullptr),
    database_url_(database_url)
{
    database_connection_interface_ = std::make_unique<SQLiteDB>(database_url_);
}


} // namespace db
} // namespace core
} // namespace black_library
