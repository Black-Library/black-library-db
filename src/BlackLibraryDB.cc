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

int BlackLibraryDB::CreateStagingEntry(const DBEntry &entry)
{
    if (database_connection_interface_->CreateEntry(entry, STAGING_ENTRY))
    {
        std::cout << "Error, failed to create staging entry" << std::endl;
        return -1;
    }

    return 0;
}

DBEntry BlackLibraryDB::ReadStagingEntry(std::string UUID)
{
    DBEntry entry;

    entry = database_connection_interface_->ReadEntry(UUID, STAGING_ENTRY);
    if (entry.UUID.empty())
    {
        std::cout << "Error, failed to read staging entry" << std::endl;
        return entry;
    }

    return entry;
}

DBUrlCheck BlackLibraryDB::DoesStagingEntryUrlExist(std::string URL)
{
    return database_connection_interface_->DoesEntryUrlExist(URL, STAGING_ENTRY);
}

int BlackLibraryDB::UpdateStagingEntry(std::string UUID, const DBEntry &entry)
{
    if (database_connection_interface_->UpdateEntry(UUID, entry, STAGING_ENTRY))
    {
        std::cout << "Error, failed to update staging entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::DeleteStagingEntry(std::string UUID)
{
    if (database_connection_interface_->DeleteEntry(UUID, STAGING_ENTRY))
    {
        std::cout << "Error, failed to delete staging entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::CreateBlackEntry(const DBEntry &entry)
{
    if (database_connection_interface_->CreateEntry(entry, BLACK_ENTRY))
    {
        std::cout << "Error, failed to create black entry" << std::endl;
        return -1;
    }

    return 0;
}

DBEntry BlackLibraryDB::ReadBlackEntry(std::string UUID)
{
    DBEntry entry;

    entry = database_connection_interface_->ReadEntry(UUID, BLACK_ENTRY);
    if (entry.UUID.empty())
    {
        std::cout << "Error, failed to read black entry" << std::endl;
        return entry;
    }

    return entry;
}

DBUrlCheck BlackLibraryDB::DoesBlackEntryUrlExist(std::string URL)
{
    return database_connection_interface_->DoesEntryUrlExist(URL, BLACK_ENTRY);
}

int BlackLibraryDB::UpdateBlackEntry(std::string UUID, const DBEntry &entry)
{
    if (database_connection_interface_->UpdateEntry(UUID, entry, BLACK_ENTRY))
    {
        std::cout << "Error, failed to update black entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::DeleteBlackEntry(std::string UUID)
{
    if (database_connection_interface_->DeleteEntry(UUID, BLACK_ENTRY))
    {
        std::cout << "Error, failed to delete black entry" << std::endl;
        return -1;
    }

    return 0;
}

} // namespace db
} // namespace core
} // namespace black_library
