#ifndef __LIBRARYCORE_BLACKLIBRARYDB_HH__
#define __LIBRARYCORE_BLACKLIBRARYDB_HH__

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
