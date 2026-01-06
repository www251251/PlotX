cmake_minimum_required(VERSION 3.20)

# =============================================================================
# 配置区域：请根据你的实际路径确认
# =============================================================================
set(PRELINK_EXECUTABLE "C:/Users/x/AppData/Local/.xmake/packages/p/prelink/v0.7.0/e452092dba0c43b996a53625acd3e6b7/bin/prelink.exe")
set(BEDROCK_RUNTIME_DATA "C:/Users/x/AppData/Local/.xmake/packages/b/bedrockdata/v1.21.120-server.1/397de74551ca4264b7e7ae91634783c0/bin/bedrock_runtime_data")

set(PLATFORM "windows")
set(ARCH "x64")
set(TARGET_TYPE "server")

# =============================================================================
# 工具函数：解析依赖库的绝对路径
# =============================================================================
function(resolve_prelink_libraries TARGET_NAME OUTPUT_VAR)
    set(RESOLVED_LIBS "")

    # 1. 获取目标链接的所有原始库列表 (Target, 路径, 或 库名)
    get_target_property(RAW_LIBS ${TARGET_NAME} LINK_LIBRARIES)

    # 2. 获取所有的库搜索路径 (Link Directories)
    #    包含目标私有的 link_directories 和 全局的 link_directories
    get_target_property(TARGET_DIRS ${TARGET_NAME} LINK_DIRECTORIES)
    if(NOT TARGET_DIRS)
        set(TARGET_DIRS "")
    endif()

    # 获取全局 Link Directories (通常 xmake 生成的 CMakeLists 会把库路径放这里)
    get_property(GLOBAL_DIRS DIRECTORY PROPERTY LINK_DIRECTORIES)

    # 合并搜索路径，并把 CMAKE_LIBRARY_PATH 也加进去
    set(SEARCH_DIRS ${TARGET_DIRS} ${GLOBAL_DIRS} ${CMAKE_LIBRARY_PATH})
    list(REMOVE_DUPLICATES SEARCH_DIRS)

    message(STATUS "[Prelink] Library search paths: ${SEARCH_DIRS}")

    if(RAW_LIBS)
        foreach(LIB ${RAW_LIBS})
            # 过滤 CMake 关键字
            if("${LIB}" MATCHES "^(debug|optimized|general|PRIVATE|PUBLIC|INTERFACE)$")
                continue()
            endif()

            set(FINAL_LIB_PATH "")

            # CASE A: 它是一个 CMake Target (例如 fmt::fmt 或 自建的库)
            if(TARGET "${LIB}")
                # 使用生成器表达式获取 Linker File (通常是 .lib 或 .a)
                list(APPEND RESOLVED_LIBS "$<TARGET_LINKER_FILE:${LIB}>")
                message(STATUS "[Prelink] Found Target: ${LIB} -> Using Generator Expression")
                continue() # 处理下一个
            endif()

            # CASE B: 它是一个绝对路径
            if(EXISTS "${LIB}" AND NOT IS_DIRECTORY "${LIB}")
                set(FINAL_LIB_PATH "${LIB}")

                # CASE C: 它只是一个名字 (例如 "fmt", "EconomyBridge", "LeviLamina")
                # 这是最常见的情况，也是之前失败的原因
            else()
                # 尝试查找库文件
                # 我们优先查找 .lib
                find_library(FOUND_LIB_${LIB}
                        NAMES "${LIB}" "${LIB}.lib" "lib${LIB}.lib"
                        PATHS ${SEARCH_DIRS}
                        NO_DEFAULT_PATH # 强制只在给定的路径找，模拟 xmake 行为
                )

                # 如果找不到，尝试在系统默认路径找一次兜底
                if(NOT FOUND_LIB_${LIB})
                    find_library(FOUND_LIB_${LIB} NAMES "${LIB}" "${LIB}.lib")
                endif()

                if(FOUND_LIB_${LIB})
                    set(FINAL_LIB_PATH "${FOUND_LIB_${LIB}}")
                else()
                    message(WARNING "[Prelink] Could not resolve library path for: ${LIB}. Prelink might fail for this lib.")
                endif()
            endif()

            # 如果找到了路径，加入列表
            if(FINAL_LIB_PATH)
                list(APPEND RESOLVED_LIBS "${FINAL_LIB_PATH}")
                message(STATUS "[Prelink] Resolved: ${LIB} -> ${FINAL_LIB_PATH}")
            endif()

        endforeach()
    endif()

    set(${OUTPUT_VAR} "${RESOLVED_LIBS}" PARENT_SCOPE)
endfunction()

# =============================================================================
# 主规则函数
# =============================================================================
function(add_prelink_rule TARGET_NAME)
    message(STATUS "[Prelink] Configuring prelink for target: ${TARGET_NAME}")

    set(BUILD_DIR "${CMAKE_BINARY_DIR}")
    set(OUT_DIR "${BUILD_DIR}/.prelink")
    set(LIB_DIR "${OUT_DIR}/lib")

    # === 核心改动：手动解析库路径 ===
    resolve_prelink_libraries(${TARGET_NAME} PRELINK_INPUT_LIBS)

    # 定义输出文件 (bedrock_runtime_api.lib)
    # 这样 CMake 才知道依赖关系
    set(PRELINKED_LIB "${LIB_DIR}/bedrock_runtime_api.lib")

    # 构建 prelink 命令
    add_custom_command(
            TARGET ${TARGET_NAME}
            PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DIR}"
            COMMAND "${PRELINK_EXECUTABLE}"
            "${TARGET_TYPE}-${PLATFORM}-${ARCH}"
            "${OUT_DIR}"
            "${BEDROCK_RUNTIME_DATA}"
            # 1. 所有的 .obj
            $<TARGET_OBJECTS:${TARGET_NAME}>
            # 2. 我们手动解析出来的 .lib 列表
            ${PRELINK_INPUT_LIBS}

            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "[Prelink] Scanning dependencies and generating symbols..."
            COMMAND_EXPAND_LISTS
            VERBATIM
    )

    # 链接生成的库
    # 使用 link_directories 确保 Linker 能找到它
    target_link_directories(${TARGET_NAME} PRIVATE "${LIB_DIR}")
    # 链接库名 (不需要加 .lib 后缀)
    target_link_libraries(${TARGET_NAME} PRIVATE bedrock_runtime_api)

    # DelayLoad 设置
    target_link_options(${TARGET_NAME} PRIVATE "/DELAYLOAD:bedrock_runtime.dll")

    get_target_property(_target_type ${TARGET_NAME} TYPE)

    if (_target_type STREQUAL "STATIC_LIBRARY")

        # ===== 静态库：仅清理 LIB_DIR =====
        add_custom_command(
                TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E remove_directory "${LIB_DIR}"
                VERBATIM
                COMMENT "[PostBuild] Static lib: clean LIB_DIR only"
        )

    else()
        # ===== 动态库 =====
        add_custom_command(
                TARGET ${TARGET_NAME}
                POST_BUILD
                # 清理
                COMMAND ${CMAKE_COMMAND} -E remove_directory "${LIB_DIR}"

                # 复制二进制
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "${CMAKE_SOURCE_DIR}/bin/${TARGET_NAME}"

                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${TARGET_NAME}>"
                "${CMAKE_SOURCE_DIR}/bin/${TARGET_NAME}/$<TARGET_FILE_NAME:${TARGET_NAME}>"

                # 复制 PDB（Windows）
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_PDB_FILE:${TARGET_NAME}>"
                "${CMAKE_SOURCE_DIR}/bin/${TARGET_NAME}/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.pdb"

                VERBATIM
                COMMENT "[PostBuild] Deploying to bin/..."
        )

    endif()
endfunction()