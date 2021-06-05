/**
 * db_driver.cc
 */

#include <getopt.h>
#include <iostream>
#include <string.h>

#include <BlackLibraryDB.h>

struct options
{
    std::string db_path = "";
    bool intialize_db = false;
};

static void Usage(const char *prog)
{
    const char *p = strchr(prog, '/');
    printf("usage: %s --(p)ath --[i]nit_db [-h]\n", p ? (p + 1) : prog);
}

static int ParseOptions(int argc, char **argv, struct options *opts)
{
    static const char *const optstr = "hip:";
    static const struct option long_opts[] = {
        { "help", no_argument, 0, 'h' },
        { "init_db", no_argument, 0, 'i' },
        { "path", required_argument, 0, 'p' },
        { "verbose", no_argument, 0, 'v' },
        { 0, 0, 0, 0 }
    };

    if (!argv || !opts)
        return -1;

    int opt = 0;
    while ((opt = getopt_long(argc, argv, optstr, long_opts, 0)) >= 0)
    {
        switch (opt)
        {
            case 'h':
                Usage(argv[0]);
                exit(0);
                break;
            case 'i':
                opts->intialize_db = true;
                break;
            case 'p':
                opts->db_path = std::string(optarg);
                break;
            default:
                exit(1);
                break;
        }
    }

    if (optind < argc)
    {
        fprintf(stderr, "trailing options..\n");
        exit(1);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    struct options opts;

    if (argc < 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    if (ParseOptions(argc, argv, &opts))
    {
        Usage(argv[0]);
        exit(1);
    }

    black_library::core::db::DBEntry create_staging_entry;
    black_library::core::db::DBEntry read_staging_entry;
    black_library::core::db::DBEntry update_staging_entry;

    std::cout << "Starting db" << std::endl;
    black_library::core::db::BlackLibraryDB blacklibrarydb(opts.db_path, opts.intialize_db);

    create_staging_entry.UUID = "55ee59ad-2feb-4196-960b-3226c65c80d5";
    create_staging_entry.title = "foo2";
    create_staging_entry.nickname = "foo3";
    create_staging_entry.source = "foo4";
    create_staging_entry.url = "foo5";
    create_staging_entry.last_url = "foo8";
    create_staging_entry.series = "the foo";
    create_staging_entry.series_length = 2;
    create_staging_entry.version = 1;
    create_staging_entry.media_path = "foo6";
    create_staging_entry.birth_date = "foo7";
    create_staging_entry.user_contributed = 4004;

    blacklibrarydb.CreateStagingEntry(create_staging_entry);

    // second time should fail
    blacklibrarydb.CreateStagingEntry(create_staging_entry);

    read_staging_entry = blacklibrarydb.ReadStagingEntry("55ee59ad-2feb-4196-960b-3226c65c80d5");

    std::cout << read_staging_entry << std::endl;

    bool check0 = blacklibrarydb.DoesStagingEntryUrlExist("foo5");
    bool check1 = blacklibrarydb.DoesStagingEntryUrlExist("foo6");

    std::cout << "url foo5 exists: " << check0 << std::endl;
    std::cout << "url foo6 exists: " << check1 << std::endl;

    // update read_staging_entry
    read_staging_entry.title = "2foo";
    read_staging_entry.nickname = "3foo";
    read_staging_entry.source = "4foo";
    read_staging_entry.url = "5foo";
    read_staging_entry.last_url = "8foo";
    read_staging_entry.series = "the bar";
    read_staging_entry.series_length = 3;
    read_staging_entry.version = 2;
    read_staging_entry.media_path = "6foo";
    read_staging_entry.birth_date = "7foo";
    read_staging_entry.user_contributed = 4004;

    blacklibrarydb.UpdateStagingEntry(read_staging_entry);

    update_staging_entry = blacklibrarydb.ReadStagingEntry("55ee59ad-2feb-4196-960b-3226c65c80d5");

    std::cout << read_staging_entry << std::endl;

    blacklibrarydb.DeleteStagingEntry("55ee59ad-2feb-4196-960b-3226c65c80d5");
    blacklibrarydb.DeleteStagingEntry("75ee5fad-2deb-4436-120c-3226ceeeaed6");

    read_staging_entry = blacklibrarydb.ReadStagingEntry("55ee59ad-2feb-4196-960b-3226c65c80d5");

    std::cout << read_staging_entry << std::endl;

    std::cout << "Closing application" << std::endl;

    return 0;
}
