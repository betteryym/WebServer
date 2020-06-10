#include "http_conn.h"

#include <mysql/mysql.h>
#include <fstream>

//定义http响应的一些状态消息
const char* ok_200_titile = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntx or is inherently impossible to staisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The requested file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the request file.\n";

locker m_lock;
map<string, string> users;

void http_conn::initmysql_result(connection_pool* connPool){
    //先从连接池中取一个连接
    MYSQL* mysql = NULL;
    connectionRAII mysqlcon(&mysql, connPool);

    //在user表中检索username, passwd数据，浏览器端输入
    if(mysql_query(mysql, "SELECT username,passwd FROM user")){
        LOG_ERROR("SELECT error:%s\n", mysql_error(mysql));
    }

    //从表中检索完整的结果集
    MYSQL_RES* result = mysql_store_result(mysql);

    //返回结果集中的列数
    int 
}
