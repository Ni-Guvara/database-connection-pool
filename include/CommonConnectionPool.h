#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <memory>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <thread>

#include "Connection.h"

using namespace std;

class CommonConnectionPool
{
public:
	static CommonConnectionPool *getConnectionPool();
	shared_ptr<Connection> getConnection();

private:
	CommonConnectionPool();

	bool loadConfigFile();

	void produceConnectionTask();
	void scanIdleConnection();

	string _ip;
	unsigned short _port;
	string _username;
	string _password;
	string _dbname;

	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeOut;

	atomic_int _connectionCnt;
	queue<Connection *> _connectionQuque;
	mutex _queueMutex;
	condition_variable cv;
};
