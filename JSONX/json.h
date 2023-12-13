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
* @param json - a string containing JSON formatted text
* @returns on success, a pointer to the new JSONOBJECT, otherwise, JSON_NULL
*/
LPJSONOBJECT jsonParse(char* json);

/**
* copies the value in source to the address pointed to by destination
* @param source - javascript object containing the desired value
* @param destination - pointer that will point to the copy created
* @returns on success, JSON_NULL, otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonGet(LPJSONOBJECT source, void* destination);

/**
* copies the value at the end of path, starting at source, to the address pointed to by destination
* @param source - javascript object; starting place for the traversal
* @param destination - pointer that will point to the copy created
* @param path - string containg the path to the desired value
* @returns on success, JSON_NULL, otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonPathGet(LPJSONOBJECT source, void* destination, char* path);

/**
* Creates a json formatted string from a javascript object
* @param json - The javascript object to be deleted
* @param output - address of the string that the json will be placed in
* @returns if the function is successful, the length of the output string. otherwise, -1
*/
int jsonEncode(LPJSONOBJECT json, char** output);

/**
* Deletes a javascript object and its members
* @param json - The javascript object to be deleted
* @returns on success, JSON_NULL, otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonDestroy(LPJSONOBJECT json);

#ifdef __cplusplus
}
#endif

#endif
