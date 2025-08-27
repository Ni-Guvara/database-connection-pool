#include "Connection.h"

Connection::Connection()
{
	_conn = mysql_init(nullptr);
}

Connection::~Connection()
{
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port, string user, string password,
						 string dbname)
{
	MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(),
								  password.c_str(), dbname.c_str(), port, nullptr, 0);

	if (p == nullptr)
	{
		// 关键：打印具体错误
		std::cout << "连接失败: " << mysql_error(_conn) << std::endl;
	}

	return p != nullptr;
}

bool Connection::update(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("UPDATE FAIL:" + sql);
		return false;
	}
	return true;
}

MYSQL_RES *Connection::query(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("UPDATE FAIL:" + sql);
		return nullptr;
	}
	return mysql_use_result(_conn);
}

void Connection::refreshtime()
{
	_alivetime = clock();
}

clock_t Connection::getalivetime()
{
	clock_t alivetime = clock() - _alivetime;
	return alivetime;
}
