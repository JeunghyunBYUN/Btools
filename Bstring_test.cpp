#include <string.h>
#include <set>
#include <gtest/gtest.h>
#include "Btools.h"

using namespace std;
using namespace Btools;

#define TEST_FILE	"test_data/sample.list"

TEST(Bstring_test, TrimRight_val)
{
	string strSample("\t\t strLine \t\n");
	string strTarget("\t\t strLine");

	TrimRight(strSample);
	EXPECT_EQ( strTarget, strSample);
}

TEST(Bstring_test, Trim_val)
{
	string strSample("\t\t strLine \t\n");
	string strTarget("strLine");

	Trim(strSample);
	EXPECT_EQ( strTarget, strSample);
}

TEST(Bstring_test, Split_val)
{
  string strInput( "&&a&& b&&a&&c");
  char delim[]= "&&";
  vector<string> vStr;
  Split(strInput, delim, vStr);
  EXPECT_EQ( vStr[0], "");
  EXPECT_EQ( vStr[1], "a");
  EXPECT_EQ( vStr[2], " b");
  EXPECT_EQ( vStr[3], "a");
  EXPECT_EQ( vStr[4], "c");

  set<string> sStr;
  sStr.insert(vStr.begin(), vStr.end());
  EXPECT_EQ( 4, sStr.size());
}

TEST(Bstring_test, Replace_val)
{
  string strInput("  ab  cd   efg! ");
  string strTarget("_ab_cd_ efg! ");

  Replace(strInput, "  ", "_");
  EXPECT_EQ( strInput, strTarget);
}

void GetFirst_simple(string strInput, const char * szDelim, vector<string> * vStr)
{
	if(vStr)vStr->clear();
	string strHead;
	while(1)
	{
		strHead = GetFirst( strInput, szDelim);
		if( strHead.empty() && strInput.empty() ) break;
		if(vStr) vStr->push_back(strHead);
	}
}
void GetFirst_const(const char * pInput, const char * szDelim, vector<string> * vStr)
{
	if(vStr)vStr->clear();
	string strHead;
	while( GetFirst( strHead, pInput, szDelim) )
		if(vStr) vStr->push_back(strHead);
}

void GetFirst_fast(char * pInput, const char * szDelim, vector<string> * vStr)
{
	if(vStr)vStr->clear();
	char * pHead;
	while( (pHead = GetFirst(pInput, szDelim)) )
		if(vStr) vStr->push_back(pHead);
}

void strsep_test(char * ptr, const char * szDelim, vector<string> * vStr)
{
	if(vStr)vStr->clear();
	char * pHead;
	for( pHead = strsep(&ptr, szDelim); ptr != NULL ;  pHead = strsep(&ptr, szDelim))
		if(vStr) vStr->push_back(pHead);
	if(vStr) vStr->push_back(pHead);
}

TEST(Bstring_test, GetFirst_val)
{
	const char    szSample[] = " a bc  def last";
	const char    szDelim[] = " ";
	vector<string> vTarget;
	vTarget.push_back("");
	vTarget.push_back("a");
	vTarget.push_back("bc");
	vTarget.push_back("");
	vTarget.push_back("def");
	vTarget.push_back("last");

	vector<string> vStr;

	GetFirst_simple(szSample, szDelim, &vStr);
	EXPECT_EQ( vTarget.size(), vStr.size());
	for(size_t i=0;i<vStr.size(); ++i)
		EXPECT_EQ( vTarget[i], vStr[i]);

	GetFirst_const(szSample, szDelim, &vStr);
	EXPECT_EQ( vTarget.size(), vStr.size());
	for(size_t i=0;i<vStr.size(); ++i)
		EXPECT_EQ( vTarget[i], vStr[i]);

	GetFirst_fast((char *)szSample, szDelim, &vStr);
	EXPECT_EQ( vTarget.size(), vStr.size());
	for(size_t i=0;i<vStr.size(); ++i)
		EXPECT_EQ( vTarget[i], vStr[i]);

	char    szSample2[] = " a bc  def last";
	strsep_test( szSample2, szDelim, &vStr);
	EXPECT_EQ( vTarget.size(), vStr.size());
	for(size_t i=0;i<vStr.size(); ++i)
		EXPECT_EQ( vTarget[i], vStr[i]);
}

void GetFirst_speed(const char * pFileName)
{
	char   szDelim[] = "\n";
	BstopWatch w;
	string strBuffer;

	//cout<<"- GetFirst(simple) Speed"<<endl;
	//LoadText(pFileName, strBuffer);
	//w.Restart();
	//GetFirst_simple(strBuffer, szDelim, NULL);
	//w.Print(stdout);

	cout<<"- GetFirst(const) Speed"<<endl;
	LoadText(pFileName, strBuffer);
	w.Restart();
	GetFirst_const(strBuffer.c_str(), szDelim, NULL);
	w.Print(stdout);

	cout<<"- GetFirst(fast) Speed"<<endl;
	LoadText(pFileName, strBuffer);
	w.Restart();
	GetFirst_fast((char*)strBuffer.c_str(), szDelim, NULL);
	w.Print(stdout);

	cout<<"- strtok Speed"<<endl;
	LoadText(pFileName, strBuffer);
	char * pHead;
	char * ptr = (char*)strBuffer.c_str();
	w.Restart();
	for( pHead = strtok(ptr, szDelim); pHead!=NULL ;  pHead = strtok(NULL, szDelim))
		;
	w.Print(stdout);

	cout<<"- strsep Speed"<<endl;
	LoadText(pFileName, strBuffer);
	ptr = (char*)strBuffer.c_str();
	w.Restart();
	strsep_test( ptr, szDelim, NULL);
	w.Print(stdout);
}

void Split_speed(const char * pFileName)
{
	cout<<"- Split Speed"<<endl;
	BstopWatch w;
	string strBuffer;
	vector<string> vStr;
	assert(LoadText(pFileName, strBuffer));
	
	w.Start();
	Split(strBuffer, "\n", vStr);
	w.Print(stderr);
}

int main(int argc, char **argv) 
{
	GetFirst_speed(TEST_FILE);
	Split_speed(TEST_FILE);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

