find_package(SQLite3 REQUIRED)

message("SQLite3 include dir: ${SQLite3_INCLUDE_DIRS}")

include(GNUInstallDirs)

add_library(BlackLibraryDB BlackLibraryDB.cc SQLiteDB.cc)
target_link_libraries(BlackLibraryDB ${SQLite3_LIBRARY})
target_include_directories(BlackLibraryDB PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

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
