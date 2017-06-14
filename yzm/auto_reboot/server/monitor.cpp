#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <errno.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <sstream>
#include <libgen.h>
#include <fstream>

#include "configManager.h"
using namespace std;

conf::CConfigManager* gPtrConf;


bool callSystemCmd(string cmd){
	pid_t status;

	status = system(cmd.c_str());

	if (-1 == status)
	{
		return false;
	}
	else
	{

		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;

}

bool is_process_exit(string process_name, int& pid_num){
	FILE *pstr; char cmd[1280], buff[1280], *p;
	pid_t pID;

	memset(cmd, 0, sizeof(cmd));

	sprintf(cmd, "ps -ef|grep %s |grep -v \"grep\"", process_name.c_str());
	pstr = popen(cmd, "r");
	if (pstr == NULL){
		return false;
	}
	memset(buff, 0, sizeof(buff));
	fgets(buff, 512, pstr);
	p = strtok(buff, " ");
	p = strtok(NULL, " ");
	pclose(pstr);
	if (p == NULL || strlen(p) == 0){
		return false;
	}

	if ((pid_num = atoi(p)) == 0){
		return false;
	}

	pID = (pid_t)pid_num;
	int ret = kill(pID, 0); //这里不是要杀死进程，而是验证一下进程是否真的存在，返回0表示真的存在     
	if (ret != 0){
		return false;
	}

	return true;
}

void exec_process(string url){
	int ret, i, status;
	char *child_argv[100] = { 0 };
	pid_t pid;

	do
	{
		pid = fork();
		if (pid == -1) {
			LOG(ERROR) << "fork() error, errno:" << errno << "error_mess : " << strerror(errno);
			break;
		}
		if (pid == 0) {

			int pos = url.find_last_of('/');
			string dir = url.substr(0, pos);
			stringstream ss;
			ss << "cd  ";
			ss << dir;
			string cmd;
			ss >> cmd;

			LOG(INFO) << "chdir()  begin ";

			if (chdir(dir.c_str()) < 0)
			{
				LOG(ERROR) << "chdir() error, dir : " << dir;
				return;
			}

			int ret = execv(url.c_str(), (char **)child_argv);
			if (ret < 0) {
				LOG(ERROR) << "execv() error, errno:" << errno << "error_mess : " << strerror(errno);
				exit(-1);
			}
			exit(-1);
		}

		if (pid > 0) {
			LOG(INFO) << "i am father process";
		}
	} while (0);


}

void exec_process2(string url){
	int ret, i, status;
	char *child_argv[100] = { 0 };
	pid_t pid;

	do
	{
		std::ifstream ifs;
		ifs.open(url, ios::binary);
		if (!ifs.is_open())
		{
			LOG(ERROR) << url << " not exist !";
			break;
		}
		pid = fork();
		if (pid == -1) {
			LOG(ERROR) << "fork() error, errno:" << errno << "error_mess : " << strerror(errno);
			break;
		}
		if (pid == 0) {
			if ((pid = fork()) < 0)//由子进程1创建子进程2
			{
				//LOG(ERROR) << "fork() error, errno:" << errno << "error_mess : " << strerror(errno);
			}
			else if (pid > 0)
			{
				exit(0);       //子进程1结束
			}
			else//子进程2
			{
				int pos = url.find_last_of('/');
				string dir = url.substr(0, pos);

				//为了写日志，记录原来工作目录
				char pre_work_dir[512] = { 0 };
				if (getcwd(pre_work_dir, 512) == NULL)
				{
					exit(-1);  
				}

				if (chdir(dir.c_str()) < 0)
				{
					LOG(ERROR) << "chdir() error, dir : " << dir;
					return;       
				}

				int ret = execv(url.c_str(), (char **)child_argv);
				if (ret < 0) {
					chdir(pre_work_dir);
					//LOG(INFO) << "previous work dir : " << pre_work_dir;
					//LOG(ERROR) << "execv() error, errno:" << errno << "error_mess : " << strerror(errno);
					exit(-1);
				}
			}
		}

		if (pid > 0) {
			waitpid(0, NULL, 0);
		}
	} while (0);

}

int main()
{
	daemon(1, 0);
	gPtrConf = new conf::CConfigManager();
	gPtrConf->get_glog_paramer("../conf/app.conf");
	conf::init_glog("monitor");

	LOG(INFO) << "server begin to work";
	if (!gPtrConf->get_monitor_program("../conf/app.conf"))
	{
		LOG(ERROR) << "get_monitor_program  failed";
		return -1;
	}
	LOG(INFO) << "server  begin...";
	while (1)
	{
		for (int i = 0; i < gPtrConf->_monitor_program_vec.size(); i++)
		{
			int pos = gPtrConf->_monitor_program_vec[i].find_last_of('/');
			string program_name = gPtrConf->_monitor_program_vec[i].substr(pos + 1);
			int pid_num;
			if (!is_process_exit(program_name, pid_num))
			{
				LOG(WARNING) << program_name << " not exist ";
				exec_process2(gPtrConf->_monitor_program_vec[i]);
			}
		}

		sleep(3);
	}

	return 0;

}


