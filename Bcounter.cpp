#include "Btools.h"

using namespace Btools;


string Bcounter::DumpCategory( string strCategory, bool bName, bool bTotal, bool bPercent)
{
	map<string, unsigned long>::iterator it = m_mCount.find(strCategory);
	if ( it == m_mCount.end()) return "";
	string strOutput;
	DumpCategory( strOutput, it, bName, bTotal, bPercent);
	return strOutput;
}
void Bcounter::DumpCategory( string & strOutput, map<string, unsigned long>::iterator it, bool bName, bool bTotal, bool bPercent)
{
	strOutput.clear();
	strOutput.reserve(80);

	if(bName) strOutput = it->first +":\t";
	strOutput += Int2String(it->second);
	if(bTotal) strOutput += " / "+Int2String(m_Total);
	if(bPercent) strOutput += GetStringf(" ( %.2f%% )", it->second / (double) m_Total * 100); 
}

void Bcounter::PrintCategory( FILE * fout, bool bPercent)
{
	if( !m_Total ) return;
	string strOutput;
	for(map<string, unsigned long >::iterator it=m_mCount.begin(); it!=m_mCount.end(); ++it)
	{
		DumpCategory(strOutput, it, true, false, bPercent);
		fprintf(fout, "%s\n", strOutput.c_str());
	}
}
void Bcounter::PrintAll( FILE * fout, bool bPercent)
{
	fprintf(fout, "Total: %lu\n", m_Total);
	PrintCategory(fout, bPercent);
}

