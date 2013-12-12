/**
 *	Title  : Btools
 *	Author : ddolgi@gmail.com
 *	To do  :
 */

#ifndef __BTOOLS_H__
#define __BTOOLS_H__


#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <algorithm>

#include <vector>
#include <map>

using namespace std;

typedef unsigned long       ulong;
typedef unsigned int        uint;
typedef const char *		constsz;

#define BTOOLS_BUFFER_SIZE 10240

#define Berror                      "ERROR "
#define Bwarn                       "WARN "
#define Binfo                       "INFO "
#define Bdbg                        cerr<<"DEBUG "<<__FILE__<<":"<< __LINE__<<": "
#define Bdbgf( _fmt, _vaarg...)     fprintf( stderr, "DEBUG %s:%d: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#define TimeLog   cerr <<Btools::GetNow() <<" "
#define TimeLogf( _fmt, _vaarg...)    fprintf( stderr, "%s " _fmt, Btools::GetNow().c_str(), ##_vaarg)

#define Bdump(strContainer) cerr<<"{{{"<<endl; \
for_each(strContainer.begin(), strContainer.end(), Btools::Print2stderr); \
cerr<<"}}}"<<endl;

#define Return(strMsg, rc) return (Btools::Print2stderr(strMsg) + rc)
#define ReturnNull(strMsg) return (char*)Btools::Print2stderr(strMsg)

namespace Btools
{
/**
 * Bfile.cpp
 */
	int     Print2stderr(string str);

	long    FileSize( string fileName );
	bool    SaveFile( string fileName, const void * pData, size_t size);
	inline bool    SaveText( string fileName, string & strBuffer)
	{ return SaveFile(fileName, (const void *)strBuffer.c_str(), strBuffer.size()); }
	bool    LoadText( string fileName, string & strBuffer);
	size_t	LoadConfig( map<string, string> & mConfig, string & strBuffer, constsz szDelim = "=", constsz szComment = "#");
	bool    GetLine(string & output, FILE * f);	// Chomp '\n'

	class	BstringTable
	{
		public:
			BstringTable() : m_strPool("") {}

			// ex> "abc/def" for "abc/def.index" and "abc/def.string"
			void	Clear() { m_vOffset.clear(); m_strPool.clear(); }
			bool	Load(const string& strTablePath); 
			void	Append(string strItem);

			template<class InputIterator >
			void	Append( InputIterator beginIter, InputIterator endIter)
			{ while (beginIter != endIter) Append(*(beginIter++)); }

			bool	Save(const string& strTablePath);
			bool	Build(const vector<string>& vStringList, const string& strPath)
			{ Clear(); Append( vStringList.begin(), vStringList.end() ); return Save(strPath); }

			void	Print(FILE * f = stderr);

			size_t	Size()               const { return m_vOffset.size(); }
			constsz	operator[](size_t i) const { return At(i); }
			constsz	At(size_t i)     const { return (i<m_vOffset.size()) ? &m_strPool[0] +m_vOffset[i] : NULL; }

		protected:
			vector<size_t>  m_vOffset;
			string          m_strPool;
	};


	/**
	 * Bstring.cpp
	 */
	string    Int2String(int num);
	string    GetStringf(constsz szFormat, ... );

	string &  Chomp(string & s);        // cut the last '\n' off
	string &  TrimRight(string & s);    // cut right white spaces(" \t\r\n") off
	string &  Trim(string & s);    		// cut left and right white spaces(" \t\r\n") off

	/**
	 * Tokenize
	 * @param strInput: input string
	 * @param szDelim: a STRING Delimiter
	 * @param vTokens: output string vector
	 * @param bClear: If true, clear vTokens at start 
	 * @remark Empty token: DISCARD!!!
	 * @return nothing
	 */
	void Tokenize( const string& strInput, constsz szDelim, vector<string>& vToken, bool bClear =true );

	/**
	 * Split 
	 * @param strInput: input string
	 * @param szDelim: a STRING Delimiter
	 * @param vPiece: output string vector
	 * @param bClear: If true, clear vTokens at start 
	 * @remark Empty token: KEEP!!!
	 * @return nothing
	 */
	void Split( const string & strInput, constsz szDelim, vector<string> & vPiece, bool bClear =true );

	string & Join( vector<string> & vPiece, constsz szDelim, string & strOutput );
	size_t Replace( string & strTarget, string strFrom, string strTo );


	/**
	 * GetFirst - Simple Ver.
	 * @param inStr_outRest: input string and output rest string pointer
	 * @param szDelim: a STRING delimiter
	 * @return a first piece string (can be empty)
	 * @remarks simple usage but VERY SLOW
	 */
	string  GetFirst(string & inStr_outRest, constsz szDelim);

	/**
	 * GetFirst - Constant Ver.
	 * @param outStrPiece: a first piece string (can be empty)
	 * @param inStr_outRest: input string pointer and output rest string pointer
	 * @param szDelim: a STRING delimiter
	 * @return true on success and false at the end
	 * @remarks input string NEVER changed
	 */
	bool    GetFirst( string & outFirst, constsz & inStr_outRest, constsz szDelim);

	inline	bool    GetLine( string & strLine, constsz & szBuffer ) { return GetFirst( strLine, szBuffer, "\n"); }
	inline	char *	GetLine( char *& szBuffer ) { return strsep( &szBuffer, "\n"); }

	/**
	 * Btime.cpp
	 */
	void    GetNow(char * szDate, char * szTime, constsz szDateDelim ="/");
	string  GetNow(constsz szDateDelim ="/");

	class BstopWatch
	{
		public:
			BstopWatch()  { Reset(); Start(); }
			void    Start();
			void    Stop();
			void    Reset();
			void    Restart() { Reset(); Start(); }
			string  GetTime() const;
			void    Print(FILE* f = stderr) const
			{
				fprintf(f, "  ==> Elapsed time : %s\n\n", GetTime().c_str() );
				fflush(f);
			}

			float   Second() const;
			float   Millisecond() const;
			ulong   Microsecond() const;

		private:
			bool    m_bRunning;
			timeval m_tvStart;
			ulong   m_accTime;
	};


	/**
	 * Bcounter.cpp
	 */
	class Bcounter
	{
		public:
			/**
			 * nTermBit
			 * 0: print everytime, 
			 * 1: print every other time(2^1),
			 * 2: print every 4(2^2) time.
			 */
			Bcounter( uint nTermBit = 10 ):m_Total(0) { m_TermBit= (1<<nTermBit) -1; }

			bool	IsToPrint() {  return !(m_Total & m_TermBit); }
			ulong	GetTotal() { return m_Total; }
			ulong	GetCat(string strCat) { return m_mCount[strCat]; }

			ulong	Count( string strCategory = "" )
			{ ++m_Total; return ++m_mCount[strCategory]; }

			void	PrintProc( FILE * fout = stderr ) { if(IsToPrint()) fprintf(fout, "Processing %lu\n", m_Total); }
			void	PrintProcK( FILE * fout = stderr ) { if(!(m_Total & 0x03ff)) fprintf(fout, "Processing %luk\n", m_Total>>10); }
			void	PrintCategory( FILE * fout = stderr, bool bPercent = true);
			void	PrintAll( FILE * fout = stderr, bool bPercent = true);
			string	DumpCategory( string strCategory, bool bName, bool bTotal, bool bPercent)
			{ return DumpCategory( m_mCount.find(strCategory), bName, bTotal, bPercent); }

		private:
			string 	DumpCategory( map<string, ulong>::iterator it, bool bName, bool bTotal, bool bPercent);

			int              	m_TermBit;
			map<string, ulong>	m_mCount;
			ulong               m_Total;
	};
}

#endif

/**
 * vim: ts=4: sw=4
 */

