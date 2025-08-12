#pragma once

#include<mysql.h>
#include<string>
#include<ctime>
using namespace std;

#include "public.h"

class Connection
{
public:
	// ��ʼ�����ݿ�����
	Connection();
	// �ͷ����ݿ�������Դ
	~Connection();
	// �������ݿ�
	bool connect(string ip, unsigned short port, string user, string password,
		string dbname);
	// ���²��� insert��delete��update
	bool update(string sql);
	// ��ѯ���� select
	MYSQL_RES* query(string sql);

	void refreshtime();
	clock_t getalivetime();
private:
	MYSQL* _conn; // ��ʾ��Connection Server��һ������
	clock_t _alivetime;
};
