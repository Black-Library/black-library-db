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
    DBEntry ReadStagingEntry(std::string UUID);
    DBUrlCheck DoesStagingEntryUrlExist(std::string URL);
    int UpdateStagingEntry(std::string UUID, const DBEntry &entry);
    int DeleteStagingEntry(std::string UUID);

    int CreateBlackEntry(const DBEntry &entry);
    DBEntry ReadBlackEntry(std::string UUID);
    DBUrlCheck DoesBlackEntryUrlExist(std::string URL);
    int UpdateBlackEntry(std::string UUID, const DBEntry &entry);
    int DeleteBlackEntry(std::string UUID);

private:
    std::string GetUUID();

    std::unique_ptr<BlackLibraryDBConnectionInterface> database_connection_interface_;
    std::string database_url_;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif
