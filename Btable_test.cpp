#include <gtest/gtest.h>
#include "Btools.h"

using namespace Btools;

#define TEST_DIR	"test_data"
#define CONF_FILE	TEST_DIR "/test.conf"
#define OUT_FILE	TEST_DIR "/test.output"

TEST(Bfile_test, BTable_val)
{
	Btable table;

	string strBuffer;
	ASSERT_TRUE(LoadText( CONF_FILE, strBuffer));

	vector<string> vItem;
	Split( strBuffer, "\n", vItem);

	ASSERT_TRUE(table.Build( vItem, OUT_FILE));

	table.Clear();
	EXPECT_EQ(table.Size(), 0);
	ASSERT_TRUE(table.Load(OUT_FILE));

  	string strLine;
	size_t n =0;
	constsz pBuffer = strBuffer.c_str();
	while(GetLine(strLine, pBuffer))
		EXPECT_EQ(strLine, table[n++]);
}

