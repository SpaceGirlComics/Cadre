#include "json.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char* jsonString = (char*)"{\"thisIsANumber\": 40, \"anotherNumber\": 3.14, \"andAnother\":1234123.4322, \"thisIsAString\":\"thisisastring\", \"andAnotherString\":\"andanotherstring\", \"null1\":null, \"bool1\":false, \"null2\":null, \"bool2\":true, \"bool3\":false, \"innerObject\": {\"field1\":\"string\", \"field2\": 1}, \"array\":[40, 3.14, 123424.11, \"ThisIsAString\", \"AndAnotherString\", null, false, null, true, false, \"innerObject\": {\"field1\":\"string\", \"field2\": 1}}";
	LPJSONOBJECT jo;
	jo = jsonParse(jsonString);
	return(EXIT_SUCCESS);
}
