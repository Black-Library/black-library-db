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
    explicit SQLiteDB(const std::string &database_url, const bool first_time_setup);
    ~SQLiteDB();

    int CreateUser(const DBUser &user) const;
    int CreateEntryType(const std::string &entry_type) const;
    int CreateDocumentSubtype(const std::string &document_subtype) const;
    int CreateImageGallerySubtype(const std::string &image_gallery_subtype) const;
    int CreateVideoSubtype(const std::string &video_subtype) const;

    int CreateEntry(const DBEntry &entry, db_entry_type_rep_t entry_type) const override;
    DBEntry ReadEntry(const std::string &UUID, db_entry_type_rep_t entry_type) const override;
    DBUrlCheck DoesEntryUrlExist(const std::string &URL, db_entry_type_rep_t entry_type) const override;
    int UpdateEntry(const std::string &UUID, const DBEntry &entry, db_entry_type_rep_t entry_type) const override;
    int DeleteEntry(const std::string &UUID, db_entry_type_rep_t entry_type) const override;

private:
    int SetupTables();
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
