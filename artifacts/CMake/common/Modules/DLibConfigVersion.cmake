set(PACKAGE_VERSION "19.21.99")
 
# Check whether the requested PACKAGE_FIND_VERSION is compatible
if("${PACKAGE_FIND_VERSION_MAJOR}" EQUAL 19)
    if("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
        set(PACKAGE_VERSION_COMPATIBLE FALSE) # requested version is less than 19.0
    else()
        set(PACKAGE_VERSION_COMPATIBLE TRUE) # compatible with any version 19.x
        if ("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
            set(PACKAGE_VERSION_EXACT TRUE) # exact match for version 19.0
        endif()
    endif()
else()
    set(PACKAGE_VERSION_COMPATIBLE FALSE) # requested version doesn't match the available major version 1
endif()
