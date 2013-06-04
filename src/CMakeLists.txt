# Boost
find_package(Boost COMPONENTS system filesystem thread signals random regex program_options date_time REQUIRED)

# Wt
SET(WT_LIBRARIES wt)
SET(WT_INCLUDE_DIR /usr/include/Wt)

# Curl
SET(CURL_LIBRARIES curl)
SET(CURL_INCLUDE_DIR /usr/include/curl)

# Project
include_directories(${Boost_INCLUDE_DIR} ${WT_INCLUDE_DIR} ${CURL_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
aux_source_directory(. SRC_FILES)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${WT_CONNECTOR} ${WT_LIBRARIES} ${CURL_LIBRARIES})

INSTALL(TARGETS manjaro-web-repo
  RUNTIME DESTINATION bin
)
