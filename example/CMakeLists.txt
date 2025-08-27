aux_source_directory(./ SRC_LIST)
add_executable(test ${SRC_LIST})
target_link_libraries(test PRIVATE CommonConnectionPool mysqlclient pthread)

# 安装可执行文件
install(TARGETS test
    RUNTIME DESTINATION bin # 可执行文件安装路径
)
