#ifndef __LIBRARYCORE_BLACKLIBRARYDB_HH__
#define __LIBRARYCORE_BLACKLIBRARYDB_HH__

#include <string>

#include <BlackLibraryDBConnectionInterface.hh>

namespace librarycore {

namespace librarydb {

class BlackLibraryDB {
public:
    explicit BlackLibraryDB(const std::string &database_path);
    ~BlackLibraryDB();

private:
    std::string GetUUID();

    BlackLibraryDBConnectionInterface db_connection_interface_;
};

} // namespace librarydb
} // namespace librarycore

#endif
