cmake_minimum_required(VERSION 3.20)

set(PRELINK_EXECUTABLE "C:/Users/x/AppData/Local/.xmake/packages/p/prelink/v0.7.0/e452092dba0c43b996a53625acd3e6b7/bin/prelink.exe")
set(BEDROCK_RUNTIME_DATA "C:/Users/x/AppData/Local/.xmake/packages/b/bedrockdata/v1.21.120-server.1/397de74551ca4264b7e7ae91634783c0/bin/bedrock_runtime_data")

set(PLATFORM "windows")
set(ARCH "x64")
set(TARGET_TYPE "server")

function(add_prelink_rule TARGET_NAME)
    message(STATUS "[prelink_rule] Enable prelink for target: ${TARGET_NAME}")

    set(BUILD_DIR "${CMAKE_BINARY_DIR}")
    set(OUT_DIR "${BUILD_DIR}/.prelink")
    set(LIB_DIR "${OUT_DIR}/lib")
    file(MAKE_DIRECTORY "${LIB_DIR}")

    # 收集静态库依赖
    set(INPUT_LIBS "")
    get_target_property(LINKED_LIBS ${TARGET_NAME} LINK_LIBRARIES)
    if(LINKED_LIBS)
        foreach(LIB ${LINKED_LIBS})
            if(TARGET ${LIB})
                get_target_property(LIB_FILE ${LIB} LOCATION)
                if(EXISTS "${LIB_FILE}")
                    list(APPEND INPUT_LIBS "${LIB_FILE}")
                endif()
            elseif(EXISTS "${LIB}")
                list(APPEND INPUT_LIBS "${LIB}")
            else()
                find_library(LIB_PATH NAMES ${LIB} PATHS ${CMAKE_LIBRARY_PATH})
                if(LIB_PATH)
                    list(APPEND INPUT_LIBS "${LIB_PATH}")
                endif()
            endif()
        endforeach()
    endif()

    # prelink command，用 generator expression 直接展开 object 文件
    add_custom_command(
            TARGET ${TARGET_NAME}
            PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DIR}"
            COMMAND "${PRELINK_EXECUTABLE}"
            "${TARGET_TYPE}-${PLATFORM}-${ARCH}"
            "${OUT_DIR}"
            "${BEDROCK_RUNTIME_DATA}"
            $<TARGET_OBJECTS:${TARGET_NAME}>
            ${INPUT_LIBS}
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "[prelink_rule] Running prelink for ${TARGET_NAME}..."
            COMMAND_EXPAND_LISTS
            VERBATIM
    )

    # 链接 bedrock_runtime_api.lib
    target_link_directories(${TARGET_NAME} PRIVATE "${LIB_DIR}")
    target_link_libraries(${TARGET_NAME} PRIVATE bedrock_runtime_api)

    # 添加 delay load
    target_link_options(${TARGET_NAME} PRIVATE "/DELAYLOAD:bedrock_runtime.dll")

    # 构建后清理
    add_custom_command(
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${LIB_DIR}"
            COMMENT "[prelink_rule] Cleaning up ${LIB_DIR}..."
    )
endfunction()
