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


typedef enum {
    NO_PERMISSIONS,
    READ_PERMISSIONS,
    WRITE_PERMISSIONS,
    READ_WRITE_PERMISSIONS,
    EXECUTE_PERMISSIONS,
    READ_EXECUTE_PERMISSIONS,
    WRITE_EXECUTE_PERMISSIONS,
    READ_WRITE_EXECUTE_PERMISSIONS,
    _NUM_PERMSSIONS
} permissions_t;

typedef uint8_t permissions_rep_t;

typedef uint16_t UID_rep_t;

struct DBUser {
    UID_rep_t UID;
    permissions_rep_t permission_level = NO_PERMISSIONS;
    std::string name = "";
};

typedef enum {
    DB_ENTRY_MEDIA_ERROR,
    DOCUMENT,
    IMAGE_GALLERY,
    VIDEO,
    _NUM_DB_ENTRY_MEDIA_TYPES
} entry_media_t;

typedef uint8_t entry_media_rep_t;

struct DBSource {
    std::string name;
    entry_media_rep_t media_type;
};

typedef enum {
    BLACK_ENTRY,
    STAGING_ENTRY,
    _NUM_ENTRY_TYPES
} entry_table_t;

typedef uint8_t entry_table_rep_t;

struct DBEntry {
    std::string UUID;
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
    std::string update_date;
    UID_rep_t user_contributed = 6;
};

#define NUM_ENTRY_COLUMNS 14;

inline std::ostream& operator<< (std::ostream &out, const DBEntry &entry)
{
    out << "UUID: " << entry.UUID << " ";
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
    out << "update_date: " << entry.update_date << " ";
    out << "user_contributed: " << entry.user_contributed << " ";

    return out;
}

struct DBRating {
    std::string UUID;
    UID_rep_t UID;
    uint16_t rating;
};

struct DBUserProgress {
    std::string UUID;
    UID_rep_t UID;
    uint16_t number;
    uint16_t chapter;
    uint32_t page;
};

struct DBStringResult {
    std::string result = "";
    std::string error_string = "";
    bool does_not_exist = false;
    int error = 0;
};

struct DBBoolResult {
    bool result = false;
    bool does_not_exist = false;
    int error = 0;
};

} // namespace db
} // namespace core
} // namespace black_library

#endif
