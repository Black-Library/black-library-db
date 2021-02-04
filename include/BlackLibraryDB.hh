#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDB_HH__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDB_HH__

#include <memory>
#include <string>

#include <BlackLibraryDBConnectionInterface.hh>

namespace black_library {

namespace core {

namespace db {

class BlackLibraryDB {
public:
    explicit BlackLibraryDB(const std::string &database_url, bool initialize = false);
    ~BlackLibraryDB();

    int CreateStagingEntry(const DBEntry &entry);
    DBEntry ReadStagingEntry(const std::string &UUID);
    bool DoesStagingEntryUrlExist(const std::string &url);
    int UpdateStagingEntry(const std::string &UUID, const DBEntry &entry);
    int DeleteStagingEntry(const std::string &UUID);

    int CreateBlackEntry(const DBEntry &entry);
    DBEntry ReadBlackEntry(const std::string &UUID);
    bool DoesBlackEntryUrlExist(const std::string &url);
    int UpdateBlackEntry(const std::string &UUID, const DBEntry &entry);
    int DeleteBlackEntry(const std::string &UUID);

    std::string GetUUIDFromUrl(const std::string &url);
    std::string GetUrlFromUUID(const std::string &UUID);

private:
    std::string GetUUID();

    std::unique_ptr<BlackLibraryDBConnectionInterface> database_connection_interface_;
    std::string database_url_;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif
