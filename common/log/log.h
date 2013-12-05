#ifndef LOG_H_
#define LOG_H_


static const char *KEY = "key: \n";
static const char *ERR = "error: \n";
static const char *WAR = "warning: \n";
static const char *INFO = "info: \n";

static const int MAX_STR_LEN = 512;

enum loglevel
{
  LOGLEVELALL = 0,
  LOGLEVELINFO,
  LOGLEVELWARNING,
  LOGLEVELERROR,
  LOGLEVELSTOP
};


class logger 
{
public:
  logger(const char* logpath, loglevel level = LOGLEVELALL);
  ~logger();

  int init();
  void loginfo(const char *strinfo, ...);
  void logwarning(const char *strinfo, ...);
  void logerror(const char *strinfo, ...);
  void set_loglevel(loglevel nlevel);
private:

  void trace(const char *strinfo);
  //char *getcurrenttime();
  void createlogname();
  int createlogfile();
  //void createlogpath();

private:
  int file_fd_;
  loglevel loglevel_;
  char log_path_name_[MAX_STR_LEN];
  char log_cur_name_[MAX_STR_LEN];
};

#endif
