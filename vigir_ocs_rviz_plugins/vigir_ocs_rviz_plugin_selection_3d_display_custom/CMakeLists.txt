# Convert the Qt Signals and Slots for QWidget events
qt4_wrap_cpp(MOC_SOURCES src/selection_3d_display_custom.h)

include_directories(${catkin_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(VIGIR_SELECTION_DISPLAY_LIB_NAME vigir_ocs_rviz_plugin_selection_3d_display_custom)

add_library(${VIGIR_SELECTION_DISPLAY_LIB_NAME}_core src/selection_3d_display_custom.cpp src/raycast_utils.cpp ${MOC_SOURCES})
target_link_libraries(${VIGIR_SELECTION_DISPLAY_LIB_NAME}_core ${catkin_LIBRARIES} ${QT_LIBRARIES})

add_dependencies(${VIGIR_SELECTION_DISPLAY_LIB_NAME}_core ${catkin_EXPORTED_TARGETS})

add_library(${VIGIR_SELECTION_DISPLAY_LIB_NAME} src/plugin_init.cpp)
target_link_libraries(${VIGIR_SELECTION_DISPLAY_LIB_NAME} ${VIGIR_SELECTION_DISPLAY_LIB_NAME}_core ${catkin_LIBRARIES})

add_dependencies(${VIGIR_SELECTION_DISPLAY_LIB_NAME} ${catkin_EXPORTED_TARGETS})

install(TARGETS ${VIGIR_SELECTION_DISPLAY_LIB_NAME} ${VIGIR_SELECTION_DISPLAY_LIB_NAME}_core
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

