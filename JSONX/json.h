#ifndef JSON_H
#define JSON_H

enum JSONTYPE { JSON_NULL, JSON_STRING, JSON_NUMBER, JSON_OBJECT, JSON_ARRAY, JSON_BOOLEAN, JSON_MALFORMED };
typedef struct _JSONOBJECT
{
	char* name;
	enum JSONTYPE type;
	void* value;
	int datalength;
	struct _JSONOBJECT* next;
} JSONOBJECT, * LPJSONOBJECT;

#ifdef __cplusplus
extern "C" {
#endif

/**
* creates a Javascript object from a JSON formatted string.
* Objects created with this function must be deleted using the jsonDestroy function
* @param json a string containing JSON formatted text
* @returns on success, a pointer to the new JSONOBJECT, otherwise, JSON_NULL
*/
LPJSONOBJECT jsonParse(char* json);
enum JSONTYPE jsonGet(LPJSONOBJECT source, void* destination);
enum JSONTYPE jsonPathGet(LPJSONOBJECT source, void* destination, char* path);
enum JSONTYPE jsonEncode(LPJSONOBJECT json, char* output);
enum JSONTYPE jsonDestroy(LPJSONOBJECT json);

#ifdef __cplusplus
}
#endif

#endif
