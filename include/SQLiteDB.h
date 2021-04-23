/**
 * SQLiteDB.h
 */

#ifndef __BLACK_LIBRARY_CORE_DB_SQLITEDB_H__
#define __BLACK_LIBRARY_CORE_DB_SQLITEDB_H__

#include <vector>

#include <sqlite3.h>

#include <BlackLibraryDBConnectionInterface.h>

namespace black_library {

namespace core {

namespace db {

namespace black_library_sqlite3 {

class SQLiteDB : public BlackLibraryDBConnectionInterface
{
public:
    explicit SQLiteDB(const std::string &database_url, const bool first_time_setup);
    ~SQLiteDB();

    int CreateUser(const DBUser &user) const;
    int CreateEntryType(const std::string &entry_type_name) const;
    int CreateSubtype(const std::string &subtype_name, db_entry_media_type_rep_t entry_type) const;
    int CreateSource(const DBSource &source) const;

    int CreateEntry(const DBEntry &entry, db_entry_type_rep_t entry_type) const override;
    DBEntry ReadEntry(const std::string &UUID, db_entry_type_rep_t entry_type) const override;
    int UpdateEntry(const std::string &UUID, const DBEntry &entry, db_entry_type_rep_t entry_type) const override;
    int DeleteEntry(const std::string &UUID, db_entry_type_rep_t entry_type) const override;

    DBBoolResult DoesEntryUrlExist(const std::string &url, db_entry_type_rep_t entry_type) const override;
    DBBoolResult DoesEntryUUIDExist(const std::string &UUID, db_entry_type_rep_t entry_type) const override;

    DBStringResult GetEntryUUIDFromUrl(const std::string &url, db_entry_type_rep_t entry_type) const;
    DBStringResult GetEntryUrlFromUUID(const std::string &UUID, db_entry_type_rep_t entry_type) const;

    bool IsReady() const;

private:
    int SetupTables();
    int SetupDefaultEntryTypeTable();
    int SetupDefaultSubtypeTable();
    int SetupDefaultSourceTable();

    int PrepareStatements();
    int SetupDefaultBlackLibraryUsers();
    int SetupDefaultSubtypes();

    int BeginTransaction() const;
    int CheckInitialized() const;
    int EndTransaction() const;
    int GenerateTable(const std::string &sql);
    int PrepareStatement(const std::string &statement, int statement_id);
    int ResetStatement(sqlite3_stmt *smt) const;

    int BindInt(sqlite3_stmt* stmt, const std::string &parameter_name, const int &bind_int) const;
    int BindText(sqlite3_stmt* stmt, const std::string &parameter_name, const std::string &bind_text) const;

    sqlite3 *database_conn_;
    std::vector<sqlite3_stmt *> prepared_statements_;
    std::string database_url_;
    bool first_time_setup_;
    bool intialized_;
};

} // namespace black_library_sqlite3
} // namespace db
} // namespace core
} // namespace black_library

#endif
