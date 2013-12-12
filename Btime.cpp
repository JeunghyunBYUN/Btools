#include <sys/time.h>
#include "Btools.h"

using namespace Btools;

void Btools::GetDateTime(char * szDate, char * szTime, const char * szDateDelim, const char * szTimeDelim)
{
  time_t      tick;
  struct tm   tms;
  tick = time(NULL);
  localtime_r(&tick, &tms);

  if (szDate) sprintf(szDate, "%04d%s%02d%s%02d", tms.tm_year+1900, szDateDelim, tms.tm_mon+1, szDateDelim, tms.tm_mday);
  if (szTime) sprintf(szTime, "%02d%s%02d%s%02d", tms.tm_hour, szTimeDelim, tms.tm_min, szTimeDelim, tms.tm_sec);
}

string Btools::GetToday(constsz szDateDelim)
{
  char szBuffer[BTOOLS_BUFFER_SIZE];
  GetDateTime( szBuffer, NULL, szDateDelim);
  return szBuffer;
}

string Btools::GetNow()
{
  char szBuffer[BTOOLS_BUFFER_SIZE];
  GetDateTime( szBuffer, szBuffer +11, "/", ":");
  return szBuffer;
}


void BstopWatch::Start()
{
  if(!m_bRunning)
  {
    gettimeofday(&m_tvStart, NULL);
    m_bRunning = true;
  }
}

void BstopWatch::Stop()
{
  if(m_bRunning)
  {
    m_accTime += Microsecond();
    m_bRunning = false;
  }
}

void BstopWatch::Reset()
{
  m_accTime = 0;
  m_bRunning = false;
}

float BstopWatch::Millisecond() const
{
  return (m_accTime + Microsecond()) / 1000.0;
}

float BstopWatch::Second() const
{
  return (m_accTime + Microsecond()) / 1000000.0;
}

string BstopWatch::GetTime() const
{
  size_t time = Millisecond();
  return GetStringf("%02d:%02d:%06.3f", time/3600000, (time/60000)%60, (time%60000) / 1000.0 );
}

ulong BstopWatch::Microsecond() const
{
  if(!m_bRunning) return 0;
  struct timeval tvEnd;
  gettimeofday(&tvEnd, NULL);
  return ( tvEnd.tv_sec - m_tvStart.tv_sec )*1000000 + tvEnd.tv_usec - m_tvStart.tv_usec;
}




