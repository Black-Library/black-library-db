/**
 * SQLiteDB.cc
 */

#include <iostream>

#include <SQLiteDB.hh>

namespace black_library {

namespace core {

namespace db {

namespace black_library_sqlite3 {

static constexpr const char CreateStagingEntryStatement[] = "INSERT INTO staging_entry(UUID, title, nickname, source, URL, version, media_path, birth_date, user_contributed) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
static constexpr const char TestStatement[] = "INSERT INTO MyTable(ID, NAME) VALUES (?, ?)";

typedef enum {
    CREATE_STAGING_DOC_STATEMENT,
    TEST_STATEMENT,
    _NUM_PREPARED_STATEMENTS
} prepared_statement_id_t;

SQLiteDB::SQLiteDB(const std::string &database_url) :
    database_conn_(),
    prepared_statements_(),
    database_url_(database_url)
{
    if (database_url_.empty())
        database_url_ = "/mnt/db/catalog.db";

    int res = sqlite3_open(database_url_.c_str(), &database_conn_);
    
    if (res != SQLITE_OK)
    {
        std::cout << "Error, failed to open db at: " << database_url_ << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return;
    }

    std::cout << "Open database at: " << database_url_ << std::endl;

    prepared_statements_.reserve(_NUM_PREPARED_STATEMENTS);

    PrepareStatements();
}

SQLiteDB::~SQLiteDB()
{
    if (database_conn_)
    {
        for (size_t i = 0; i < prepared_statements_.size(); ++i)
        {
            sqlite3_finalize(prepared_statements_[i]);
        }
        sqlite3_close(database_conn_);
    }
}

int SQLiteDB::CreateStagingEntry(const DBEntry &entry) const
{
    std::cout << "create staging doc for UUID: " << entry.UUID << std::endl;

    if (BeginTransaction())
        return -1;

    int ret = SQLITE_OK;
    int statement_id = CREATE_STAGING_DOC_STATEMENT;

    std::cout << "test1" << std::endl;

    const char *sql1 = "INSERT INTO MyTable(ID, Name) VALUES (6, 'Robert')";
    char *error_msg = 0;
    ret = sqlite3_exec(database_conn_, sql1, 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, exec failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    std::cout << "test2" << std::endl;
    const char *sql2 = "INSERT INTO MyTable(ID, Name) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int err;

    err = sqlite3_prepare_v2(database_conn_, sql2, -1, &stmt, nullptr);
    if (err != SQLITE_OK) {
        printf("prepare failed: %s\n", sqlite3_errmsg(database_conn_));
        return -1;
    }
    std::cout << sqlite3_sql(stmt) << std::endl;

    ret = sqlite3_bind_int(stmt, 1, 3);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, bind of ID failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }
    ret = sqlite3_bind_text(stmt, 2, "timmy", -1, SQLITE_STATIC);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, bind of Name failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }

    ret = sqlite3_step(stmt);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, step failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }

    std::cout << "prep" << std::endl;
    stmt = prepared_statements_[statement_id];
    std::cout << sqlite3_sql(stmt) << std::endl;

    if (BindText(prepared_statements_[statement_id], "UUID", entry.UUID))
        return -1;
    if (BindText(prepared_statements_[statement_id], "title", entry.title))
        return -1;
    if (BindText(prepared_statements_[statement_id], "nickname", entry.nickname))
        return -1;
    if (BindText(prepared_statements_[statement_id], "source", entry.source))
        return -1;
    if (BindText(prepared_statements_[statement_id], "URL", entry.URL))
        return -1;
    if (BindInt(prepared_statements_[statement_id], "version", entry.version))
        return -1;
    if (BindText(prepared_statements_[statement_id], "URL", entry.URL))
        return -1;
    if (BindText(prepared_statements_[statement_id], "URL", entry.URL))
        return -1;
    if (BindInt(prepared_statements_[statement_id], "user_contributed", entry.user_contributed))
        return -1;

    std::cout << "prep" << std::endl;

    std::cout << sqlite3_sql(prepared_statements_[statement_id]) << std::endl;

    // run statement
    ret = sqlite3_step(prepared_statements_[statement_id]);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, insert staging doc failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    ResetStatement(prepared_statements_[statement_id]);

    if (EndTransaction())
        return -1;

    return 0;
}

// DBEntry SQLiteDB::ReadStagingDoc(std::string UUID)
// {
//     DBEntry entry;

//     return entry;
// }

// int SQLiteDB::UpdateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "")
// {
//     return 0;
// }

// int SQLiteDB::DeleteStagingDoc(std::string UUID)
// {
//     return 0;
// }

int SQLiteDB::BeginTransaction() const
{
    char *error_msg = 0;
    std::cout << "Begin transaction" << std::endl;
    int ret = sqlite3_exec(database_conn_, "BEGIN TRANSACTION", 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, begin transaction failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::EndTransaction() const
{
    char *error_msg = 0;
    std::cout << "End transaction" << std::endl;
    int ret = sqlite3_exec(database_conn_, "END TRANSACTION", 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, end transaction  failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::ResetStatement(sqlite3_stmt* stmt) const
{
    int ret = sqlite3_reset(stmt);
    std::cout << "Reset statement: " << sqlite3_sql(stmt) << std::endl;
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, reset failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::BindInt(sqlite3_stmt* stmt, const std::string &parameter_name, const int &bind_int) const
{
    std::cout << "BindInt " << parameter_name << ": " << bind_int << std::endl;
    int ret = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, parameter_name.c_str()), bind_int);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, bind of " << parameter_name << ": " << bind_int << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }

    return 0;
}

int SQLiteDB::BindText(sqlite3_stmt* stmt, const std::string &parameter_name, const std::string &bind_text) const
{
    std::cout << "BindText " << parameter_name << ": " << bind_text << std::endl;
    int index = sqlite3_bind_parameter_index(stmt, parameter_name.c_str());
    std::cout << index << std::endl;
    int ret = sqlite3_bind_text(stmt, index, bind_text.c_str(), bind_text.length(), SQLITE_STATIC);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, bind of " << parameter_name << ": " << bind_text << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }

    return 0;
}

int SQLiteDB::PrepareStatements()
{
    if (!database_conn_)
        return -1;

    int ret;

    prepared_statements_.emplace_back();
    ret = sqlite3_prepare_v2(database_conn_, CreateStagingEntryStatement, -1, &prepared_statements_[CREATE_STAGING_DOC_STATEMENT], nullptr);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, prepare failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    prepared_statements_.emplace_back();
    ret = sqlite3_prepare_v2(database_conn_, TestStatement, -1, &prepared_statements_[TEST_STATEMENT], nullptr);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, prepare failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }


    return 0;
}

} // namespace black_library_sqlite3
} // namespace db
} // namespace core
} // namespace black_library
