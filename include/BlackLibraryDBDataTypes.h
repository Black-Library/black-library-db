/**
 * BlackLibraryDBDataTypes.h
 */

#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBDATATYPES_H__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBDATATYPES_H__

#include <string>
#include <sstream>

namespace black_library {

namespace core {

namespace db {

enum class DBPermissions : uint8_t {
    NoPermission = 0,
    ReadPermission,
    WritePermission,
    ReadWritePermission,
    ExecutePermission,
    ReadExecutePermisson,
    WriteExecutePermission,
    ReadWriteExecutePermission,
    _NUM_PERMSSIONS
};

typedef uint16_t UID_rep_t;

struct DBUser {
    UID_rep_t uid;
    DBPermissions permission_level = DBPermissions::NoPermission;
    std::string name = "";
};

enum class DBEntryMediaType : uint8_t {
    DBEntryMediaTypeError = 0,
    Document,
    ImageGallery,
    Video,
    _NUM_DB_ENTRY_MEDIA_TYPES
};

struct DBSource {
    std::string name;
    DBEntryMediaType media_type;
};

typedef enum {
    BLACK_ENTRY,
    STAGING_ENTRY,
    _NUM_ENTRY_TYPES
} entry_table_t;

typedef uint8_t entry_table_rep_t;

struct DBEntry {
    std::string uuid;
    std::string title;
    std::string author;
    std::string nickname = "";
    std::string source;
    std::string url;
    std::string last_url;
    std::string series;
    uint16_t series_length = 1;
    uint16_t version = 1;
    std::string media_path;
    std::string birth_date;
    std::string check_date;
    std::string update_date;
    UID_rep_t user_contributed = 6;
};

inline std::ostream& operator<< (std::ostream &out, const DBEntry &entry)
{
    out << "UUID: " << entry.uuid << " ";
    out << "title: " << entry.title << " ";
    out << "author: " << entry.author << " ";
    out << "nickname: " << entry.nickname << " ";
    out << "source: " << entry.source << " ";
    out << "url: " << entry.url << " ";
    out << "last_url: " << entry.last_url << " ";
    out << "series: " << entry.series << " ";
    out << "series_length: " << entry.series_length << " ";
    out << "version: " << entry.version << " ";
    out << "media_path: " << entry.media_path << " ";
    out << "birth_date: " << entry.birth_date << " ";
    out << "check_date: " << entry.check_date << " ";
    out << "update_date: " << entry.update_date << " ";
    out << "user_contributed: " << entry.user_contributed;

    return out;
}

struct DBRating {
    std::string uuid;
    UID_rep_t uid;
    uint16_t rating;
};

struct DBUserProgress {
    std::string uuid;
    UID_rep_t uid;
    uint16_t number;
    uint16_t chapter;
    uint32_t page;
};

struct DBStringResult {
    std::string result = "";
    std::string debug_string = "";
    bool does_not_exist = false;
    int error = 0;
};

struct DBBoolResult {
    bool result = false;
    bool does_not_exist = false;
    int error = 0;
};

struct ErrorEntry {
    std::string uuid;
    size_t progress_num;
};

inline std::ostream& operator<< (std::ostream &out, const ErrorEntry &entry)
{
    out << "UUID: " << entry.uuid << " ";
    out << "progress_num: " << entry.progress_num;

    return out;
}

} // namespace db
} // namespace core
} // namespace black_library

#endif
