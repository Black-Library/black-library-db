/**
 * BlackLibraryDB.cc
 */

#include <iostream>

#include <BlackLibraryDB.h>
#include <SQLiteDB.h>

namespace black_library {

namespace core {

namespace db {

BlackLibraryDB::BlackLibraryDB(const std::string &database_url, bool initialize) :
    database_connection_interface_(nullptr),
    mutex_(),
    database_url_(database_url)
{
    database_connection_interface_ = std::make_unique<SQLiteDB>(database_url_, initialize);
}

BlackLibraryDB::~BlackLibraryDB()
{
}

std::vector<DBEntry> BlackLibraryDB::GetStagingEntryList()
{
    const std::lock_guard<std::mutex> lock(mutex_);

    auto entry_list = database_connection_interface_->ListEntries(STAGING_ENTRY);

    return entry_list;
}

std::vector<DBEntry> BlackLibraryDB::GetBlackEntryList()
{
    const std::lock_guard<std::mutex> lock(mutex_);

    auto entry_list = database_connection_interface_->ListEntries(BLACK_ENTRY);

    return entry_list;
}

std::vector<ErrorEntry> BlackLibraryDB::GetErrorEntryList()
{
    const std::lock_guard<std::mutex> lock(mutex_);

    auto entry_list = database_connection_interface_->ListErrorEntries();

    return entry_list;
}

int BlackLibraryDB::CreateStagingEntry(const DBEntry &entry)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->CreateEntry(entry, STAGING_ENTRY))
    {
        std::cout << "Error: failed to create staging entry" << std::endl;
        return -1;
    }

    return 0;
}

DBEntry BlackLibraryDB::ReadStagingEntry(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBEntry entry;

    entry = database_connection_interface_->ReadEntry(uuid, STAGING_ENTRY);
    if (entry.uuid.empty())
    {
        std::cout << "Error: failed to read staging entry" << std::endl;
        return entry;
    }

    return entry;
}

int BlackLibraryDB::UpdateStagingEntry(const DBEntry &entry)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->UpdateEntry(entry, STAGING_ENTRY))
    {
        std::cout << "Error: failed to update staging entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::DeleteStagingEntry(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->DeleteEntry(uuid, STAGING_ENTRY))
    {
        std::cout << "Error: failed to delete staging entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::CreateBlackEntry(const DBEntry &entry)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->CreateEntry(entry, BLACK_ENTRY))
    {
        std::cout << "Error: failed to create black entry" << std::endl;
        return -1;
    }

    return 0;
}

DBEntry BlackLibraryDB::ReadBlackEntry(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBEntry entry;

    entry = database_connection_interface_->ReadEntry(uuid, BLACK_ENTRY);
    if (entry.uuid.empty())
    {
        std::cout << "Error: failed to read black entry" << std::endl;
        return entry;
    }

    return entry;
}

int BlackLibraryDB::UpdateBlackEntry(const DBEntry &entry)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->UpdateEntry(entry, BLACK_ENTRY))
    {
        std::cout << "Error: failed to update black entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::DeleteBlackEntry(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->DeleteEntry(uuid, BLACK_ENTRY))
    {
        std::cout << "Error: failed to delete black entry" << std::endl;
        return -1;
    }

    return 0;
}

int BlackLibraryDB::CreateErrorEntry(const ErrorEntry &entry)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    if (database_connection_interface_->CreateErrorEntry(entry))
    {
        std::cout << "Error: failed to create error entry" << std::endl;
        return -1;
    }

    return 0;
}

bool BlackLibraryDB::DoesStagingEntryUrlExist(const std::string &url)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBBoolResult check = database_connection_interface_->DoesEntryUrlExist(url, STAGING_ENTRY);
    
    if (check.error != 0)
    {
        std::cout << "Error: database returned " << check.error << std::endl;
        return false;
    }

    return check.result;
}

bool BlackLibraryDB::DoesBlackEntryUrlExist(const std::string &url)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBBoolResult check = database_connection_interface_->DoesEntryUrlExist(url, BLACK_ENTRY);
    
    if (check.error != 0)
    {
        std::cout << "Error: database returned " << check.error << std::endl;
        return false;
    }

    return check.result;
}

bool BlackLibraryDB::DoesStagingEntryUUIDExist(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBBoolResult check = database_connection_interface_->DoesEntryUUIDExist(uuid, STAGING_ENTRY);
    
    if (check.error != 0)
    {
        std::cout << "Error: database returned " << check.error << std::endl;
        return false;
    }

    return check.result;
}

bool BlackLibraryDB::DoesBlackEntryUUIDExist(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBBoolResult check = database_connection_interface_->DoesEntryUUIDExist(uuid, BLACK_ENTRY);
    
    if (check.error != 0)
    {
        std::cout << "Error: database returned " << check.error << std::endl;
        return false;
    }

    return check.result;
}

bool BlackLibraryDB::DoesErrorEntryExist(const std::string &uuid, size_t progress_num)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBBoolResult check = database_connection_interface_->DoesErrorEntryExist(uuid, progress_num);
    
    if (check.error != 0)
    {
        std::cout << "Error: database returned " << check.error << std::endl;
        return false;
    }

    return check.result;
}

DBStringResult BlackLibraryDB::GetStagingEntryUUIDFromUrl(const std::string &url)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBStringResult res = database_connection_interface_->GetEntryUUIDFromUrl(url, STAGING_ENTRY);
    if (res.error)
        std::cout << "Error: failed to get staging UUID from url" << std::endl;

    return res;
}

DBStringResult BlackLibraryDB::GetStagingEntryUrlFromUUID(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    return database_connection_interface_->GetEntryUrlFromUUID(uuid, STAGING_ENTRY);
}

DBStringResult BlackLibraryDB::GetBlackEntryUUIDFromUrl(const std::string &url)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    DBStringResult res = database_connection_interface_->GetEntryUUIDFromUrl(url, BLACK_ENTRY);
    if (res.error)
        std::cout << "Error: failed to get black UUID from url" << std::endl;

    return res;
}

DBStringResult BlackLibraryDB::GetBlackEntryUrlFromUUID(const std::string &uuid)
{
    const std::lock_guard<std::mutex> lock(mutex_);

    return database_connection_interface_->GetEntryUrlFromUUID(uuid, BLACK_ENTRY);
}

bool BlackLibraryDB::IsReady()
{
    const std::lock_guard<std::mutex> lock(mutex_);

    return database_connection_interface_->IsReady();
}

} // namespace db
} // namespace core
} // namespace black_library
