#include <string.h>
#include <sys/stat.h>
#include "Btools.h"
using namespace Btools;

int Btools::Print2stderr(string str) { cerr <<str <<endl; return 0; }

bool Btools::GetLine(string & strLine, FILE * f)
{
	strLine.clear();
	if( !f ) return false;

	char szBuffer[BTOOLS_BUFFER_SIZE];
	while (fgets(szBuffer, BTOOLS_BUFFER_SIZE, f))
	{
		strLine += szBuffer;
		if (*strLine.rbegin() == '\n') break;
	}
	if (strLine.empty()) return false;
	Chomp(strLine);
	return true;
}

size_t Btools::LoadConfig( StrMap & mFieldValue, string & strBuffer, constsz szDelim, constsz szComment)
{
	mFieldValue.clear();

	string strLine;
	constsz pBuffer = strBuffer.c_str();
	while( GetLine( strLine, pBuffer ) )
	{
		string strAssign = GetFirst(strLine, szComment);
		if( Trim(strAssign).empty() ) continue;
		string strField = GetFirst(strAssign, szDelim);
		string strValue = GetFirst(strAssign, szDelim);
		Trim(strField);
		Trim(strValue);

		int nStart = (*strValue.begin() == '\"')? 1 : 0;
		int nEnd   = (*strValue.rbegin()== '\"')? 1 : 0;
		if( nStart || nEnd )
			strValue = strValue.substr(nStart, strValue.size() -1 -nEnd);
		Trim(strValue);
		mFieldValue[strField] = strValue;
	}

	return mFieldValue.size();
}

long Btools::FileSize( string fileName )
{
	struct stat pStat;
	if(stat( fileName.c_str(), &pStat) < 0)
		Return ( "Cannot access: " + fileName, -1);
	return pStat.st_size;
}

bool Btools::LoadText ( string fileName, string & strBuffer)
{
	long nFileSize = FileSize(fileName.c_str());
	
	if( nFileSize <0 ) return false;
	strBuffer.resize(nFileSize);

	FILE * pFile = fopen(fileName.c_str(), "r");
	if (!pFile) Return("Failed to Open " + fileName, false);
	long nRead = fread( (void *)strBuffer.c_str(), 1, nFileSize, pFile);
	fclose(pFile);
	return nRead == nFileSize;
}

bool Btools::SaveFile( string fileName, const void * pData, size_t size)
{
	FILE * pFile = fopen(fileName.c_str(), "w");
	if (!pFile) Return("Failed to Open " + fileName, false);
	size_t nWrite = fwrite( pData, 1, size, pFile);
	fclose(pFile);

	if (nWrite == size)  return true;
	Return("Writing Failed.", false);
}


#define INDEX_FILE   ".index"
#define STRING_FILE  ".string"

bool BstringTable::Load(const string& strTablePath)
{
	string strIndexFile = strTablePath + INDEX_FILE;
	long nFileSize = FileSize(strIndexFile.c_str());
	if( nFileSize < -1 ) return -1;
	size_t nSize = nFileSize / sizeof(char *);
	m_vOffset.resize( nSize );

	FILE * pFile = fopen( strIndexFile.c_str(), "r");
	if (!pFile) Return ("Failed to load " + strIndexFile, false);
	ulong nRead = fread ( &m_vOffset[0], sizeof(char *), nSize, pFile);
	fclose(pFile);
	if( nRead != nSize ) Return ("Reading Failed.", false);

	if(!LoadText( strTablePath + STRING_FILE, m_strPool))
		Return("Failed to Load " + strTablePath + STRING_FILE, 0);

	return true;
}

void BstringTable::Append(string strItem)
{
	m_vOffset.push_back(m_strPool.size());
	m_strPool.append(strItem);
	m_strPool.push_back('\0');
}

bool BstringTable::Save(const string& strPath)
{
	if( !SaveFile(strPath + INDEX_FILE, (const void *)&m_vOffset[0], sizeof(size_t) * m_vOffset.size()))
		Return ("Faild to Write " + strPath + INDEX_FILE, false);
	if( !SaveText(strPath + STRING_FILE, m_strPool))
		Return ("Faild to Write " + strPath + STRING_FILE, false);

	return true;
}

void BstringTable::Print(FILE * f)
{
	fprintf(f, "Index\tOffset\tSize\tString\n");
	for (size_t i = 0; i < m_vOffset.size(); ++i)
		fprintf(f, "%lu\t%lu\t%lu\t%s\n", i, m_vOffset[i], strlen(At(i)), At(i));
}
