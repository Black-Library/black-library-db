/**
 * SQLiteDB.cc
 */

#include <iostream>

#include <SQLiteDB.hh>

namespace black_library {

namespace core {

namespace db {

SQLiteDB::SQLiteDB(const std::string &database_url) :
    database_conn_(),
    database_url_(database_url)
{
    // if (database_url_.empty())
    //     database_url_ = "file:/localhost/mnt/db/catalog.db";

    if (database_url_.empty())
        database_url_ = "/home/benjamin/bl-parsers/BlackLibraryDB/tools/sqlite3/catalog.db";

    int res = sqlite3_open(database_url_.c_str(), &database_conn_);
    
    if (res != SQLITE_OK)
    {
        std::cout << "Error, failed to open db at: " << database_url_ << " - " << sqlite3_errmsg(database_conn_) << std::endl;
        return;
    }

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

int SQLiteDB::CreateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname) const
{
    std::cout << UUID << title << source << URL << uid << nickname << std::endl;
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

int SQLiteDB::PrepareStatements()
{
    if (!database_conn_)
        return -1;

    prepared_statements_.emplace_back();
    int ret = sqlite3_prepare_v2(database_conn_, "SELECT * FROM user", -1, &prepared_statements_[0], nullptr);
    if (ret != SQLITE_OK)
    {
        std::cout << "Error, prepare failed - " << sqlite3_errmsg(database_conn_) << std::endl;
        return -1;
    }

    return 0;
}

} // namespace db
} // namespace core
} // namespace black_library
