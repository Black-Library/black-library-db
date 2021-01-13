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

struct DBType {
    std::string name;
};

struct DBSource {
    std::string name;
    DBType type;
};

struct DBEntry {
    std::string UUID;
    std::string title;
    std::string nickname = "";
    std::string source;
    std::string URL;
    std::string series;
    uint16_t series_length;
    uint16_t version;
    std::string media_path;
    std::string birth_date;
    UID_rep_t user_contributed = 6;
};

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

class BlackLibraryDBConnectionInterface
{
public:
    virtual ~BlackLibraryDBConnectionInterface() {}
    // virtual DBUser ReadUser();

    virtual int CreateStagingEntry(const DBEntry &entry) const = 0;
    virtual DBEntry ReadStagingEntry(std::string UUID) const = 0;
    // virtual int UpdateStagingEntry(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    // virtual int DeleteStagingEntry(std::string UUID);

    // virtual int CreateDoc();
    // virtual int ReadDoc();
    // virtual int UpdateDoc();
    // virtual int DeleteDoc();

private:

};

} // namespace db
} // namespace core
} // namespace black_library

#endif
