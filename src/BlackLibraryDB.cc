/**
 * BlackLibraryDB.cc
 */

#include <iostream>

#include <BlackLibraryDB.hh>
#include <SQLiteDB.hh>

namespace black_library {

namespace core {

namespace db {

BlackLibraryDB::BlackLibraryDB(const std::string &database_url, bool initialize) :
    database_connection_interface_(nullptr),
    database_url_(database_url)
{
    database_connection_interface_ = std::make_unique<black_library_sqlite3::SQLiteDB>(database_url_, initialize);
}

BlackLibraryDB::~BlackLibraryDB()
{
}

int BlackLibraryDB::CreateStagingEntry(const DBEntry &entry) const
{
    if (database_connection_interface_->CreateStagingEntry(entry))
    {
        std::cout << "Error, failed to create staging doc" << std::endl;
        return -1;
    }

    return 0;
}

DBEntry BlackLibraryDB::ReadStagingEntry(std::string UUID) const
{
    DBEntry entry;

    entry = database_connection_interface_->ReadStagingEntry(UUID);
    if (entry.UUID.empty())
    {
        std::cout << "Error, failed to read staging doc" << std::endl;
        return entry;
    }

    return entry;
}


} // namespace db
} // namespace core
} // namespace black_library
