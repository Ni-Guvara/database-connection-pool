#pragma once

#include<mysql.h>
#include<string>
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
private:
	MYSQL* _conn; // ��ʾ��Connection Server��һ������
};
