/**
 * SQLiteDB.hh
 */

#ifndef __BLACK_LIBRARY_CORE_DB_SQLITEDB_HH__
#define __BLACK_LIBRARY_CORE_DB_SQLITEDB_HH__

#include <vector>

#include <sqlite3.h>

#include <BlackLibraryDBConnectionInterface.hh>

namespace black_library {

namespace core {

namespace db {

class SQLiteDB : public BlackLibraryDBConnectionInterface
{
public:
    explicit SQLiteDB(const std::string &database_url);
    ~SQLiteDB();

    int CreateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "") const override;
    // DBEntry ReadStagingDoc(std::string UUID);
    // int UpdateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    // int DeleteStagingDoc(std::string UUID);

private:
    int PrepareStatements();

    sqlite3 *database_conn_;
    std::vector<sqlite3_stmt *> prepared_statements_;
    std::string database_url_;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif