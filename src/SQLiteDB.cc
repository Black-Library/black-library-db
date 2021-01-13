/**
 * SQLiteDB.cc
 */

#include <iostream>
#include <string>

#include <SQLiteDB.hh>

namespace black_library {

namespace core {

namespace db {

namespace black_library_sqlite3 {

static constexpr const char CreateUserTable[]                    = "CREATE TABLE IF NOT EXISTS user(UID INTEGER PRIMARY KEY, permission_level INTEGER DEFAULT 0 NOT NULL, name TEXT NOT NULL)";
static constexpr const char CreateEntryTypeTable[]               = "CREATE TABLE IF NOT EXISTS entry_type(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateDocumentSubtypeTable[]         = "CREATE TABLE IF NOT EXISTS document_subtype(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateImageGallerySubtypeTable[]     = "CREATE TABLE IF NOT EXISTS image_gallery_subtype(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateVideoSubtypeTable[]            = "CREATE TABLE IF NOT EXISTS video_subtype(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateBookGenreTable[]               = "CREATE TABLE IF NOT EXISTS book_genre(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateDocumentTagTable[]             = "CREATE TABLE IF NOT EXISTS document_tag(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateSourceTable[]                  = "CREATE TABLE IF NOT EXISTS source(name TEXT NOT NULL PRIMARY KEY, type TEXT, FOREIGN KEY(type) REFERENCES entry_type(name))";
static constexpr const char CreateStagingEntryTable[]            = "CREATE TABLE IF NOT EXISTS staging_entry(UUID VARCHAR(36) PRIMARY KEY NOT NULL, title TEXT NOT NULL, nickname TEXT, source TEXT, URL TEXT, series TEXT, series_length DEFAULT 1, version INTEGER, media_path TEXT NOT NULL, birth_date TEXT NOT NULL, user_contributed INTEGER NOT NULL, FOREIGN KEY(source) REFERENCES source(name), FOREIGN KEY(user_contributed) REFERENCES user(UID))";
static constexpr const char CreateBlackEntryTable[]              = "CREATE TABLE IF NOT EXISTS black_entry(UUID VARCHAR(36) PRIMARY KEY NOT NULL, title TEXT NOT NULL, nickname TEXT, source TEXT, URL TEXT, series TEXT, series_length DEFAULT 1, version INTEGER, media_path TEXT NOT NULL, birth_date TEXT NOT NULL, user_contributed INTEGER NOT NULL, FOREIGN KEY(source) REFERENCES source(name), FOREIGN KEY(user_contributed) REFERENCES user(UID))";

static constexpr const char CreateUserStatement[]                = "INSERT INTO user(UID, permission_level, name) VALUES (:UID, :permission_level, :name)";
static constexpr const char CreateEntryTypeStatement[]           = "INSERT INTO entry_type(name) VALUES (:name)";
static constexpr const char CreateDocumentSubtypeStatement[]     = "INSERT INTO document_subtype(name) VALUES (:name)";
static constexpr const char CreateImageGallerySubtypeStatement[] = "INSERT INTO image_gallery_subtype(name) VALUES (:name)";
static constexpr const char CreateVideoSubtypeStatement[]        = "INSERT INTO video_subtype(name) VALUES (:name)";
static constexpr const char CreateSourceStatement[]              = "INSERT INTO source(name, type) VALUES (:name, :type)";
static constexpr const char CreateStagingEntryStatement[]        = "INSERT INTO staging_entry(UUID, title, nickname, source, URL, series, series_length, version, media_path, birth_date, user_contributed) VALUES (:UUID, :title, :nickname, :source, :URL, :series, :series_length, :version, :media_path, :birth_date, :user_contributed)";

static constexpr const char ReadStagingEntryStatment[]           = "SELECT * FROM staging_entry WHERE UUID = :UUID";

typedef enum {
    CREATE_USER_STATEMENT,
    CREATE_ENTRY_TYPE_STATEMENT,
    CREATE_DOCUMENT_SUBTYPE_STATEMENT,
    CREATE_IMAGE_GALLERY_SUBTYPE_STATEMENT,
    CREATE_VIDEO_SUBTYPE_STATEMENT,
    CREATE_SOURCE_STATEMENT,
    CREATE_STAGING_ENTRY_STATEMENT,

    READ_STAGING_ENTRY_STATEMENT,
    _NUM_PREPARED_STATEMENTS
} prepared_statement_id_t;

SQLiteDB::SQLiteDB(const std::string &database_url, const bool first_time_setup) :
    database_conn_(),
    prepared_statements_(),
    database_url_(database_url),
    first_time_setup_(first_time_setup),
    intialized_(false)
{
    if (database_url_.empty())
    {
        database_url_ = "/mnt/db/catalog.db";
        std::cout << "Empty database url given, using default: " << database_url_ << std::endl;
    }

    int res = sqlite3_open(database_url_.c_str(), &database_conn_);
    
    if (res != SQLITE_OK)
    {
        std::cout << "Error, failed to open db at: " << database_url_ << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return;
    }

    std::cout << "Open database at: " << database_url_ << std::endl;


    if (SetupTables())
    {
        std::cout << "Error, failed to setup database tables" << std::endl;
        return;
    }

    if (PrepareStatements())
    {
        std::cout << "Error, failed to setup prepare statements" << std::endl;
        return;
    }

    if (first_time_setup_)
    {
        if (SetupDefaultBlackLibraryUsers())
        {
            std::cout << "Error, failed to setup default black library users" << std::endl;
            return;
        }

        if (SetupDefaultSubtypes())
        {
            std::cout << "Error, failed to setup default subtypes" << std::endl;
            return;
        }
    }

    intialized_ = true;
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

int SQLiteDB::CreateUser(const DBUser &user) const
{
    std::cout << "create user: " << user.name << " with UID: " << user.UID << std::endl;

    if (BeginTransaction())
        return -1;

    int ret = SQLITE_OK;
    int statement_id = CREATE_USER_STATEMENT;
    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindInt(stmt, "UID", user.UID))
        return -1;
    if (BindInt(stmt, "permission_level", user.permission_level))
        return -1;
    if (BindText(stmt, "name", user.name))
        return -1;

    // run statement
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error, insert user failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::CreateStagingEntry(const DBEntry &entry) const
{
    std::cout << "create staging entry for UUID: " << entry.UUID << std::endl;

    if (CheckInitialized())
        return -1;

    if (BeginTransaction())
        return -1;

    int ret = SQLITE_OK;
    int statement_id = CREATE_STAGING_ENTRY_STATEMENT;
    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", entry.UUID))
        return -1;
    if (BindText(stmt, "title", entry.title))
        return -1;
    if (BindText(stmt, "nickname", entry.nickname))
        return -1;
    if (BindText(stmt, "source", entry.source))
        return -1;
    if (BindText(stmt, "URL", entry.URL))
        return -1;
    if (BindText(stmt, "series", entry.series))
        return -1;
    if (BindInt(stmt, "series_length", entry.series_length))
        return -1;
    if (BindInt(stmt, "version", entry.version))
        return -1;
    if (BindText(stmt, "media_path", entry.media_path))
        return -1;
    if (BindText(stmt, "birth_date", entry.birth_date))
        return -1;
    if (BindInt(stmt, "user_contributed", entry.user_contributed))
        return -1;

    // run statement
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error, insert staging doc failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

DBEntry SQLiteDB::ReadStagingEntry(std::string UUID) const
{
    DBEntry entry;

    std::cout << "Read staging entry for UUID: " << UUID << std::endl;

    // TODO check if entry is empty in another step
    if (CheckInitialized())
        return entry;

    if (BeginTransaction())
        return entry;

    int ret = SQLITE_OK;
    int statement_id = READ_STAGING_ENTRY_STATEMENT;
    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", UUID))
    {
        EndTransaction();
        return entry;
    }

    // run statement
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "Error, read staging entry failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return entry;
    }

    entry.UUID = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    entry.title = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    entry.nickname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    entry.source = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    entry.URL = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    entry.series = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    entry.series_length = sqlite3_column_int(stmt, 6);
    entry.version = sqlite3_column_int(stmt, 7);
    entry.media_path = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
    entry.birth_date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9)));
    entry.user_contributed = sqlite3_column_int(stmt, 10);

    ResetStatement(stmt);

    if (EndTransaction())
        return entry;

    return entry;
}

// int SQLiteDB::UpdateStagingEntry(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "")
// {
//     return 0;
// }

// int SQLiteDB::DeleteStagingEntry(std::string UUID)
// {
//     return 0;
// }

int SQLiteDB::SetupTables()
{
    GenerateTable(CreateUserTable);
    GenerateTable(CreateEntryTypeTable);
    GenerateTable(CreateDocumentSubtypeTable);
    GenerateTable(CreateImageGallerySubtypeTable);
    GenerateTable(CreateVideoSubtypeTable);
    GenerateTable(CreateBookGenreTable);
    GenerateTable(CreateDocumentTagTable);
    GenerateTable(CreateSourceTable);
    GenerateTable(CreateStagingEntryTable);
    GenerateTable(CreateBlackEntryTable);

    return 0;
}

int SQLiteDB::PrepareStatements()
{
    if (!database_conn_)
        return -1;

    PrepareStatement(CreateUserStatement, CREATE_USER_STATEMENT);
    PrepareStatement(CreateEntryTypeStatement, CREATE_ENTRY_TYPE_STATEMENT);
    PrepareStatement(CreateDocumentSubtypeStatement, CREATE_DOCUMENT_SUBTYPE_STATEMENT);
    PrepareStatement(CreateImageGallerySubtypeStatement, CREATE_IMAGE_GALLERY_SUBTYPE_STATEMENT);
    PrepareStatement(CreateVideoSubtypeStatement, CREATE_VIDEO_SUBTYPE_STATEMENT);
    PrepareStatement(CreateSourceStatement, CREATE_SOURCE_STATEMENT);
    PrepareStatement(CreateStagingEntryStatement, CREATE_STAGING_ENTRY_STATEMENT);

    PrepareStatement(ReadStagingEntryStatment, READ_STAGING_ENTRY_STATEMENT);

    return 0;
}

int SQLiteDB::SetupDefaultBlackLibraryUsers()
{
    DBUser black_library_admin;
    DBUser black_library_librarian;
    DBUser black_library_writer;
    DBUser black_library_reader;
    DBUser black_library_no_permissions;

    black_library_admin.UID = 4007;
    black_library_admin.permission_level = READ_WRITE_EXECUTE_PERMISSIONS;
    black_library_admin.name = "BlackLibraryAdmin";

    black_library_librarian.UID = 4004;
    black_library_librarian.permission_level = READ_WRITE_PERMISSIONS;
    black_library_librarian.name = "BlackLibraryLibrarian";

    black_library_writer.UID = 4003;
    black_library_writer.permission_level = WRITE_PERMISSIONS;
    black_library_writer.name = "BlackLibraryWriter";

    black_library_reader.UID = 4001;
    black_library_reader.permission_level = READ_PERMISSIONS;
    black_library_reader.name = "BlackLibraryReader";

    black_library_no_permissions.UID = 4000;
    black_library_no_permissions.permission_level = NO_PERMISSIONS;
    black_library_no_permissions.name = "BlackLibraryNoPermissions";

    if (CreateUser(black_library_admin))
        return -1;
    if (CreateUser(black_library_librarian))
        return -1;
    if (CreateUser(black_library_writer))
        return -1;
    if (CreateUser(black_library_reader))
        return -1;
    if (CreateUser(black_library_no_permissions))
        return -1;

    return 0;
}

int SQLiteDB::SetupDefaultSubtypes()
{
    
    return 0;
}

int SQLiteDB::BeginTransaction() const
{
    char *error_msg = 0;
    // std::cout << "Begin transaction" << std::endl;
    int ret = sqlite3_exec(database_conn_, "BEGIN TRANSACTION", 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, begin transaction failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::CheckInitialized() const
{
    if (!intialized_)
    {
        std::cout << "Error, db not initialized" << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::EndTransaction() const
{
    char *error_msg = 0;
    // std::cout << "End transaction" << std::endl;
    int ret = sqlite3_exec(database_conn_, "END TRANSACTION", 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, end transaction  failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::GenerateTable(const std::string &sql)
{
    char *error_msg = 0;
    int ret = sqlite3_exec(database_conn_, sql.c_str(), 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, generate table failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }
    return 0;
}

int SQLiteDB::PrepareStatement(const std::string &statement, int statement_id)
{
    prepared_statements_.emplace_back();
    int ret = sqlite3_prepare_v2(database_conn_, statement.c_str(), -1, &prepared_statements_[statement_id], nullptr);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, prepare failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::ResetStatement(sqlite3_stmt* stmt) const
{
    int ret = sqlite3_reset(stmt);
    // std::cout << "Reset statement: " << sqlite3_sql(stmt) << std::endl;
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, reset statement failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::BindInt(sqlite3_stmt* stmt, const std::string &parameter_name, const int &bind_int) const
{
    // std::cout << "BindInt " << parameter_name << ": " << bind_int << std::endl;
    const std::string parameter_index_name = ":" + parameter_name;
    int ret = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, parameter_index_name.c_str()), bind_int);
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
    // std::cout << "BindText " << parameter_name << ": " << bind_text << std::endl;
    const std::string parameter_index_name = ":" + parameter_name;
    int index = sqlite3_bind_parameter_index(stmt, parameter_index_name.c_str());
    int ret = sqlite3_bind_text(stmt, index, bind_text.c_str(), bind_text.length(), SQLITE_STATIC);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, bind of " << parameter_name << ": " << bind_text << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }

    return 0;
}

} // namespace black_library_sqlite3
} // namespace db
} // namespace core
} // namespace black_library
