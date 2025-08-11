#include "CommonConnectionPool.h"


CommonConnectionPool& CommonConnectionPool::getConnectionPool()
{
	static CommonConnectionPool connPool;
	return connPool;
}

shared_ptr<Connection> CommonConnectionPool::getConnection()
{
	return shared_ptr<Connection>();
}

CommonConnectionPool::CommonConnectionPool()
{
	if (!loadConfigFile())
	{
		return;
	}

	// 创建初始链接
	for (int i = 0; i < _initSize; i++)
	{
		Connection* conn = new Connection;
		conn->connect(_ip, _port, _username, _password, _dbname);
		_connectionQuque.push(conn);
		_connecionCnt++;
	}

	// 启动新的线程作为连接生产者

	thread produce(std::bind(&CommonConnectionPool::produceConnectionTask, this));

}

bool CommonConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not existed!");
		return false;
	}

	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string sline = line;
		int idx = sline.find('=', 0);
		if (-1 == idx)
		{
			continue;
		}

		int endIdx = sline.find('\n');
		string key = sline.substr(0, idx);
		string value = sline.substr(idx + 1, endIdx - idx - 1);

		if (key == "ip")
		{
			_ip = value;
		}
		else if (key == "port")
		{
			_port = atoi(value.c_str());
		}else if(key =="username")
		{
			_username = value;
		}
		else if (key == "password")
		{
			_password = value;
		}
		else if (key == "InitSize")
		{
			_initSize = atoi(value.c_str());
		}
		else if (key == "MaxSize")
		{
			_maxSize = atoi(value.c_str());
		}
		else if (key == "MaxIdleTime")
		{
			_maxIdleTime = atoi(value.c_str());
		}
		else if(key == "MaxConnectionTime")
		{
			_connectionTimeOut = atoi(value.c_str());
		}
		else if (key == "dbname")
		{
			_dbname = value;
		}

	}
	return true;
}

void CommonConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lck(_queueMutex);
		while (!_connectionQuque.empty())
		{
			cv.wait(lck);
		}

		// 创建新的连接
		if (_connecionCnt < _maxSize)
		{
			Connection* conn = new Connection;
			conn->connect(_ip, _port, _username, _password, _dbname);
			_connectionQuque.push(conn);
			_connecionCnt++;
		}
		cv.notify_all(); // 通知消费线程进行消费
	}
}

