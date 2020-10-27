/**
 * SQLiteDB.hh
 */

#ifndef __LIBRARYCORE_SQLITEDB_HH__
#define __LIBRARYCORE_SQLITEDB_HH__

#include <sqlite3.h>

#include <BlackLibraryDBConnectionInterface.hh>

namespace librarycore {

namespace librarydb {

class SQLiteDB : BlackLibraryDBConnectionInterface
{
public:
    explicit SQLiteDB();
    ~SQLiteDB();

    int CreateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    DBEntry ReadStagingDoc(std::string UUID);
    int UpdateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    int DeleteStagingDoc(std::string UUID);

private:
    sqlite3 *db_conn_;
    std::string uri_;
};

} // namespace librarydb
} // namespace librarycore

#endif