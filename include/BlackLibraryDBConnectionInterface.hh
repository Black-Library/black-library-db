/**
 * BlackLibraryDBConnectionInterface.hh
 */

#ifndef __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBCONNECTIONINTERFACE_HH__
#define __BLACK_LIBRARY_CORE_DB_BLACKLIBRARYDBCONNECTIONINTERFACE_HH__

#include <string>

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
    DOCUMENT,
    IMAGE_GALLERY,
    VIDEO,
    _NUM_DB_ENTRY_MEDIA_TYPES
} db_entry_media_type;

typedef uint8_t db_entry_media_type_rep_t;

struct DBSource {
    std::string name;
    db_entry_media_type_rep_t type;
};

typedef enum {
    BLACK_ENTRY,
    STAGING_ENTRY,
    _NUM_ENTRY_TYPES
} db_entry_type;

typedef uint8_t db_entry_type_rep_t;

struct DBEntry {
    std::string UUID;
    std::string title;
    std::string nickname = "";
    std::string source;
    std::string URL;
    std::string series;
    uint16_t series_length = 1;
    uint16_t version = 1;
    std::string media_path;
    std::string birth_date;
    UID_rep_t user_contributed = 6;
};

struct DBRating {
    std::string UUID;
    UID_rep_t UID;
    uint16_t rating;
};

struct DBUrlCheck {
    int error = 0;
    bool exists = true;
};

struct DBUserProgress {
    std::string UUID;
    UID_rep_t UID;
    uint16_t number;
    uint16_t chapter;
    uint32_t page;
};

class BlackLibraryDBConnectionInterface
{
public:
    virtual ~BlackLibraryDBConnectionInterface() {}
    // virtual DBUser ReadUser();

    virtual int CreateEntry(const DBEntry &entry, db_entry_type_rep_t entry_type) const = 0;
    virtual DBEntry ReadEntry(const std::string &UUID, db_entry_type_rep_t entry_type) const = 0;
    virtual DBUrlCheck DoesEntryUrlExist(const std::string &URL, db_entry_type_rep_t entry_type) const = 0;
    virtual int UpdateEntry(const std::string &UUID, const DBEntry &entry, db_entry_type_rep_t entry_type) const = 0;
    virtual int DeleteEntry(std::string UUID, db_entry_type_rep_t entry_type) const = 0;

private:

};

} // namespace db
} // namespace core
} // namespace black_library

#endif
