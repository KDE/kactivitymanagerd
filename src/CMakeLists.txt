
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Installation directory for ontologies.  Needed here because used in both ontology/ and lib/ sub
# directories.

set(KACTIVITIES_ONTOLOGIES_DIR ${CMAKE_INSTALL_PREFIX}/share/ontology/kde)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

# Testing for C++0x/C++11 features

include(CheckCxxFeatures)

CXX_CHECK_FEATURE ("c++11" "auto"               "N2546" HAVE_CXX11_AUTO               "${ADDITIONAL_DEFINITIONS}")
CXX_CHECK_FEATURE ("c++11" "nullptr"            "N2431" HAVE_CXX11_NULLPTR            "${ADDITIONAL_DEFINITIONS}")
CXX_CHECK_FEATURE ("c++11" "lambda"             "N2927" HAVE_CXX11_LAMBDA             "${ADDITIONAL_DEFINITIONS}")
CXX_CHECK_FEATURE ("c++11" "override"           "N3206" HAVE_CXX11_OVERRIDE           "${ADDITIONAL_DEFINITIONS}")
CXX_CHECK_FEATURE ("c++11" "unique_ptr"         "none"  HAVE_CXX11_UNIQUE_PTR         "${ADDITIONAL_DEFINITIONS}")
CXX_CHECK_FEATURE ("c++11" "variadic-templates" "N2242" HAVE_CXX11_VARIADIC_TEMPLATES "${ADDITIONAL_DEFINITIONS}")

add_subdirectory(lib)

string (COMPARE EQUAL "${CXX_FEATURES_UNSUPPORTED}" "" CXX_COMPILER_IS_MODERN)

if (NOT HAVE_CXX11_AUTO OR NOT HAVE_CXX11_LAMBDA OR NOT HAVE_CXX11_UNIQUE_PTR OR NOT HAVE_CXX11_VARIADIC_TEMPLATES)

    macro_log_feature (FALSE
        "C++11 enabled compiler"
        "GCC >=4.5 or Clang 3.1"
        "http://www.open-std.org/jtc1/sc22/wg21/" FALSE ""
        "REQUIRED: You need a more modern compiler in order to build the Activity Manager daemon"
        )

else ()

    if (CXX_COMPILER_IS_MODERN)
        macro_log_feature (
            CXX_COMPILER_IS_MODERN
            "C++11 enabled compiler"
            "Your compiler is state-of-the-art"
            "http://www.open-std.org/jtc1/sc22/wg21/" FALSE ""
            "Your compiler doesn't support the following features: ${CXX_FEATURES_UNSUPPORTED} but
             the list of the supported ones is sufficient for the build: ${CXX_FEATURES_SUPPORTED}"
            )
    else ()
        macro_log_feature (
            CXX_COMPILER_IS_MODERN
            "C++11 enabled compiler"
            "Having to use the latest versions of GCC and Clang would be awesome"
            "http://www.open-std.org/jtc1/sc22/wg21/" FALSE ""
            "Your compiler doesn't support the following features: ${CXX_FEATURES_UNSUPPORTED} but
             the list of the supported ones is sufficient for the build: ${CXX_FEATURES_SUPPORTED}"
            )
    endif ()

    add_subdirectory(service)

endif ()

add_subdirectory(ontologies)
add_subdirectory(workspace)
