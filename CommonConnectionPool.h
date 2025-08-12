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
	static CommonConnectionPool* getConnectionPool();  // ����ģʽ-����ģʽ
	shared_ptr<Connection> getConnection();

private:
	CommonConnectionPool();

	bool loadConfigFile();

	void produceConnectionTask();
	void scanIdleConnection();

	string _ip;  // mysql ip��ַ
	unsigned short _port; // mysql �˿ں�
	string _username; // �û���
	string _password; // ����
	string _dbname;

	int _initSize;  // ���ӳ�����������ʼ��
	int _maxSize;   // �����������
	int _maxIdleTime; // ������ʱ��
	int _connectionTimeOut;  // �������ʱ��

	atomic_int _connectionCnt; // ��¼��������������
	queue<Connection*> _connectionQuque;
	mutex _queueMutex;
	condition_variable cv; // �����߳�-�����߳�ͨ�� ������������
};
