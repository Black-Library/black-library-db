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

namespace black_library_sqlite3 {

class SQLiteDB : public BlackLibraryDBConnectionInterface
{
public:
    explicit SQLiteDB(const std::string &database_url);
    ~SQLiteDB();

    int CreateStagingEntry(const DBEntry &entry) const override;
    // DBEntry ReadStagingDoc(std::string UUID);
    // int UpdateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    // int DeleteStagingDoc(std::string UUID);

private:
    int BeginTransaction() const;
    int EndTransaction() const;
    int ResetStatement(sqlite3_stmt *smt) const;

    int BindInt(sqlite3_stmt* stmt, const std::string &parameter_name, const int &bind_int) const;
    int BindText(sqlite3_stmt* stmt, const std::string &parameter_name, const std::string &bind_text) const;

    int PrepareStatements();

    sqlite3 *database_conn_;
    std::vector<sqlite3_stmt *> prepared_statements_;
    std::string database_url_;
};

} // black_library_sqlite3
} // namespace db
} // namespace core
} // namespace black_library

#endif