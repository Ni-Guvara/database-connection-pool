# 收集源文件
file(GLOB SOURCES "*.cpp")

# set(LIB_LIBRARY lib${PROJECT_NAME})
set(SO_LIBRARY ${PROJECT_NAME})

# 生成动态库
add_library(${SO_LIBRARY} SHARED ${SOURCES})

# 生成静态库
# add_library(${LIB_LIBRARY} STATIC ${SOURCES})

# 安装动态库
install(TARGETS ${SO_LIBRARY}
    LIBRARY DESTINATION lib # 动态库安装路径
)

# 安装静态库
# install(TARGETS ${LIB_LIBRARY}
# ARCHIVE DESTINATION lib # 静态库安装路径
# )

# 安装头文件（如果需要）
install(DIRECTORY ../include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
