#pragma once

#include <string>
#include <queue>
#include <mutex>
#include "Connection.h"

class CommonConnectionPool
{
public:
	static CommonConnectionPool& getConnectionPool();  // ����ģʽ-����ģʽ
	bool loadConfigFile();

private:
	CommonConnectionPool();

	string _ip;  // mysql ip��ַ
	unsigned short _port; // mysql �˿ں�
	string _username; // �û���
	string _password; // ����
	string _dbname;

	int _initSize;  // ���ӳ�����������ʼ��
	int _maxSize;   // �����������
	int _maxIdleTime; // ������ʱ��
	int _connectionTimeOut;  // �������ʱ��

	atomic_int _connecionCnt; // ��¼��������������
	queue<Connection*> _connectionQuque;
	mutex _queueMutex;

};