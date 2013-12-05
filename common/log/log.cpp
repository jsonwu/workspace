#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include "log.h"

using namespace std;
logger::logger(const char *logpath, loglevel level):
  file_fd_(-1)
{
  this->loglevel_ = level;
  strncpy(this->log_path_name_, logpath, MAX_STR_LEN-1);
}

logger::~logger()
{
  if (this->file_fd_ != -1)
    close(this->file_fd_);
}

int logger::init()
{
  if (access(this->log_path_name_, 0) != 0)
  {
    if (mkdir(this->log_path_name_, 0777) == -1)
      return -1;
  }
  int len = strlen(this->log_path_name_);
  if (this->log_path_name_[len-1] != '/')
    strcat(this->log_path_name_, "/");

  this->createlogname();

  return this->createlogfile();
}

void logger::loginfo(const char *strinfo, ...)
{
  if (this->loglevel_ > LOGLEVELINFO)
    return ;
  char tmp[1024] = {0};
  sprintf(tmp,"[%d][%s]:", time(NULL), "info");
  va_list argp;
  va_start(argp, strinfo);
  vsprintf(tmp + strlen(tmp), strinfo, argp);
  va_end(argp);
  this->trace(tmp);
}

void logger::logwarning(const char *strinfo, ...)
{
  if (this->loglevel_  >  LOGLEVELWARNING)
    return ;

  char tmp[1024] = {0};
  sprintf(tmp,"[%d][%s]:", time(NULL), "warning");
  va_list argp;
  va_start(argp, strinfo);
  vsprintf(tmp + strlen(tmp), strinfo, argp);
  va_end(argp);
  this->trace(tmp);
}

void logger::logerror(const char *strinfo, ...)
{
  if (this->loglevel_ > LOGLEVELERROR)
    return ;
  char tmp[1024] = {0};
  sprintf(tmp,"[%d][%s]:", time(NULL), "error");
  va_list argp;
  va_start(argp, strinfo);
  vsprintf(tmp + strlen(tmp), strinfo, argp);
  va_end(argp);
  this->trace(tmp);
}

void logger::trace(const char *info)
{
  try{
    write(this->file_fd_, info, strlen(info));
    write(this->file_fd_, "\n", 1);
  }catch(exception& e)
  {
    std::cout << "wite log error" << endl;
  }
}

int logger::createlogfile()
{
  char tmp[1024] = {0};
  strcat(tmp, this->log_path_name_);
  strcat(tmp, this->log_cur_name_);
  this->file_fd_ = open(tmp, O_RDWR | O_APPEND | O_CREAT, 0644);
  if (this->file_fd_ == -1)
  {
    cout << strerror(errno) << endl;
    return -1;
  }
  return 0;
}

void logger::createlogname()
{
  time_t t = time(NULL);
  struct tm *tm_ = localtime(&t);
  strftime(this->log_cur_name_, MAX_STR_LEN, "%F", tm_);
  strcat(this->log_cur_name_, ".log");
}
// ---------------------test------------------------------------------
int main()
{
  logger log("./", LOGLEVELWARNING);
  if (log.init() != 0)
    cout << "init log error" << endl;
  log.loginfo("ca %s %d", "sss", 10);
  log.logwarning("ca %s %d", "sss", 10);
  log.logerror("ca %s %d", "sss", 10);
}
