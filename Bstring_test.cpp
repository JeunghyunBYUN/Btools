#include <string.h>
#include <set>
#include <gtest/gtest.h>
#include "Btools.h"

using namespace std;
using namespace Btools;

#define TEST_FILE	"test_data/sample.list"
const char szSample[] = " a bc  def last ";
const char szDelim[] = " ";
const int nTarget = 7;
const char *szTarget[nTarget] = {"", "a", "bc", "", "def", "last", ""};


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

TEST(Bstring_test, GetFirstSimple_val)
{
	string strFirst, strBuffer(szSample);
	for(int i=0;i<nTarget;++i)
	{
		strFirst = GetFirst(strBuffer, szDelim);
		EXPECT_EQ(szTarget[i], strFirst);
	}
}

TEST(Bstring_test, GetFirstConst_val)
{
	string strFirst;
	constsz szBuffer = szSample;
	for(int i=0;i<nTarget-1;++i)
	{
		ASSERT_TRUE(GetFirst(strFirst, szBuffer, szDelim));
		EXPECT_EQ(szTarget[i], strFirst);
	}
}

TEST(Bstring_test, strsep_val)
{
	string strBuffer(szSample);
	char * szBuffer = (char *)strBuffer.c_str();
	char * szFirst;
	for(int i=0;i<nTarget-1;++i)
	{
		ASSERT_TRUE((szFirst = strsep(&szBuffer, szDelim)) != NULL);
		EXPECT_STREQ(szTarget[i], szFirst);
	}
}

//TEST(Bstring_test, GetFirstFast_val)
//{
	//string strBuffer(szSample);
	//char * szBuffer = (char *)strBuffer.c_str();
	//char * szFirst;
	//for(int i=0;i<nTarget-1;++i)
	//{
		//ASSERT_TRUE((szFirst = GetFirst(szBuffer, szDelim)) != NULL);
		//EXPECT_STREQ(szTarget[i], szFirst);
	//}
//}

