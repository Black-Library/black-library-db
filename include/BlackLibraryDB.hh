#ifndef BLACKLIBRARYDB_HH_
#define BLACKLIBRARYDB_HH_

#include <string>

#include <sqlite3.h>

namespace librarycore {

class BlackLibraryDB {
public:
    explicit BlackLibraryDB(const std::string &database_path);
    ~BlackLibraryDB();
private:
    sqlite3 *database_connection_;
};

} // namespace librarycore

#endif
