add_library(libcurl SHARED EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_SOURCE_DIR}/Export.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Strings.cpp
)
target_include_directories(libcurl PRIVATE ${LIBS_INCLUDE_DIR})
configure_windows_unwind(libcurl)

if(BUILD_TESTING)
    add_test_executable(guest_curl_strequal_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/Strequal.cpp)
    target_include_directories(guest_curl_strequal_tests PRIVATE ${LIBS_INCLUDE_DIR})
    target_link_libraries(guest_curl_strequal_tests PRIVATE libcurl libc)
    configure_windows_unwind(guest_curl_strequal_tests)
    add_test(NAME guest_curl_strequal COMMAND guest_curl_strequal_tests)
    set_tests_properties(guest_curl_strequal PROPERTIES TIMEOUT 20)
    if(WIN32)
        set_tests_properties(guest_curl_strequal PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libcurl>")
    endif()
endif()
