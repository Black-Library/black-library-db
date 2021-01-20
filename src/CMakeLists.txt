message(STATUS "Searching for sqlite3.h")
find_path( SQLite3_INCLUDE_DIR sqlite3.h )
message(STATUS "Searching for sqlite3.h - ${SQLite3_INCLUDE_DIR}")

message(STATUS "Searching for libsqlite3")
find_library( SQLite3_LIBRARY sqlite3 )
message(STATUS "Searching for libsqlite3 - ${SQLite3_LIBRARY}")

include(GNUInstallDirs)

add_library(BlackLibraryDB BlackLibraryDB.cc SQLiteDB.cc)
target_link_libraries(BlackLibraryDB ${SQLite3_LIBRARY})
target_include_directories(BlackLibraryDB PUBLIC ${SQLite3_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)

install(
  TARGETS BlackLibraryDB
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_executable(blacklibrarydb blacklibrarydb.cc)

target_link_libraries(blacklibrarydb BlackLibraryDB)

set_target_properties(blacklibrarydb PROPERTIES INSTALL_RPATH
    "$LD_LIBRARY_PATH;${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS blacklibrarydb RUNTIME DESTINATION bin)
