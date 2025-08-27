#include "CommonConnectionPool.h"

CommonConnectionPool *CommonConnectionPool::getConnectionPool()
{
	static CommonConnectionPool pool;
	return &pool;
}

shared_ptr<Connection> CommonConnectionPool::getConnection()
{
	unique_lock<mutex> lck(_queueMutex);

	while (_connectionQuque.empty())
	{
		if (cv_status::timeout == cv.wait_for(lck, chrono::milliseconds(_connectionTimeOut)))
		{
			if (_connectionQuque.empty())
			{
				LOG("获取连接时间超时，连接失败！！！");
				return nullptr;
			}
		}
	}

	// shared_ptr会自动delete connection, connection会自动执行析构函数，直接释放资源
	// 但是这种方式与程序直接将用完的连接归还队列的目的不一致
	// 因此使用自定义删除器，完成归还操作, 这里对队列进行操作要注意线程安全
	shared_ptr<Connection> front(_connectionQuque.front(), [&](Connection *sp)
								 {
		unique_lock<mutex> lck(_queueMutex);
		sp->refreshtime();
		_connectionQuque.push(sp); });

	_connectionQuque.pop();
	cv.notify_all();
	return front;
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
		Connection *conn = new Connection;
		if (conn->connect(_ip, _port, _username, _password, _dbname))
		{
			conn->refreshtime();
			_connectionQuque.push(conn);
			_connectionCnt++;
		}
	}

	// 启动新的线程作为连接生产者
	thread produce(std::bind(&CommonConnectionPool::produceConnectionTask, this));
	produce.detach();

	// 启动扫描空闲连接线程
	thread scanner(std::bind(&CommonConnectionPool::scanIdleConnection, this));
	scanner.detach();
}

bool CommonConnectionPool::loadConfigFile()
{
	FILE *pf = fopen("../config/mysql.ini", "r");
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not existed!");
		return false;
	}

	while (!feof(pf))
	{
		char line[1024] = {0};
		fgets(line, 1024, pf);
		string sline = line;
		size_t idx = sline.find('=', 0);
		if (-1 == idx)
		{
			continue;
		}

		size_t endIdx = sline.find('\n');
		string key = sline.substr(0, idx);
		string value = sline.substr(idx + 1, endIdx - idx - 1);

		if (key == "ip")
		{
			_ip = value;
		}
		else if (key == "port")
		{
			_port = atoi(value.c_str());
		}
		else if (key == "username")
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
		else if (key == "MaxConnectionTime")
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
		if (_connectionCnt < _maxSize)
		{
			Connection *conn = new Connection;
			conn->connect(_ip, _port, _username, _password, _dbname);
			conn->refreshtime();
			_connectionQuque.push(conn);
			_connectionCnt++;
		}
		cv.notify_all(); // 通知消费线程进行消费
	}
}

void CommonConnectionPool::scanIdleConnection()
{
	for (;;)
	{
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));

		unique_lock<mutex> lck(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection *sp = _connectionQuque.front();
			if (sp->getalivetime() >= _maxIdleTime * 1000)
			{
				_connectionQuque.pop();
				_connectionCnt--;
				delete sp;
			}
			else
			{
				break;
			}
		}
	}
}
