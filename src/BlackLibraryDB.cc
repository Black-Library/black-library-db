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

DBEntry BlackLibraryDB::ReadStagingEntry(const std::string &UUID)
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

bool BlackLibraryDB::DoesStagingEntryUrlExist(const std::string &url)
{
    DBUrlCheck check = database_connection_interface_->DoesEntryUrlExist(url, STAGING_ENTRY);
    
    if (check.error)
        return false;

    return check.exists;
}

int BlackLibraryDB::UpdateStagingEntry(const std::string &UUID, const DBEntry &entry)
{
    if (database_connection_interface_->UpdateEntry(UUID, entry, STAGING_ENTRY))
    {
        std::cout << "Error, failed to update staging entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::DeleteStagingEntry(const std::string &UUID)
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

DBEntry BlackLibraryDB::ReadBlackEntry(const std::string &UUID)
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

bool BlackLibraryDB::DoesBlackEntryUrlExist(const std::string &url)
{
    DBUrlCheck check = database_connection_interface_->DoesEntryUrlExist(url, BLACK_ENTRY);
    
    if (check.error)
        return false;

    return check.exists;
}

int BlackLibraryDB::UpdateBlackEntry(const std::string &UUID, const DBEntry &entry)
{
    if (database_connection_interface_->UpdateEntry(UUID, entry, BLACK_ENTRY))
    {
        std::cout << "Error, failed to update black entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::DeleteBlackEntry(const std::string &UUID)
{
    if (database_connection_interface_->DeleteEntry(UUID, BLACK_ENTRY))
    {
        std::cout << "Error, failed to delete black entry" << std::endl;
        return -1;
    }

    return 0;
}

std::string BlackLibraryDB::GetUUIDFromUrl(const std::string &url)
{
    return database_connection_interface_->GetUUIDFromUrl(url);
}

std::string BlackLibraryDB::GetUrlFromUUID(const std::string &UUID)
{
    return database_connection_interface_->GetUrlFromUUID(UUID);
}

} // namespace db
} // namespace core
} // namespace black_library
