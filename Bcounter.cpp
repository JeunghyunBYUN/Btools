#include "Btools.h"
#include <sstream>

using namespace Btools;


string Bcounter::DumpCategory( map<string, ulong>::iterator it, bool bName, bool bTotal, bool bPercent)
{
	if(it == m_mCount.end()) return "";

	stringstream ss;
	if(bName) ss << it->first << ":\t";
	ss << Int2String(it->second);
	if(bTotal) ss <<  " / " << Int2String(m_Total);
	if(bPercent) ss << GetStringf(" ( %.2f%% )", it->second / (double) m_Total * 100); 

	return ss.str(); 
}

void Bcounter::PrintCategory( FILE * fout, bool bPercent)
{
	if( !m_Total ) return;
	for(map<string, ulong >::iterator it = m_mCount.begin(); it != m_mCount.end(); ++it)
		fprintf(fout, "%s\n", DumpCategory(it, true, false, bPercent).c_str());
}

void Bcounter::PrintAll( FILE * fout, bool bPercent)
{
	fprintf(fout, "Total:\t%lu\n", m_Total);
	PrintCategory(fout, bPercent);
}

