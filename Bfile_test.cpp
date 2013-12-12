#include <gtest/gtest.h>
#include "Btools.h"

using namespace Btools;

#define TEST_DIR	"test_data"
#define CONF_FILE	TEST_DIR "/Bfile_input.conf"
#define TSV_FILE	TEST_DIR "/Bfile_input.tsv"
#define OUT_FILE	TEST_DIR "/Bfile_output"

TEST(Bfile_test, GetLine_Save_Load_val)
{
	FILE * f = fopen(CONF_FILE, "r");
	ASSERT_TRUE(f);

	string strDump;
	string strLine;
	while(GetLine(strLine, f))
		strDump += strLine;
	fclose(f);

	ASSERT_TRUE(SaveText(OUT_FILE, strDump));

	string strBuffer;
	ASSERT_TRUE(LoadText(OUT_FILE, strBuffer));

	EXPECT_EQ(strBuffer, strDump);
}

TEST(Bfile_test, LoadConfig_val)
{
	string strBuffer;

	ASSERT_TRUE(LoadText(CONF_FILE, strBuffer));
	map<string, string> mConfig1;
	EXPECT_GT( LoadConfig(mConfig1, strBuffer), 0);

	ASSERT_TRUE(LoadText(TSV_FILE, strBuffer));
	map<string, string> mConfig2;
	EXPECT_GT( LoadConfig(mConfig2, strBuffer, "\t"), 0);

	EXPECT_EQ( mConfig1.size(), mConfig2.size());

	for(map<string, string>::iterator it=mConfig1.begin(); it!=mConfig1.end();++it)
	{
		EXPECT_TRUE( mConfig2.find(it->first) != mConfig2.end());
		EXPECT_TRUE( it->second == mConfig2[it->first]);
		//cout << it->first <<"\t" <<it->second <<endl;
	}
}

TEST(Bfile_test, StringTable_val)
{
	BstringTable strTable;

	string strBuffer;
	ASSERT_TRUE(LoadText( CONF_FILE, strBuffer));

	vector<string> vItem;
	Split( strBuffer, "\n", vItem);

	ASSERT_TRUE(strTable.Build( vItem, OUT_FILE));

	strTable.Clear();
	EXPECT_EQ(strTable.Size(), 0);
	ASSERT_TRUE(strTable.Load(OUT_FILE));

  	string strLine;
	size_t n =0;
	constsz pBuffer = strBuffer.c_str();
	while(GetLine(strLine, pBuffer))
		EXPECT_EQ(strLine, strTable[n++]);
}

