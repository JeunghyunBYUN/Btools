//#include <string.h>
//#include <stdlib.h>
//#include <iostream>
#include "Btools.h"

using namespace Btools;

void Bcounter_test(int n)
{
	cout <<" n = " <<n <<endl;
	Bcounter c(n);
	for(int i=0;i<5; i++)
	{
		c.Count("abc");
		c.PrintProc();
		c.Count("de");
		c.PrintProc();
		c.Count("abc");
		c.PrintProc();
	}

	c.PrintAll();
	cerr<<endl;
}

int main(int argc, char * argv[])
{
	cout <<GetNow() <<endl;
	cout <<GetNow("-") <<endl;
	cout <<endl;

	Bcounter_test(0);
	Bcounter_test(1);
	Bcounter_test(2);

	return 0;
}
