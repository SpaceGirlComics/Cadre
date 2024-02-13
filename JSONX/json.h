#ifndef JSON_H
#define JSON_H

enum JSONTYPE { JSON_NULL, JSON_STRING, JSON_NUMBER, JSON_OBJECT, JSON_ARRAY, JSON_BOOLEAN, JSON_MALFORMED, JSON_MEMDENIED, JSON_FILENF };
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
* @param _input - a string containing JSON formatted text
* @returns on success, a pointer to the new JSONOBJECT, otherwise, JSON_NULL
*/
LPJSONOBJECT jsonParse(char* _input);

/**
* Copies the object at address _source to the object at _dest
* @param _source - Pointer to the JSONOBJECT being duplicated
* @param _dest - Pointer to an empty JSONOBJCET that will hold the duplicate
* @returns on success, JSON_NULL, otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonCopy(LPJSONOBJECT _source, LPJSONOBJECT _dest);

/**
* copies the value in source to the address pointed to by destination
* @param _source - javascript object containing the desired value
* @param _dest - pointer that will point to the copy created
* @returns on success, JSON_NULL, otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonGet(LPJSONOBJECT _source, void* _dest);

/**
* Gets a member of _source using _path to determine which
* This function does not remove the member
* @param _source - the object with the member being sought
* @param _path - a json formatted string indicating the member being requested
* @returns a pointer to the member at the end of _path; NULL if the member is not found
*/
LPJSONOBJECT jsonGetByPath(LPJSONOBJECT _source, const char* _path);

/**
* Creates a json formatted string from a javascript object
* @param _json - The javascript object being encoded
* @param _output - address of the string that the json will be placed in
* @returns if the function is successful, the length of the output string. otherwise, -1
*/
enum JSONTYPE jsonEncode(LPJSONOBJECT _json, char** _output);

/**
* Deletes a javascript object and its members
* @param _json - The javascript object to be deleted
* @returns on success, JSON_NULL, otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonDestroy(LPJSONOBJECT _json);

/**
* Appends to a array or object
* @param _destination - The javascript object being added to
* @param _new - the object being being added
* @returns on success, JSON_NULL; otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonPush(LPJSONOBJECT _destination, LPJSONOBJECT _new);

/**
* palces _ new at the beginning of an object or array
* @param _destination - The javascript object being added to
* @param _new - the object being being added
* @returns on success, JSON_NULL; otherwise, JSON_MALFORMED
*/
enum JSONTYPE jsonUnshift(LPJSONOBJECT _destination, LPJSONOBJECT _new);

/**
* Removes the last member of an array or object
* @param _source - the array or object containg the member
* @returns the removed item or NULL if there is an error
*/
LPJSONOBJECT jsonPop(LPJSONOBJECT _source);

/**
* Removes the first member of an array or object
* @param _source - the array or object containg the member
* @returns the removed item or NULL if there is an error
*/
LPJSONOBJECT jsonShift(LPJSONOBJECT _source);

/**
* loads a JSON formatted text file
* Objects created with this function must be deleted using the jsonDestroy function
* @param _fileName - the name of a JSON formatted file
* @returns On success, the JSONOBJECT constructed from the file contents, otherwise zero
*/
LPJSONOBJECT jsonLoad(const char* _fileName);

/**
* Saves to a JSON formatted text file
* @param _json - object being saved
* @param _fileName - the name of a JSON formatted file
* @returns On success, JSON_NULL. Otherwise non-zero
*/
enum JSONTYPE jsonSave(LPJSONOBJECT _json, const char* _fileName);

#ifdef __cplusplus
}
#endif

#endif
