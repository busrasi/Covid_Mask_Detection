#
# Find lib library.
#
# DLIB_FOUND - system has found AST library
# DLIB_DIRS - the AST include directory
# DLIB_LIBRARIES - the AST static library
include(ProjectEnvironment)

##################################
# Includes
##################################
find_path(DLIB_DIR
	NAMES dlib/any
	PATHS ${TC_ARTIFACTS_SEARCH_DIR}/include/
	NO_DEFAULT_PATH
	NO_CMAKE_FIND_ROOT_PATH
)

set(DLIB_DIRS ${DLIB_DIR})

##################################
# Static Libraries
##################################
include(SelectLibraryConfigurations)


##################################
# FIND DLIB
##################################
find_library(DLIB_LIBRARY_DEBUG
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}dlib${CMAKE_STATIC_LIBRARY_SUFFIX}
        PATHS ${TC_ARTIFACTS_SEARCH_DIR}/lib
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH
)

find_library(DLIB_LIBRARY_RELEASE
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}dlib${CMAKE_RELEASE_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
        PATHS ${TC_ARTIFACTS_SEARCH_DIR}/lib
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH
)

select_library_configurations(DLIB)

set(DLIB_LIBRARIES ${DLIB_LIBRARY})


###################################
# Setup package
###################################
include(FeatureSummary)
include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

# handle the QUIETLY and REQUIRED arguments and set DLIB_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Dlib
	DEFAULT_MSG
	DLIB_LIBRARIES
	DLIB_DIRS
)

SET_PACKAGE_PROPERTIES(Dlib PROPERTIES
    DESCRIPTION "(search path <${TC_ARTIFACTS_SEARCH_DIR}>)"
)

mark_as_advanced(
    DLIB_DEBUG
    DLIB_RELEASE
    DLIB_DIR
)

##################################
# Dependencies
##################################