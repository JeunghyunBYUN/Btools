/////////////// LOG
enum LogCode {LOG_INIT, LOG_RUN, LOG_FINISH, LOG_ERROR};
class CLog
{
  public:
    CLog (): m_Context(1), m_Socket(m_Context, ZMQ_PUSH){}
    ~CLog () { m_Socket.close(); }

    bool  Init( string managerAddr, string projectName, string versionName, string processName, string ports );
    bool  Write( LogCode eLog, string strMsg );
    void  Finish()
    { Write(LOG_FINISH, "Finished - "+m_StopWatch.GetTime()); }

    void  Interupt()
    { Write(LOG_ERROR, "[ERROR] Process Interrupted."); }

  private:
    string  ReadStatusFile();

    zmq::context_t  m_Context;
    zmq::socket_t   m_Socket;

    string  m_ProjectName;
    string  m_VersionName;
    string  m_HostName;
    pid_t   m_ProcessID;
    string  m_ProcessName;
    string  m_Ports;
    string  m_StatFilePath;

    Blib::TStopWatch m_StopWatch;
};

bool CLog::Init( string strManagerAddr, string strProjectName, string strVersionName, string strProcessName, string strPorts )
{
  if (!ConnectAndWait( m_Socket, strManagerAddr, "management_server", ZMQ_POLLOUT))
    return false;

  m_ProjectName = strProjectName;
  m_VersionName = strVersionName;
  m_ProcessName = strProcessName;
  m_Ports       = strPorts;

  char szHostName[MAX_HOSTNAME_LENGTH];
  if ( gethostname(szHostName, MAX_HOSTNAME_LENGTH))
    Return("Failed to get host name.", false);
  m_HostName = szHostName;
  m_ProcessID = getpid();
  m_StatFilePath = "/proc/self/status";

  return Write(LOG_INIT, "Log Initiated.");
}

bool CLog::Write( LogCode eLog, string strMsg )
{
  BSONObjBuilder bobLog;
  bobLog.append("project_name"  , m_ProjectName);
  bobLog.append("version_name"  , m_VersionName);
  bobLog.append("hostname"      , m_HostName);
  bobLog.append("pid"           , (int)m_ProcessID);
  bobLog.append("process_name"  , m_ProcessName);
  bobLog.append("ports"         , m_Ports);
  bobLog.append("time"          , (int)time(NULL));
  bobLog.append("time_str"      , Blib::GetEzNow());
  bobLog.append("stat_file"     , ReadStatusFile());
  bobLog.append("status_code"   , (int)eLog);
  bobLog.append("status_msg"    , strMsg);

  TimeLog<<strMsg<<endl;
  return SendZmqMsg( m_Socket, bobLog.done().jsonString());
}

string CLog::ReadStatusFile()
{
  FILE * pFile = fopen(m_StatFilePath.c_str(), "r");
  if (!pFile) return "";

  string strReturn, strLine;
  while( GetLine(strLine, pFile) )
    strReturn += strLine;

  fclose(pFile);
  return strReturn;
}
