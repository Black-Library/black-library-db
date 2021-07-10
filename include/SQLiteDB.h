/**
 * SQLiteDB.h
 */

#ifndef __BLACK_LIBRARY_CORE_DB_SQLITEDB_H__
#define __BLACK_LIBRARY_CORE_DB_SQLITEDB_H__

#include <vector>

#include <sqlite3.h>

#include <DBConnectionInterface.h>

namespace black_library {

namespace core {

namespace db {

class SQLiteDB : public DBConnectionInterface
{
public:
    explicit SQLiteDB(const std::string &database_url, bool first_time_setup);
    ~SQLiteDB();

    std::vector<DBEntry> ListEntries(entry_table_rep_t entry_type) const;
    std::vector<ErrorEntry> ListErrorEntries() const;

    int CreateUser(const DBUser &user) const;
    int CreateEntryType(const std::string &entry_type_name) const;
    int CreateSubtype(DBEntryMediaSubtype media_subtype, DBEntryMediaType media_type) const;
    int CreateSource(const DBSource &source) const;

    int CreateEntry(const DBEntry &entry, entry_table_rep_t entry_type) const override;
    DBEntry ReadEntry(const std::string &uuid, entry_table_rep_t entry_type) const override;
    int UpdateEntry(const DBEntry &entry, entry_table_rep_t entry_type) const override;
    int DeleteEntry(const std::string &uuid, entry_table_rep_t entry_type) const override;

    int CreateErrorEntry(const ErrorEntry &entry) const;

    DBBoolResult DoesEntryUrlExist(const std::string &url, entry_table_rep_t entry_type) const override;
    DBBoolResult DoesEntryUUIDExist(const std::string &uuid, entry_table_rep_t entry_type) const override;
    DBBoolResult DoesErrorEntryExist(const std::string &uuid, size_t progress_num) const;

    DBStringResult GetEntryUUIDFromUrl(const std::string &url, entry_table_rep_t entry_type) const;
    DBStringResult GetEntryUrlFromUUID(const std::string &uuid, entry_table_rep_t entry_type) const;

    bool IsReady() const;

private:
    int GenerateTables();

    int SetupDefaultTables();
    int SetupDefaultEntryTypeTable();
    int SetupDefaultSubtypeTable();
    int SetupDefaultSourceTable();

    int PrepareStatements();
    int SetupDefaultBlackLibraryUsers();

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
    bool initialized_;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif
