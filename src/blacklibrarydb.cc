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

    return 0;
}
