#include "CTableDB.h"
#include "Csv.h"


struct SData {
	int id;
	int a;
	int b;
};

class TestTableDB : public CResTable<SData>
{
	virtual bool LoadLine(SLine& line, SData& data)
	{
		data.id = atoi(line["ID"]);
		data.a = atoi(line["a"]);
		data.b = atoi(line["b"]);

		return true;
	}
};

int main()
{
	TestTableDB testDB;
	if (!testDB.LoadTableBy_Id("", "test.csv"))
		return 0;

	//Csv csv("", "test.csv");
	return 1;
}