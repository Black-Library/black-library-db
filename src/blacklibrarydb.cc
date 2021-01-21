/**
 * blacklibrarydb.cc
 */

#include <getopt.h>
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

    black_library::core::db::DBEntry create_staging_entry;
    black_library::core::db::DBEntry read_staging_entry;

    std::cout << "Starting db" << std::endl;
    black_library::core::db::BlackLibraryDB *blacklibrarydb = new black_library::core::db::BlackLibraryDB("/mnt/db/catalog.db", true);

    create_staging_entry.UUID = "55ee59ad-2feb-4196-960b-3226c65c80d5";
    create_staging_entry.title = "foo2";
    create_staging_entry.nickname = "foo3";
    create_staging_entry.source = "foo4";
    create_staging_entry.URL = "foo5";
    create_staging_entry.series = "the foo";
    create_staging_entry.series_length = 2;
    create_staging_entry.version = 1;
    create_staging_entry.media_path = "foo6";
    create_staging_entry.birth_date = "foo7";
    create_staging_entry.user_contributed = 4004;

    blacklibrarydb->CreateStagingEntry(create_staging_entry);

    // second time should fail
    blacklibrarydb->CreateStagingEntry(create_staging_entry);

    read_staging_entry = blacklibrarydb->ReadStagingEntry("55ee59ad-2feb-4196-960b-3226c65c80d5");

    std::cout << "\tUUID: " << read_staging_entry.UUID << std::endl;
    std::cout << "\ttitle: " << read_staging_entry.title << std::endl;
    std::cout << "\tnickname: " << read_staging_entry.nickname << std::endl;
    std::cout << "\tsource: " << read_staging_entry.source << std::endl;
    std::cout << "\tURL: " << read_staging_entry.URL << std::endl;
    std::cout << "\tseries: " << read_staging_entry.series << std::endl;
    std::cout << "\tseries_length: " << read_staging_entry.series_length << std::endl;
    std::cout << "\tversion: " << read_staging_entry.version << std::endl;
    std::cout << "\tmedia_path: " << read_staging_entry.media_path << std::endl;
    std::cout << "\tbirth_date: " << read_staging_entry.birth_date << std::endl;
    std::cout << "\tuser_contributed: " << read_staging_entry.user_contributed << std::endl;

    black_library::core::db::DBUrlCheck check0 = blacklibrarydb->DoesStagingEntryUrlExist("foo5");
    black_library::core::db::DBUrlCheck check1 = blacklibrarydb->DoesStagingEntryUrlExist("foo6");

    std::cout << "URL foo5 exists: " << check0.exists << std::endl;
    std::cout << "URL foo6 exists: " << check1.exists << std::endl;

    std::cout << "Closing application" << std::endl;

    return 0;
}
