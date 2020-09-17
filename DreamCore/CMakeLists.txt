cmake_minimum_required (VERSION 3.0)

project(DreamCore)

file(GLOB_RECURSE SRC_FILES "*.c" "*.cpp" "*.h" "*.hpp")

find_package(ALUT REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(BULLET REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OPENGL REQUIRED)


include_directories(
    ${ALUT_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${OGG_INCLUDE_DIR}
    ${VORBIS_INCLUDE_DIR}

	${DEP_ANGELSCRIPT_INC_DIR}
	${DEP_ASSIMP_INC_DIR}
	${DEP_GLAD_INC_DIR}
    ${DEP_GLM_INC_DIR}
	${DEP_JSON_INC_DIR}
	${DEP_SOIL_INC_DIR}
    ${DEP_SPDLOG_INC_DIR}
	${DEP_TWEENY_INC_DIR}
	${DEP_TINYSPLINE_INC_DIR}
)

if (WIN32)
    add_library(
        DreamCore
        ${SRC_FILES}

	    ${DEP_ANGELSCRIPT_SRC_FILES}
		${DEP_GLAD_SRC_FILES}
		${DEP_SOIL_SRC_FILES}
		${DEP_TINYSPLINE_SRC_FILES}
		${DEP_SPDLOG_SRC_FIELS}
    )
else()
    add_library(
        DreamCore
        SHARED
        ${SRC_FILES}
		${DEP_ANGELSCRIPT_SRC_FILES}
		${DEP_GLAD_SRC_FILES}
		${DEP_SOIL_SRC_FILES}
		${DEP_TINYSPLINE_SRC_FILES}
		${DEP_SPDLOG_SRC_FIELS}
    )
endif()

IF (WIN32)
    target_link_libraries(
        DreamCore
        ${ALUT_LIBRARY}
		${ASSIMP_LIBRARIES}
        ${BULLET_LIBRARY}
        ${OGGVORBIS_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${OPENGL_LIBRARIES}
    )
ELSEIF (UNIX AND NOT APPLE) # Linux
    target_link_libraries(
         DreamCore
         -lpthread
         -ldl
        ${ALUT_LIBRARY}
		${ASSIMP_LIBRARIES}
        ${BULLET_LIBRARY}
        ${OGGVORBIS_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${OPENGL_LIBRARIES}
    )
ELSEIF(APPLE)
    target_link_libraries(
        DreamCore
        -lpthread
        -ldl
        "-framework CoreFoundation"
        ${ALUT_LIBRARY}
		${ASSIMP_LIBRARIES}
        ${BULLET_LIBRARY}
        ${OGGVORBIS_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${OPENGL_LIBRARIES}
    )
ENDIF()
