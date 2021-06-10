/**
 * SQLiteDB.cc
 */

#include <iostream>
#include <string>
#include <sstream>

#include <FileOperations.h>
#include <SourceInformation.h>

#include <DBConnectionInterfaceUtils.h>
#include <SQLiteDB.h>

namespace black_library {

namespace core {

namespace db {

static constexpr const char CreateUserTable[]                     = "CREATE TABLE IF NOT EXISTS user(UID INTEGER PRIMARY KEY, permission_level INTEGER DEFAULT 0 NOT NULL, name TEXT NOT NULL)";
static constexpr const char CreateEntryTypeTable[]                = "CREATE TABLE IF NOT EXISTS entry_type(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateDocumentSubtypeTable[]          = "CREATE TABLE IF NOT EXISTS document_subtype(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateImageGallerySubtypeTable[]      = "CREATE TABLE IF NOT EXISTS image_gallery_subtype(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateVideoSubtypeTable[]             = "CREATE TABLE IF NOT EXISTS video_subtype(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateBookGenreTable[]                = "CREATE TABLE IF NOT EXISTS book_genre(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateDocumentTagTable[]              = "CREATE TABLE IF NOT EXISTS document_tag(name TEXT NOT NULL PRIMARY KEY)";
static constexpr const char CreateSourceTable[]                   = "CREATE TABLE IF NOT EXISTS source(name TEXT NOT NULL PRIMARY KEY, type TEXT, FOREIGN KEY(type) REFERENCES entry_type(name))";
static constexpr const char CreateStagingEntryTable[]             = "CREATE TABLE IF NOT EXISTS staging_entry(UUID VARCHAR(36) PRIMARY KEY NOT NULL, title TEXT NOT NULL, author TEXT NOT NULL, nickname TEXT, source TEXT, url TEXT, last_url TEXT, series TEXT, series_length DEFAULT 1, version INTEGER, media_path TEXT NOT NULL, birth_date INTEGER, check_date INTEGER, update_date INTEGER, user_contributed INTEGER NOT NULL, FOREIGN KEY(source) REFERENCES source(name), FOREIGN KEY(user_contributed) REFERENCES user(UID))";
static constexpr const char CreateBlackEntryTable[]               = "CREATE TABLE IF NOT EXISTS black_entry(UUID VARCHAR(36) PRIMARY KEY NOT NULL, title TEXT NOT NULL, author TEXT NOT NULL, nickname TEXT, source TEXT, url TEXT, last_url TEXT, series TEXT, series_length DEFAULT 1, version INTEGER, media_path TEXT NOT NULL, birth_date INTEGER, check_date INTEGER, update_date INTEGER, user_contributed INTEGER NOT NULL, FOREIGN KEY(source) REFERENCES source(name), FOREIGN KEY(user_contributed) REFERENCES user(UID))";
static constexpr const char CreateErrorEntryTable[]               = "CREATE TABLE IF NOT EXISTS error_entry(UUID VARCHAR(36) PRIMARY KEY NOT NULL, progress_num INTEGER)";

static constexpr const char CreateUserStatement[]                 = "INSERT INTO user(UID, permission_level, name) VALUES (:UID, :permission_level, :name)";
static constexpr const char CreateEntryTypeStatement[]            = "INSERT INTO entry_type(name) VALUES (:name)";
static constexpr const char CreateDocumentSubtypeStatement[]      = "INSERT INTO document_subtype(name) VALUES (:name)";
static constexpr const char CreateImageGallerySubtypeStatement[]  = "INSERT INTO image_gallery_subtype(name) VALUES (:name)";
static constexpr const char CreateVideoSubtypeStatement[]         = "INSERT INTO video_subtype(name) VALUES (:name)";
static constexpr const char CreateSourceStatement[]               = "INSERT INTO source(name, type) VALUES (:name, :type)";
static constexpr const char CreateStagingEntryStatement[]         = "INSERT INTO staging_entry(UUID, title, author, nickname, source, url, last_url, series, series_length, version, media_path, birth_date, check_date, update_date, user_contributed) VALUES (:UUID, :title, :author, :nickname, :source, :url, :last_url, :series, :series_length, :version, :media_path, :birth_date, :check_date, :update_date, :user_contributed)";
static constexpr const char CreateBlackEntryStatement[]           = "INSERT INTO black_entry(UUID, title, author, nickname, source, url, last_url, series, series_length, version, media_path, birth_date, check_date, update_date, user_contributed) VALUES (:UUID, :title, :author, :nickname, :source, :url, :last_url, :series, :series_length, :version, :media_path, :birth_date, :check_date, :update_date, :user_contributed)";
static constexpr const char CreateErrorEntryStatement[]           = "INSERT INTO error_entry(UUID, progress_num) VALUES (:UUID, :progress_num)";

static constexpr const char ReadStagingEntryStatement[]           = "SELECT * FROM staging_entry WHERE UUID = :UUID";
static constexpr const char ReadStagingEntryUrlStatement[]        = "SELECT * FROM staging_entry WHERE url = :url";
static constexpr const char ReadStagingEntryUUIDStatement[]       = "SELECT * FROM staging_entry WHERE UUID = :UUID";
static constexpr const char ReadBlackEntryStatement[]             = "SELECT * FROM black_entry WHERE UUID = :UUID";
static constexpr const char ReadBlackEntryUrlStatement[]          = "SELECT * FROM black_entry WHERE url = :url";
static constexpr const char ReadBlackEntryUUIDStatement[]         = "SELECT * FROM black_entry WHERE UUID = :UUID";
static constexpr const char ReadErrorEntryStatement[]             = "SELECT * FROM error_entry WHERE UUID = :UUID AND progress_num = :progress_num";

static constexpr const char UpdateStagingEntryStatement[]         = "UPDATE staging_entry SET title = :title, author = :author, nickname = :nickname, source = :source, url = :url, last_url = :last_url, series = :series, series_length = :series_length, version = :version, media_path = :media_path, birth_date = :birth_date, check_date = :check_date, update_date = :update_date, user_contributed = :user_contributed WHERE UUID = :UUID";
static constexpr const char UpdateBlackEntryStatement[]           = "UPDATE black_entry SET title = :title, author = :author, nickname = :nickname, source = :source, url = :url, last_url = :last_url, series = :series, series_length = :series_length, version = :version, media_path = :media_path, birth_date = :birth_date, check_date = :check_date, update_date = :update_date, user_contributed = :user_contributed WHERE UUID = :UUID";

static constexpr const char DeleteStagingEntryStatement[]         = "DELETE FROM staging_entry WHERE UUID = :UUID";
static constexpr const char DeleteBlackEntryStatement[]           = "DELETE FROM black_entry WHERE UUID = :UUID";
static constexpr const char DeleteErrorEntryStatement[]           = "DELETE FROM error_entry WHERE UUID = :UUID AND progress_num = :progress_num";

static constexpr const char GetStagingEntriesStatement[]          = "SELECT * FROM staging_entry";
static constexpr const char GetBlackEntriesStatement[]            = "SELECT * FROM black_entry"; 
static constexpr const char GetErrorEntriesStatement[]            = "SELECT * FROM error_entry";

static constexpr const char GetStagingEntryUUIDFromUrlStatement[] = "SELECT UUID FROM staging_entry WHERE url = :url";
static constexpr const char GetBlackEntryUUIDFromUrlStatement[]   = "SELECT UUID FROM black_entry WHERE url = :url";
static constexpr const char GetStagingEntryUrlFromUUIDStatement[] = "SELECT url, last_url FROM staging_entry WHERE UUID = :UUID";
static constexpr const char GetBlackEntryUrlFromUUIDStatement[]   = "SELECT url, last_url FROM black_entry WHERE UUID = :UUID";

typedef enum {
    CREATE_USER_STATEMENT,
    CREATE_ENTRY_TYPE_STATEMENT,
    CREATE_DOCUMENT_SUBTYPE_STATEMENT,
    CREATE_IMAGE_GALLERY_SUBTYPE_STATEMENT,
    CREATE_VIDEO_SUBTYPE_STATEMENT,
    CREATE_SOURCE_STATEMENT,
    CREATE_STAGING_ENTRY_STATEMENT,
    CREATE_BLACK_ENTRY_STATEMENT,
    CREATE_ERROR_ENTRY_STATEMENT,

    READ_STAGING_ENTRY_STATEMENT,
    READ_STAGING_ENTRY_URL_STATEMENT,
    READ_STAGING_ENTRY_UUID_STATEMENT,
    READ_BLACK_ENTRY_STATEMENT,
    READ_BLACK_ENTRY_URL_STATEMENT,
    READ_BLACK_ENTRY_UUID_STATEMENT,
    READ_ERROR_ENTRY_STATEMENT,

    UPDATE_STAGING_ENTRY_STATEMENT,
    UPDATE_BLACK_ENTRY_STATEMENT,

    DELETE_STAGING_ENTRY_STATEMENT,
    DELETE_BLACK_ENTRY_STATEMENT,
    DELETE_ERROR_ENTRY_STATEMENT,

    GET_STAGING_ENTRIES_STATEMENT,
    GET_BLACK_ENTRIES_STATEMENT,
    GET_ERROR_ENTRIES_STATEMENT,

    GET_STAGING_ENTRY_UUID_FROM_URL_STATEMENT,
    GET_STAGING_ENTRY_URL_FROM_UUID_STATEMENT,
    GET_BLACK_ENTRY_UUID_FROM_URL_STATEMENT,
    GET_BLACK_ENTRY_URL_FROM_UUID_STATEMENT,

    _NUM_PREPARED_STATEMENTS
} prepared_statement_id_t;

SQLiteDB::SQLiteDB(const std::string &database_url, bool first_time_setup) :
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

    if (!black_library::core::common::Exists(database_url_))
    {
        first_time_setup_ = true;
    }

    int res = sqlite3_open(database_url_.c_str(), &database_conn_);
    
    if (res != SQLITE_OK)
    {
        std::cout << "Error: failed to open db at: " << database_url_ << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return;
    }

    std::cout << "Open database at: " << database_url_ << std::endl;

    if (first_time_setup_)
    {
        if (GenerateTables())
        {
            std::cout << "Error: failed to generate database tables" << std::endl;
            return;
        }
    }

    if (PrepareStatements())
    {
        std::cout << "Error: failed to setup prepare statements" << std::endl;
        return;
    }

    if (first_time_setup_)
    {
        if (SetupDefaultBlackLibraryUsers())
        {
            std::cout << "Error: failed to setup default black library users" << std::endl;
            return;
        }
        if (SetupDefaultTables())
        {
            std::cout << "Error: failed to setup default tables" << std::endl;
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

std::vector<DBEntry> SQLiteDB::ListEntries(entry_table_rep_t entry_type) const
{
    std::cout << "List " << GetEntryTypeString(entry_type) << " entries" << std::endl;

    std::vector<DBEntry> entries;

    if (CheckInitialized())
        return entries;

    if (BeginTransaction())
        return entries;

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = GET_BLACK_ENTRIES_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = GET_STAGING_ENTRIES_STATEMENT;
            break;
        default:
            return entries;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];
    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;

    // run statement in loop until done
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        DBEntry entry;

        entry.uuid = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        entry.title = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        entry.author = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        entry.nickname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        entry.source = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        entry.url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        entry.last_url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        entry.series = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
        entry.series_length = sqlite3_column_int(stmt, 8);
        entry.version = sqlite3_column_int(stmt, 9);
        entry.media_path = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
        entry.birth_date = sqlite3_column_int(stmt, 11);
        entry.check_date = sqlite3_column_int(stmt, 12);
        entry.update_date = sqlite3_column_int(stmt, 13);
        entry.user_contributed = sqlite3_column_int(stmt, 14);

        entries.emplace_back(entry);
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return entries;

    return entries;
}

std::vector<ErrorEntry> SQLiteDB::ListErrorEntries() const
{
    std::cout << "List error entries" << std::endl;

    std::vector<ErrorEntry> entries;

    if (CheckInitialized())
        return entries;

    if (BeginTransaction())
        return entries;

    sqlite3_stmt *stmt = prepared_statements_[GET_ERROR_ENTRIES_STATEMENT];
    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;

    // run statement in loop until done
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        ErrorEntry entry;

        entry.uuid = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        entry.progress_num = sqlite3_column_int(stmt, 1);

        entries.emplace_back(entry);
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return entries;

    return entries;
}

int SQLiteDB::CreateUser(const DBUser &user) const
{
    std::cout << "Create user: " << user.name << " with UID: " << user.uid << std::endl;

    if (BeginTransaction())
        return -1;

    int statement_id = CREATE_USER_STATEMENT;
    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindInt(stmt, "UID", user.uid))
        return -1;
    if (BindInt(stmt, "permission_level", static_cast<uint8_t>(user.permission_level)))
        return -1;
    if (BindText(stmt, "name", user.name))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: create user failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::CreateEntryType(const std::string &entry_type_name) const
{
    if (BeginTransaction())
        return -1;

    int statement_id = CREATE_ENTRY_TYPE_STATEMENT;
    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "name", entry_type_name))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    // TODO: add trace for db sql calls
    std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: create entry type: " << entry_type_name << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::CreateSubtype(const std::string &subtype_name, DBEntryMediaType media_type) const
{
    if (BeginTransaction())
        return -1;

    int statement_id;

    switch (media_type)
    {
    case DBEntryMediaType::Document:
        statement_id = CREATE_DOCUMENT_SUBTYPE_STATEMENT;
        break;
    case DBEntryMediaType::ImageGallery:
        statement_id = CREATE_IMAGE_GALLERY_SUBTYPE_STATEMENT;
        break;
    case DBEntryMediaType::Video:
        statement_id = CREATE_VIDEO_SUBTYPE_STATEMENT;
        break;
    default:
        break;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "name", subtype_name))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: create subtype: " << subtype_name << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::CreateSource(const DBSource &source) const
{
    if (BeginTransaction())
        return -1;

    int statement_id = CREATE_SOURCE_STATEMENT;
    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "name", source.name))
        return -1;
    if (BindText(stmt, "type", GetMediaTypeString(source.media_type)))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: create source: " << source.name << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::CreateEntry(const DBEntry &entry, entry_table_rep_t entry_type) const
{
    std::cout << "Create " << GetEntryTypeString(entry_type) << " entry for UUID: " << entry.uuid << std::endl;

    if (CheckInitialized())
        return -1;

    if (BeginTransaction())
        return -1;

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = CREATE_BLACK_ENTRY_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = CREATE_STAGING_ENTRY_STATEMENT;
            break;
        default:
            return -1;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", entry.uuid))
        return -1;
    if (BindText(stmt, "title", entry.title))
        return -1;
    if (BindText(stmt, "author", entry.author))
        return -1;
    if (BindText(stmt, "nickname", entry.nickname))
        return -1;
    if (BindText(stmt, "source", entry.source))
        return -1;
    if (BindText(stmt, "url", entry.url))
        return -1;
    if (BindText(stmt, "last_url", entry.url))
        return -1;
    if (BindText(stmt, "series", entry.series))
        return -1;
    if (BindInt(stmt, "series_length", entry.series_length))
        return -1;
    if (BindInt(stmt, "version", entry.version))
        return -1;
    if (BindText(stmt, "media_path", entry.media_path))
        return -1;
    if (BindInt(stmt, "birth_date", entry.birth_date))
        return -1;
    if (BindInt(stmt, "check_date", entry.check_date))
        return -1;
    if (BindInt(stmt, "update_date", entry.update_date))
        return -1;
    if (BindInt(stmt, "user_contributed", entry.user_contributed))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: create " << GetEntryTypeString(entry_type) << " entry failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

DBEntry SQLiteDB::ReadEntry(const std::string &uuid, entry_table_rep_t entry_type) const
{
    DBEntry entry;

    std::cout << "Read " << GetEntryTypeString(entry_type) << " entry with UUID: " << uuid << std::endl;

    if (CheckInitialized())
        return entry;

    if (BeginTransaction())
        return entry;

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = READ_BLACK_ENTRY_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = READ_STAGING_ENTRY_STATEMENT;
            break;
        default:
            return entry;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", uuid))
    {
        EndTransaction();
        return entry;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "Error: read " << GetEntryTypeString(entry_type) << " entry failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return entry;
    }

    entry.uuid = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    entry.title = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    entry.author = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    entry.nickname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    entry.source = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    entry.url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    entry.last_url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
    entry.series = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
    entry.series_length = sqlite3_column_int(stmt, 8);
    entry.version = sqlite3_column_int(stmt, 9);
    entry.media_path = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
    entry.birth_date = sqlite3_column_int(stmt, 11);
    entry.check_date = sqlite3_column_int(stmt, 12);
    entry.update_date = sqlite3_column_int(stmt, 13);
    entry.user_contributed = sqlite3_column_int(stmt, 14);

    ResetStatement(stmt);

    if (EndTransaction())
        return entry;

    return entry;
}

int SQLiteDB::UpdateEntry(const DBEntry &entry, entry_table_rep_t entry_type) const
{
    std::cout << "Update " << GetEntryTypeString(entry_type) << " entry for UUID: " << entry.uuid << std::endl;

    if (CheckInitialized())
        return -1;

    if (BeginTransaction())
        return -1;

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = UPDATE_BLACK_ENTRY_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = UPDATE_STAGING_ENTRY_STATEMENT;
            break;
        default:
            return -1;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", entry.uuid))
        return -1;
    if (BindText(stmt, "title", entry.title))
        return -1;
    if (BindText(stmt, "author", entry.author))
        return -1;
    if (BindText(stmt, "nickname", entry.nickname))
        return -1;
    if (BindText(stmt, "source", entry.source))
        return -1;
    if (BindText(stmt, "url", entry.url))
        return -1;
    if (BindText(stmt, "last_url", entry.last_url))
        return -1;
    if (BindText(stmt, "series", entry.series))
        return -1;
    if (BindInt(stmt, "series_length", entry.series_length))
        return -1;
    if (BindInt(stmt, "version", entry.version))
        return -1;
    if (BindText(stmt, "media_path", entry.media_path))
        return -1;
    if (BindInt(stmt, "birth_date", entry.birth_date))
        return -1;
    if (BindInt(stmt, "check_date", entry.check_date))
        return -1;
    if (BindInt(stmt, "update_date", entry.update_date))
        return -1;
    if (BindInt(stmt, "user_contributed", entry.user_contributed))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: update " << GetEntryTypeString(entry_type) << " entry failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::DeleteEntry(const std::string &uuid, entry_table_rep_t entry_type) const
{
    std::cout << "Delete " << GetEntryTypeString(entry_type) << " UUID: " << uuid << std::endl;

    if (CheckInitialized())
        return -1;

    if (BeginTransaction())
        return -1;

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = DELETE_BLACK_ENTRY_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = DELETE_STAGING_ENTRY_STATEMENT;
            break;
        default:
            return -1;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", uuid))
    {
        EndTransaction();
        return -1;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: delete " << GetEntryTypeString(entry_type) <<  " entry failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

int SQLiteDB::CreateErrorEntry(const ErrorEntry &entry) const
{
    std::cout << "Create error entry for UUID: " << entry.uuid << std::endl;

    if (CheckInitialized())
        return -1;

    if (BeginTransaction())
        return -1;

    sqlite3_stmt *stmt = prepared_statements_[CREATE_ERROR_ENTRY_STATEMENT];

    // bind statement variables
    if (BindText(stmt, "UUID", entry.uuid))
        return -1;
    if (BindInt(stmt, "progress_num", entry.progress_num))
        return -1;

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        std::cout << "Error: create error entry failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        return -1;
    }

    ResetStatement(stmt);

    if (EndTransaction())
        return -1;

    return 0;
}

DBBoolResult SQLiteDB::DoesEntryUrlExist(const std::string &url, entry_table_rep_t entry_type) const
{
    DBBoolResult check;

    std::cout << "Check " << GetEntryTypeString(entry_type) << " entries for url: " << url << std::endl;

    if (CheckInitialized())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    if (BeginTransaction())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = READ_BLACK_ENTRY_URL_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = READ_STAGING_ENTRY_URL_STATEMENT;
            break;
        default:
            return check;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "url", url))
    {
        EndTransaction();
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "url: " << url << " does not exist" << std::endl;
        check.result = false;
        ResetStatement(stmt);
        EndTransaction();
        return check;
    }
    else
    {
        check.result = true;
    }
    

    ResetStatement(stmt);

    if (EndTransaction())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    return check;
}

DBBoolResult SQLiteDB::DoesEntryUUIDExist(const std::string &uuid, entry_table_rep_t entry_type) const
{
    DBBoolResult check;

    std::cout << "Check " << GetEntryTypeString(entry_type) << " entries for UUID: " << uuid << std::endl;

    if (uuid.empty())
    {
        check.result = false;
        return check;
    }

    if (CheckInitialized())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    if (BeginTransaction())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = READ_BLACK_ENTRY_UUID_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = READ_STAGING_ENTRY_UUID_STATEMENT;
            break;
        default:
            return check;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", uuid))
    {
        EndTransaction();
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "UUID: " << uuid << " does not exist" << std::endl;
        check.result = false;
        ResetStatement(stmt);
        EndTransaction();
        return check;
    }
    else
    {
        check.result = true;
    }
    

    ResetStatement(stmt);

    if (EndTransaction())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    return check;
}

DBBoolResult SQLiteDB::DoesErrorEntryExist(const std::string &uuid, size_t progress_num) const
{
    DBBoolResult check;

    std::cout << "Check error entries for UUID: " << uuid << "progress_num: " << progress_num << std::endl;

    if (uuid.empty())
    {
        check.result = false;
        return check;
    }

    if (CheckInitialized())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    if (BeginTransaction())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    sqlite3_stmt *stmt = prepared_statements_[READ_ERROR_ENTRY_STATEMENT];

    // bind statement variables
    if (BindText(stmt, "UUID", uuid))
    {
        EndTransaction();
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }
    if (BindInt(stmt, "progress_num", progress_num))
    {
        EndTransaction();
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "UUID: " << uuid << " progress_num: " << progress_num << "does not exist" << std::endl;
        check.result = false;
        ResetStatement(stmt);
        EndTransaction();
        return check;
    }
    else
    {
        check.result = true;
    }
    

    ResetStatement(stmt);

    if (EndTransaction())
    {
        check.error = sqlite3_errcode(database_conn_);
        return check;
    }

    return check;
}

DBStringResult SQLiteDB::GetEntryUUIDFromUrl(const std::string &url, entry_table_rep_t entry_type) const
{
    std::cout << "Get UUID from: " << url << std::endl;

    DBStringResult res;

    if (CheckInitialized())
    {
        return res;
    }

    if (BeginTransaction())
    {
        return res;
    }

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = GET_BLACK_ENTRY_UUID_FROM_URL_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = GET_STAGING_ENTRY_UUID_FROM_URL_STATEMENT;
            break;
        default:
            res.error = true;
            return res;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "url", url))
    {
        EndTransaction();
        return res;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "url: " << url << " does not exist" << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        res.does_not_exist = true;
        res.error = false;
        return res;
    }

    res.result = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));

    ResetStatement(stmt);

    if (EndTransaction())
    {
        return res;
    }

    res.error = false;

    return res;
}

DBStringResult SQLiteDB::GetEntryUrlFromUUID(const std::string &uuid, entry_table_rep_t entry_type) const
{
    std::cout << "Get url from: " << uuid << std::endl;

    DBStringResult res;

    if (CheckInitialized())
    {
        return res;
    }

    if (BeginTransaction())
    {
        return res;
    }

    int statement_id;
    switch (entry_type)
    {
        case BLACK_ENTRY:
            statement_id = GET_BLACK_ENTRY_URL_FROM_UUID_STATEMENT;
            break;
        case STAGING_ENTRY:
            statement_id = GET_STAGING_ENTRY_URL_FROM_UUID_STATEMENT;
            break;
        default:
            res.error = true;
            return res;
    }

    sqlite3_stmt *stmt = prepared_statements_[statement_id];

    // bind statement variables
    if (BindText(stmt, "UUID", uuid))
    {
        EndTransaction();
        return res;
    }

    // run statement
    int ret = SQLITE_OK;

    // std::cout << "\t" << sqlite3_expanded_sql(stmt) << std::endl;
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW)
    {
        std::cout << "UUID: " << uuid << " does not exist" << std::endl;
        ResetStatement(stmt);
        EndTransaction();
        res.does_not_exist = true;
        res.error = false;
        return res;
    }

    std::string url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    std::string last_url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

    ResetStatement(stmt);

    if (EndTransaction())
    {
        return res;
    }

    if (!last_url.empty())
    {
        res.result = last_url;
    }
    else
    {
        res.result = url;
    } 

    res.error = false;

    return res;
}

bool SQLiteDB::IsReady() const
{
    return intialized_;
}

int SQLiteDB::GenerateTables()
{
    int res = 0;

    res += GenerateTable(CreateUserTable);
    res += GenerateTable(CreateEntryTypeTable);
    res += GenerateTable(CreateDocumentSubtypeTable);
    res += GenerateTable(CreateImageGallerySubtypeTable);
    res += GenerateTable(CreateVideoSubtypeTable);
    res += GenerateTable(CreateBookGenreTable);
    res += GenerateTable(CreateDocumentTagTable);
    res += GenerateTable(CreateSourceTable);
    res += GenerateTable(CreateStagingEntryTable);
    res += GenerateTable(CreateBlackEntryTable);
    res += GenerateTable(CreateErrorEntryTable);

    return res;
}

int SQLiteDB::SetupDefaultTables()
{
    int res = 0;

    res += SetupDefaultEntryTypeTable();
    res += SetupDefaultSubtypeTable();
    res += SetupDefaultSourceTable();

    return res;
}

int SQLiteDB::SetupDefaultEntryTypeTable()
{
    int res = 0;

    res += CreateEntryType(GetMediaTypeString(DBEntryMediaType::Document));
    res += CreateEntryType(GetMediaTypeString(DBEntryMediaType::ImageGallery));
    res += CreateEntryType(GetMediaTypeString(DBEntryMediaType::Video));

    return res;
}

int SQLiteDB::SetupDefaultSubtypeTable()
{
    int res = 0;

    res += CreateSubtype("blog", DBEntryMediaType::Document);
    res += CreateSubtype("book", DBEntryMediaType::Document);
    res += CreateSubtype("news-article", DBEntryMediaType::Document);
    res += CreateSubtype("paper", DBEntryMediaType::Document);
    res += CreateSubtype("webnovel", DBEntryMediaType::Document);

    res += CreateSubtype("manga", DBEntryMediaType::ImageGallery);
    res += CreateSubtype("photo-album", DBEntryMediaType::ImageGallery);

    res += CreateSubtype("movie", DBEntryMediaType::Video);
    res += CreateSubtype("tv-show", DBEntryMediaType::Video);
    res += CreateSubtype("youtube", DBEntryMediaType::Video);

    return res;
}

int SQLiteDB::SetupDefaultSourceTable()
{
    DBSource ao3_source;
    DBSource ffn_source;
    DBSource rr_source;
    DBSource sbf_source;
    DBSource yt_source;

    ao3_source.name = black_library::core::common::AO3::source_name;
    ao3_source.media_type = DBEntryMediaType::Document;

    ffn_source.name = black_library::core::common::FFN::source_name;
    ffn_source.media_type = DBEntryMediaType::Document;

    rr_source.name = black_library::core::common::RR::source_name;
    rr_source.media_type = DBEntryMediaType::Document;

    sbf_source.name = black_library::core::common::SBF::source_name;
    sbf_source.media_type = DBEntryMediaType::Document;

    yt_source.name = black_library::core::common::YT::source_name;
    yt_source.media_type = DBEntryMediaType::Video;

    if (CreateSource(ao3_source))
        return -1;
    if (CreateSource(ffn_source))
        return -1;
    if (CreateSource(rr_source))
        return -1;
    if (CreateSource(sbf_source))
        return -1;
    if (CreateSource(yt_source))
        return -1;

    return 0;
}

int SQLiteDB::PrepareStatements()
{
    if (!database_conn_)
        return -1;

    int res = 0;

    res += PrepareStatement(CreateUserStatement, CREATE_USER_STATEMENT);
    res += PrepareStatement(CreateEntryTypeStatement, CREATE_ENTRY_TYPE_STATEMENT);
    res += PrepareStatement(CreateDocumentSubtypeStatement, CREATE_DOCUMENT_SUBTYPE_STATEMENT);
    res += PrepareStatement(CreateImageGallerySubtypeStatement, CREATE_IMAGE_GALLERY_SUBTYPE_STATEMENT);
    res += PrepareStatement(CreateVideoSubtypeStatement, CREATE_VIDEO_SUBTYPE_STATEMENT);
    res += PrepareStatement(CreateSourceStatement, CREATE_SOURCE_STATEMENT);

    res += PrepareStatement(CreateStagingEntryStatement, CREATE_STAGING_ENTRY_STATEMENT);
    res += PrepareStatement(CreateBlackEntryStatement, CREATE_BLACK_ENTRY_STATEMENT);
    res += PrepareStatement(CreateErrorEntryStatement, CREATE_ERROR_ENTRY_STATEMENT);

    res += PrepareStatement(ReadStagingEntryStatement, READ_STAGING_ENTRY_STATEMENT);
    res += PrepareStatement(ReadStagingEntryUrlStatement, READ_STAGING_ENTRY_URL_STATEMENT);
    res += PrepareStatement(ReadStagingEntryUUIDStatement, READ_STAGING_ENTRY_UUID_STATEMENT);

    res += PrepareStatement(ReadBlackEntryStatement, READ_BLACK_ENTRY_STATEMENT);
    res += PrepareStatement(ReadBlackEntryUrlStatement, READ_BLACK_ENTRY_URL_STATEMENT);
    res += PrepareStatement(ReadBlackEntryUUIDStatement, READ_BLACK_ENTRY_UUID_STATEMENT);

    res += PrepareStatement(ReadErrorEntryStatement, READ_ERROR_ENTRY_STATEMENT);

    res += PrepareStatement(UpdateStagingEntryStatement, UPDATE_STAGING_ENTRY_STATEMENT);
    res += PrepareStatement(UpdateBlackEntryStatement, UPDATE_BLACK_ENTRY_STATEMENT);

    res += PrepareStatement(DeleteStagingEntryStatement, DELETE_STAGING_ENTRY_STATEMENT);
    res += PrepareStatement(DeleteBlackEntryStatement, DELETE_BLACK_ENTRY_STATEMENT);
    res += PrepareStatement(DeleteBlackEntryStatement, DELETE_ERROR_ENTRY_STATEMENT);

    res += PrepareStatement(GetStagingEntriesStatement, GET_STAGING_ENTRIES_STATEMENT);
    res += PrepareStatement(GetBlackEntriesStatement, GET_BLACK_ENTRIES_STATEMENT);
    res += PrepareStatement(GetErrorEntriesStatement, GET_ERROR_ENTRIES_STATEMENT);

    res += PrepareStatement(GetStagingEntryUUIDFromUrlStatement, GET_STAGING_ENTRY_UUID_FROM_URL_STATEMENT);
    res += PrepareStatement(GetStagingEntryUrlFromUUIDStatement, GET_STAGING_ENTRY_URL_FROM_UUID_STATEMENT);
    res += PrepareStatement(GetBlackEntryUUIDFromUrlStatement, GET_BLACK_ENTRY_UUID_FROM_URL_STATEMENT);
    res += PrepareStatement(GetBlackEntryUrlFromUUIDStatement, GET_BLACK_ENTRY_URL_FROM_UUID_STATEMENT);

    return res;
}

int SQLiteDB::SetupDefaultBlackLibraryUsers()
{
    DBUser black_library_admin;
    DBUser black_library_librarian;
    DBUser black_library_writer;
    DBUser black_library_reader;
    DBUser black_library_no_permissions;

    black_library_admin.uid = 4007;
    black_library_admin.permission_level = DBPermissions::ReadWriteExecutePermission;
    black_library_admin.name = "BlackLibraryAdmin";

    black_library_librarian.uid = 4004;
    black_library_librarian.permission_level = DBPermissions::ReadWritePermission;
    black_library_librarian.name = "BlackLibraryLibrarian";

    black_library_writer.uid = 4003;
    black_library_writer.permission_level = DBPermissions::WritePermission;
    black_library_writer.name = "BlackLibraryWriter";

    black_library_reader.uid = 4001;
    black_library_reader.permission_level = DBPermissions::ReadPermission;
    black_library_reader.name = "BlackLibraryReader";

    black_library_no_permissions.uid = 4000;
    black_library_no_permissions.permission_level = DBPermissions::NoPermission;
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

int SQLiteDB::BeginTransaction() const
{
    char *error_msg = 0;
    // std::cout << "Begin transaction" << std::endl;
    int ret = sqlite3_exec(database_conn_, "BEGIN TRANSACTION", 0, 0, &error_msg);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error: begin transaction failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

int SQLiteDB::CheckInitialized() const
{
    if (!intialized_)
    {
        std::cout << "Error: db not initialized" << std::endl;
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
        std::cout << "Error: end transaction  failed - " << error_msg << " - " << sqlite3_errmsg(database_conn_) << std::endl;
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
        std::cout << "Error: generate table failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }
    return 0;
}

// TODO: fix this so it uses a map intead of memory mapping in order
int SQLiteDB::PrepareStatement(const std::string &statement, int statement_id)
{
    prepared_statements_.emplace_back();
    int ret = sqlite3_prepare_v2(database_conn_, statement.c_str(), -1, &prepared_statements_[statement_id], nullptr);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error: prepare failed - " << sqlite3_errmsg(database_conn_) << std::endl;
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
        std::cout << "Error: reset statement failed - " << sqlite3_errmsg(database_conn_) << std::endl;
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
        std::cout << "Error: bind of " << parameter_name << ": " << bind_int << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
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
        std::cout << "Error: bind of " << parameter_name << ": " << bind_text << " failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        ResetStatement(stmt);
        return -1;
    }

    return 0;
}

} // namespace db
} // namespace core
} // namespace black_library
