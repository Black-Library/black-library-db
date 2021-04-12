/**
 * BlackLibraryDB.h
 */

#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDB_H__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDB_H__

#include <memory>
#include <string>

#include <BlackLibraryDBConnectionInterface.h>

namespace black_library {

namespace core {

namespace db {

class BlackLibraryDB {
public:
    explicit BlackLibraryDB(const std::string &database_url, bool initialize = false);
    ~BlackLibraryDB();

    int CreateStagingEntry(const DBEntry &entry);
    DBEntry ReadStagingEntry(const std::string &UUID);
    int UpdateStagingEntry(const std::string &UUID, const DBEntry &entry);
    int DeleteStagingEntry(const std::string &UUID);

    int CreateBlackEntry(const DBEntry &entry);
    DBEntry ReadBlackEntry(const std::string &UUID);
    int UpdateBlackEntry(const std::string &UUID, const DBEntry &entry);
    int DeleteBlackEntry(const std::string &UUID);

    bool DoesStagingEntryUrlExist(const std::string &url);
    bool DoesBlackEntryUrlExist(const std::string &url);

    bool DoesStagingEntryUUIDExist(const std::string &UUID);
    bool DoesBlackEntryUUIDExist(const std::string &UUID);

    DBStringResult GetStagingEntryUUIDFromUrl(const std::string &url);
    DBStringResult GetStagingEntryUrlFromUUID(const std::string &UUID);

    DBStringResult GetBlackEntryUUIDFromUrl(const std::string &url);
    DBStringResult GetBlackEntryUrlFromUUID(const std::string &UUID);

    bool IsReady();

private:
    std::string GetUUID();

    std::unique_ptr<BlackLibraryDBConnectionInterface> database_connection_interface_;
    std::string database_url_;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif
