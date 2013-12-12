//#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Btools.h"

string Btools::Int2String(int num)
{
	char szBuffer[ BTOOLS_BUFFER_SIZE ];
	snprintf(szBuffer, BTOOLS_BUFFER_SIZE, "%d", num);
	return szBuffer;
}

string Btools::GetStringf(constsz szFormat, ... )
{
	char szBuffer[ BTOOLS_BUFFER_SIZE ];
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsnprintf(szBuffer, BTOOLS_BUFFER_SIZE, szFormat, vaArgs);
	va_end(vaArgs);
	return szBuffer;
}

string & Btools::Chomp(string & strLine)
{
	if( !strLine.empty() && *strLine.rbegin() == '\n' )
		strLine.resize(strLine.size()-1);
	return strLine;
}

string & Btools::TrimRight(string & s)
{
	static const char* szSpaces = " \t\r\n";
	size_t nEnd = s.find_last_not_of(szSpaces);
	if (nEnd == string::npos)
	   	s.clear();
	else
		s.resize(nEnd+1);

	return s;
}

string & Btools::Trim(string & s)
{
	static const char* szSpaces = " \t\r\n";

	size_t nBegin = s.find_first_not_of(szSpaces);
	if (nBegin == string::npos)
		s.clear();
	else
	{
		size_t nEnd = s.find_last_not_of(szSpaces);
		s = s.substr(nBegin, nEnd -nBegin +1);
	}
	return s;
}

void Btools::Tokenize(const string& strInput, constsz szDelim, vector<string>& vToken, bool bClear)
{
	if(bClear) vToken.clear();

	string::size_type nLastPos = strInput.find_first_not_of(szDelim, 0);
	string::size_type nPos = strInput.find_first_of(szDelim, nLastPos);

	while ((string::npos != nPos) || (string::npos != nLastPos))
	{
		vToken.push_back(strInput.substr(nLastPos, nPos-nLastPos));
		nLastPos = strInput.find_first_not_of(szDelim, nPos);
		nPos = strInput.find_first_of(szDelim, nLastPos);
	}
}

void Btools::Split( const string & str, constsz szDelim, vector<string> & vPiece, bool bClear )
{
	if(bClear) vPiece.clear();

	string strFirst;
	constsz pBuffer = str.c_str();
	while(GetFirst(strFirst, pBuffer, szDelim))
		vPiece.push_back(strFirst);
}

string & Btools::Join(vector<string> & vPiece, constsz szDelim, string & strOutput )
{
	strOutput.clear();
	if(vPiece.empty()) return strOutput;

	size_t nSize = 0;
	for( vector<string>::iterator it = vPiece.begin();  it != vPiece.end(); ++it)
		nSize += it->size();

	strOutput.reserve(nSize + strlen(szDelim) * vPiece.size() );

	vector<string>::iterator it = vPiece.begin();
	strOutput = *(it++);
	while( it != vPiece.end())
		strOutput += szDelim + *(it++);
	return strOutput;
}

size_t Btools::Replace(string & strTarget, string strFrom, string strTo)
{
	if(strTarget.empty() || strFrom.empty()) return 0;
	size_t nCount =0;
	for(size_t nPos = 0; (nPos = strTarget.find(strFrom, nPos)) != string::npos; nPos+= strTo.size(), ++nCount)
		strTarget = strTarget.substr(0, nPos) + strTo + strTarget.substr(nPos + strFrom.size());
	return nCount;
}

// Simple Version
string Btools::GetFirst( string & strBuffer, constsz szDelim)
{
	string strReturn;
	size_t nPos = strBuffer.find(szDelim);
	if (nPos == string::npos)
		nPos = strBuffer.size();
	strReturn.assign(strBuffer, 0, nPos);
	strBuffer.erase( 0, nPos+ strlen(szDelim));
	return strReturn;
}

// Costant Version
bool Btools::GetFirst( string & strPiece, constsz & pBuffer, constsz szDelim)
{
	if( !pBuffer || !*pBuffer )
	{
		strPiece.clear();
		return false;
	}
	static constsz pDelim;
	pDelim = strstr(pBuffer, szDelim);
	if(pDelim)
	{
//Bdbgf("%p %p\n", pBuffer, pDelim);		
		strPiece.assign( pBuffer, pDelim - pBuffer);
		pBuffer = pDelim + strlen(szDelim);
	}
	else
	{
		strPiece.assign(pBuffer);
		pBuffer += strlen(pBuffer);
	}
	return true;
}

