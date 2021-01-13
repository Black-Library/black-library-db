#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDB_HH__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDB_HH__

#include <memory>
#include <string>

#include <BlackLibraryDBConnectionInterface.hh>

namespace black_library {

namespace core {

namespace db {

class BlackLibraryDB : public BlackLibraryDBConnectionInterface {
public:
    explicit BlackLibraryDB(const std::string &database_url);
    ~BlackLibraryDB();

    int CreateStagingEntry(const DBEntry &entry) const override;
    DBEntry ReadStagingEntry(std::string UUID) const override;

private:
    std::string GetUUID();

    std::unique_ptr<BlackLibraryDBConnectionInterface> database_connection_interface_;
    std::string database_url_;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif
