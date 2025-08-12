#pragma once

#include<mysql.h>
#include<string>
#include<ctime>
using namespace std;

#include "public.h"

class Connection
{
public:
	// 初始化数据库连接
	Connection();
	// 释放数据库连接资源
	~Connection();
	// 连接数据库
	bool connect(string ip, unsigned short port, string user, string password,
		string dbname);
	// 更新操作 insert、delete、update
	bool update(string sql);
	// 查询操作 select
	MYSQL_RES* query(string sql);

	void refreshtime();
	clock_t getalivetime();
private:
	MYSQL* _conn; // 表示和Connection Server的一条连接
	clock_t _alivetime;
};
