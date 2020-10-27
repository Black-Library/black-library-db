/**
 * BlackLibraryDBConnectionInterface.hh
 */

#ifndef __LIBRARYCORE_BLACKLIBRARYDBCONNECTIONINTERFACE_HH__
#define __LIBRARYCORE_BLACKLIBRARYDBCONNECTIONINTERFACE_HH__

#include <string>

namespace librarycore {

namespace librarydb {

typedef uint16_t UID_rep;

struct DBUser {
    UID_rep UID;
    uint8_t permission_level = 0;
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
    std::string name;
    std::string nickname;
    std::string source;
    std::string URL;
    uint16_t version;
    std::string media_path;
    int birth_date;
    UID_rep user_contributed;
};

struct DBRating {
    std::string UUID;
    UID_rep UID;
    uint16_t rating;
};

struct DBUserProgress {
    std::string UUID;
    UID_rep UID;
    uint16_t number;
    uint16_t chapter;
    uint32_t page;
};

class BlackLibraryDBConnectionInterface
{
public:
    // virtual dbUser ReadUser();

    virtual int CreateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    virtual DBEntry ReadStagingDoc(std::string UUID);
    virtual int UpdateStagingDoc(std::string UUID, std::string title, std::string source, std::string URL, int uid, std::string nickname = "");
    virtual int DeleteStagingDoc(std::string UUID);

    // virtual int CreateDoc();
    // virtual int ReadDoc();
    // virtual int UpdateDoc();
    // virtual int DeleteDoc();

private:

};

} // namespace librarydb
} // namespace librarycore

#endif
