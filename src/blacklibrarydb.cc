/**
 * blacklibrarydb.cc
 */

#include <iostream>

#include <BlackLibraryDB.hh>

int main(int argc, char* argv[])
{
    // check to see if there are any extra arguments
    if (argc > 1)
    {
        std::cout << "extra arguments ";
        for (int i = 0; i < argc; ++i)
        {
            std::cout << std::string(argv[i]);
        }
        std::cout << std::endl;
    }

    black_library::core::db::DBEntry staging_entry;

    std::cout << "Starting db" << std::endl;
    black_library::core::db::BlackLibraryDB *blacklibrarydb = new black_library::core::db::BlackLibraryDB("");

    staging_entry.UUID = "55ee59ad-2feb-4196-960b-3226c65c80f5";
    staging_entry.title = "foo2";
    staging_entry.nickname = "foo3";
    staging_entry.source = "foo4";
    staging_entry.URL = "foo5";
    staging_entry.version = 1;
    staging_entry.media_path = "foo6";
    staging_entry.birth_date = "foo7";
    staging_entry.user_contributed = 2;

    blacklibrarydb->CreateStagingEntry(staging_entry);

    std::cout << "Ending db" << std::endl;

    return 0;
}
