/**
 * SQLiteDB.cc
 */

#include <SQLiteDB.hh>

namespace librarycore {

namespace librarydb {

SQLiteDB::SQLiteDB() :
    db_conn_(),
    uri_()
{
    if (uri_.empty())
        uri_ = "file:/localhost/mnt/db/catalog.db";

    int res = sqlite3_open(uri_.c_str(), &db_conn_);
    
    // if (res > 0)
    // TODO handle error  
}

SQLiteDB::~SQLiteDB()
{
    if (db_conn_)
        sqlite3_close(db_conn_);
}

int SQLiteDB::CreateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "")
{
    return 0;
}

DBEntry SQLiteDB::ReadStagingDoc(std::string UUID)
{
    DBEntry entry;

    return entry;
}

int SQLiteDB::UpdateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "")
{
    return 0;
}

int SQLiteDB::DeleteStagingDoc(std::string UUID)
{
    return 0;
}

} // namespace librarydb
} // namespace librarycore
