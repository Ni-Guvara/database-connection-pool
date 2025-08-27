#pragma once

#include <mysql/mysql.h>
#include <string>
#include <ctime>
using namespace std;

#include "public.h"

class Connection
{
public:
	Connection();

	~Connection();

	bool connect(string ip, unsigned short port, string user, string password,
				 string dbname);

	bool update(string sql);

	MYSQL_RES *query(string sql);

	void refreshtime();
	clock_t getalivetime();

private:
	MYSQL *_conn;
	clock_t _alivetime;
};
