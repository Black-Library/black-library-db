/**
 * BlackLibraryDBInterface.hh
 */

#ifndef __LIBRARYCORE_BLACKLIBRARYDBINTERFACE_HH__
#define __LIBRARYCORE_BLACKLIBRARYDBINTERFACE_HH__

#include <string>

namespace librarycore {

class BlackLibraryDBInterface
{
public:
    // virtual int ReadUser();

    virtual int CreateStagingDoc(std::string name, std::string source, std::string URL, int uid, std::string nickname = "");
    virtual int ReadStagingDoc(std::string UUID);
    virtual int UpdateStagingDoc(std::string UUID, std::string name, std::string source, std::string URL, int uid, std::string nickname = "");
    virtual int DeleteStagingDoc(std::string UUID);

    // virtual int CreateDoc();
    // virtual int ReadDoc();
    // virtual int UpdateDoc();
    // virtual int DeleteDoc();

protected:
    std::string GenerateUUID();

private:

};

} // librarycore namespace

#endif
