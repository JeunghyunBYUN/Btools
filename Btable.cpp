#include <Btools.h>

using namespace Btools;

#define INDEX_FILE   ".index"
#define STRING_FILE  ".string"


bool Btable::Load(const string& strTablePath)
{
	string strIndexFile = strTablePath + INDEX_FILE;
	long nFileSize = FileSize(strIndexFile.c_str());
	if( nFileSize < -1 ) return -1;
	size_t nSize = nFileSize / sizeof(char *);
	m_vOffset.resize( nSize );

	FILE * pFile = fopen( strIndexFile.c_str(), "r");
	if (!pFile) MsgReturn ("Failed to load " + strIndexFile, false);
	ulong nRead = fread ( &m_vOffset[0], sizeof(char *), nSize, pFile);
	fclose(pFile);
	if( nRead != nSize ) MsgReturn ("Reading Failed.", false);

	if(!LoadText( strTablePath + STRING_FILE, m_strPool))
		MsgReturn("Failed to Load " + strTablePath + STRING_FILE, 0);

	return true;
}

void Btable::Append(string strItem)
{
	m_vOffset.push_back(m_strPool.size());
	m_strPool.append(strItem);
	m_strPool.push_back('\0');
}

bool Btable::Save(const string& strPath)
{
	if( !SaveFile(strPath + INDEX_FILE, (const void *)&m_vOffset[0], sizeof(size_t) * m_vOffset.size()))
		MsgReturn ("Faild to Write " + strPath + INDEX_FILE, false);
	if( !SaveText(strPath + STRING_FILE, m_strPool))
		MsgReturn ("Faild to Write " + strPath + STRING_FILE, false);

	return true;
}

void Btable::Print(FILE * f)
{
	fprintf(f, "Index\tOffset\tSize\tString\n");
	for (size_t i = 0; i < m_vOffset.size(); ++i)
		fprintf(f, "%lu\t%lu\t%lu\t%s\n", i, m_vOffset[i], strlen(At(i)), At(i));
}


