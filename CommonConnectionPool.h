#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <memory>
#include <functional>
#include "Connection.h"
using namespace std;

class CommonConnectionPool
{
public:
	static CommonConnectionPool* getConnectionPool();  // 单例模式-懒汉模式
	shared_ptr<Connection> getConnection();

private:
	CommonConnectionPool();

	bool loadConfigFile();

	void produceConnectionTask();
	void scanIdleConnection();

	string _ip;  // mysql ip地址
	unsigned short _port; // mysql 端口号
	string _username; // 用户名
	string _password; // 密码
	string _dbname;

	int _initSize;  // 连接池连接数量初始化
	int _maxSize;   // 最大连接数量
	int _maxIdleTime; // 最大空闲时间
	int _connectionTimeOut;  // 连接最大时间

	atomic_int _connectionCnt; // 记录创建的连接数量
	queue<Connection*> _connectionQuque;
	mutex _queueMutex;
	condition_variable cv; // 生产线程-消费线程通信 设置条件变量
};
