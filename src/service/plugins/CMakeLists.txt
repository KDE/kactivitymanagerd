# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

function(kamd_add_plugin)
   kcoreaddons_add_plugin(${ARGN} INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR})
endfunction()
add_subdirectory (slc)
add_subdirectory (sqlite)
add_subdirectory (activitytemplates)
add_subdirectory (virtualdesktopswitch)
add_subdirectory (globalshortcuts)
add_subdirectory (recentlyused-eventspy)
add_subdirectory (runapplication)
add_subdirectory (krunner)
