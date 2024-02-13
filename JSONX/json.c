#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "json.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NULLIFY(a)			if(a){free(a);} a = 0
#ifdef UNICODE
#define STRCAT(a, b)		wcscat(a, b)
#define STRCPY(a, b)		wcscpy(a, b)
#define STRCMP(a, b)		wcscmp(a, b)
#define STRLEN(a)			wcslen(a)
#define STRDUP(a)			wcsdup(a)

#define JSONISNULL(a)		a[0] == 'n' && a[1] == 'u' && a[2] == 'l' && a[3] == 'l'
#define JSONISTRUE(a)		a[0] == 't' && a[1] == 'r' && a[2] == 'u' && a[3] == 'e'
#define JSONISFALSE(a)		a[0] == 'f' && a[1] == 'a' && a[2] == 'l' && a[3] == 's' && a[4] == 'e'

#define CASEJSONOBJECT		case '{':
#define CASEJSONARRAY		case '[':
#define CASEJSONSTRING		case '\"':
#define CASEJSONNUMBER		case '-': \
							case '+': \
							case '.': \
							case 'e': \
							case '0': \
							case '1': \
							case '2': \
							case '3': \
							case '4': \
							case '5': \
							case '6': \
							case '7': \
							case '8': \
							case '9': 
#define CASEJSONBOOLEAN		case 't': \
							case 'f':
#define CASEJSONNULL		case 'n':
#define CASEJSONKEY			case ':':
#define CASEJSONWHITESPACE	case ' ': \
							case '\n': \
							case '\r': \
							case '\t':
#define CASEJSONNEXT		case ',':
#else
#define STRCAT(a, b)		strcat(a, b)
#define STRCPY(a, b)		strcpy(a, b)
#define STRCMP(a, b)		strcmp(a, b)
#define STRLEN(a)			strlen(a)
#define STRDUP(a)			strdup(a)

#define JSONISNULL(a)		a[0] == 'n' && a[1] == 'u' && a[2] == 'l' && a[3] == 'l'
#define JSONISTRUE(a)		a[0] == 't' && a[1] == 'r' && a[2] == 'u' && a[3] == 'e'
#define JSONISFALSE(a)		a[0] == 'f' && a[1] == 'a' && a[2] == 'l' && a[3] == 's' && a[4] == 'e'

#define CASEJSONOBJECT		case '{':
#define CASEJSONARRAY		case '[':
#define CASEJSONSTRING		case '\"':
#define CASEJSONNUMBER		case '-': \
							case '+': \
							case '.': \
							case 'e': \
							case '0': \
							case '1': \
							case '2': \
							case '3': \
							case '4': \
							case '5': \
							case '6': \
							case '7': \
							case '8': \
							case '9': 
#define CASEJSONBOOLEAN		case 't': \
							case 'f':
#define CASEJSONNULL		case 'n':
#define CASEJSONKEY			case ':':
#define CASEJSONWHITESPACE	case ' ': \
							case '\n': \
							case '\r': \
							case '\t': 
#define CASEJSONNEXT		case ',':
#endif

/**
* Doubles the capacity of a buffer by creating a new buffer and copying to it
* @param _buffer - a double pointer to the existing buffer
* @param _size - a pointer to the current size
* @returns on success, JSON_NULL. if arguments used are not valid, JSON_MALFORMED.
* if the buffer could not be created, JSON_MEMDENIED
*/
enum JSONTYPE jsonStringDoubleCapacity(char** _buffer, size_t* _size)
{
	if (!_buffer || !*_buffer || !_size)
	{
		return(JSON_MALFORMED);
	}
	*_size *= 2;
	char* newString = calloc(*_size, sizeof(char));
	if (!newString)
	{
		return(JSON_MEMDENIED);
	}
	strcpy(newString, *_buffer);
	free(*_buffer);
	*_buffer = newString;
	return(JSON_NULL);
}

/**
* Trims the size of the buffer to the length of the string
* @param _buffer - a double pointer to the existing buffer
* @param _size - a pointer to the current size
* @returns on success, JSON_NULL. if arguments used are not valid, JSON_MALFORMED.
* if the buffer could not be created, JSON_MEMDENIED
*/
enum JSONTYPE jsonStringTrimCapacity(char** _buffer, size_t* _size)
{
	if (!_buffer || !*_buffer || !_size)
	{
		return(JSON_MALFORMED);
	}
	size_t len = strlen(*_buffer);
	if (len + 1 < *_size)
	{
		*_size = len + 1;
		char* newString = calloc(*_size, sizeof(char));
		if (!newString)
		{
			return(JSON_MEMDENIED);
		}
		strcpy(newString, *_buffer);
		free(*_buffer);
		*_buffer = newString;
		return(JSON_NULL);
	}
	return(JSON_MALFORMED);
}

/**
* Doubles the size of a buffer if the currents size is inssuficient
* @param _dest - a double pointer to the destination
* @param _source - a pointer to the string being concatenated
* @param _size - pointer to the size of the _dest buffer
* @returns on success, JSON_NULL. if arguments used are not valid, JSON_MALFORMED.
* if the buffer could not be created, JSON_MEMDENIED
*/
enum JSONTYPE jsonStringEnsureCapacity(char** _dest, char* _source, size_t* _size)
{
	if (!_dest || !*_dest || !_source || !_size)
	{
		return(JSON_MALFORMED);
	}
	size_t dlength = strlen(*_dest);
	size_t slength = strlen(_source);

	while (dlength + slength > *_size - 1)
	{
		if (jsonStringDoubleCapacity(_dest, _size))
		{
			return(JSON_MEMDENIED);
		}
	}
	return(JSON_NULL);
}

/**
* gets a string from JSON formatted text and encapsulates it in a JSONOBJECT
* Objects created with this function must be deleted using the jsonDestroy function
* @param _input - a string containing JSON formatted text
* @param _json - pointer to an object that will be filled
* @returns on success, the number of characters parsed, otherwise, JSON_MALFORMED
*/
int jsonString(char* _input, LPJSONOBJECT _json)
{
	_json->type = JSON_STRING;
	int size = 0;
	while (_input[size] != '\"')
	{
		if (_input[size] == '\\')
		{
			size++;
			if (_input[size] == 'u')
			{
				size += 4;
			}
		}
		size++;
	}
	_json->datalength = size+1;
	_json->value = malloc(_json->datalength);
	if (_json->value)
	{
		memcpy(_json->value, _input, size);
		((char*)_json->value)[size] = '\0';
	}
	return(++size);
}

/**
* gets a number from JSON formatted text and encapsulates it in a JSONOBJECT
* Objects created with this function must be deleted using the jsonDestroy function
* @param _input - a string containing JSON formatted text
* @param _json - pointer to an object that will be filled
* @returns on success, the number of characters parsed, otherwise, JSON_MALFORMED
*/
int jsonNumber(char* _input, LPJSONOBJECT _json)
{
	_json->type = JSON_NUMBER;
	_json->datalength = sizeof(double);
	_json->value = malloc(sizeof(double));
	sscanf(_input, "%lf", (double*)_json->value);
	int count = 0;
	while (_input[count] != ',' && _input[count] != '}' && _input[count] != ']')
	{
		count++;
	}
	return(count-1);
}

/**
* gets a boolean from JSON formatted text and encapsulates it in a JSONOBJECT
* Objects created with this function must be deleted using the jsonDestroy function
* @param _input - a string containing JSON formatted text
* @param _json - pointer to an object that will be filled
* @returns on success, the number of characters parsed, otherwise, JSON_MALFORMED
*/
int jsonBool(char* _input, LPJSONOBJECT _json)
{
	_json->type = JSON_BOOLEAN;
	_json->datalength = sizeof(unsigned char);
	_json->value = malloc(sizeof(unsigned char));
	int count = 0;
	if (JSONISTRUE(_input))
	{
		*(char*)_json->value = 1;
		count += 3;
	}
	else if (JSONISFALSE(_input))
	{
		*(char*)_json->value = 0;
		count += 4;
	}
	else
	{
		NULLIFY(_jsonObject->value);
		_jsonObject->datalength = 0;
		_jsonObject->type = JSON_MALFORMED;
	}
	return(count);
}

/**
* gets a null from JSON formatted text and encapsulates it in a JSONOBJECT
* Objects created with this function must be deleted using the jsonDestroy function
* @param _input - a string containing JSON formatted text
* @param _json - pointer to an object that will be filled
* @returns on success, the number of characters parsed, otherwise, JSON_MALFORMED
*/
int jsonNull(char* _input, LPJSONOBJECT _json)
{
	int count = 0;
	if (JSONISNULL(_input))
	{
		_json->type = JSON_NULL;
		_json->datalength = 0;
		_json->value = 0;
		count += 3;
	}
	else
	{
		_json->type = JSON_MALFORMED;
	}
	return(count);
}

/**
* gets an object or an array from JSON formatted text
* Objects created with this function must be deleted using the jsonDestroy function
* @param _input - a string containing JSON formatted text
* @param _json - pointer to an object that will be filled
* @returns on success, the number of characters parsed, otherwise, JSON_MALFORMED
*/
int jsonObject(char* _input, LPJSONOBJECT _json)
{
	char delimeter;
	int* counter = 0;
	if (_input[0] == '{')
	{
		_json->type = JSON_OBJECT;
		_json->datalength = 0;
		delimeter = '}';
	}
	else
	{
		_json->type = JSON_ARRAY;
		counter = &_json->datalength;
		*counter = 0;
		delimeter = ']';
	}
	_json->value = malloc(sizeof(JSONOBJECT));
	_json->next = 0;
	_json = _json->value;
	int x;

	for (x = 1; _input[x] != delimeter; x++)
	{
		if (delimeter == ']')
		{
			_json->name = 0;
		}
		switch (_input[x])
		{
			CASEJSONARRAY
			CASEJSONOBJECT
			{
				x += jsonObject(&_input[x], _json);
				if (delimeter == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONSTRING
			{
				x += jsonString(&_input[x + 1], _json);
				if (delimeter == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONNUMBER
			{
				x += jsonNumber(&_input[x], _json);
				if (delimeter == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONBOOLEAN
			{
				x += jsonBool(&_input[x], _json);
				if (delimeter == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONNULL
			{
				x += jsonNull(&_input[x], _json);
				if (delimeter == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONKEY
			{
				if (_json->type == JSON_STRING)
				{
					_json->name = _json->value;
					_json->value = 0;
				}
				else
				{
					_json->type = JSON_MALFORMED;
				}
				break;
			}

			CASEJSONWHITESPACE
			{
				break;
			}

			CASEJSONNEXT
			{
				_json->next = malloc(sizeof(JSONOBJECT));
				_json = _json->next;
				_json->next = 0;
				
				break;
			}

		default:
			{
				_json->type = JSON_MALFORMED;
				break;
			}
		}
	}
	return(x);
}

LPJSONOBJECT jsonParse(char* _input)
{
	LPJSONOBJECT root		= malloc(sizeof(JSONOBJECT));
	LPJSONOBJECT current	= root;
	current->datalength		= 0;
	current->name			= 0;
	current->next			= 0;
	current->type			= JSON_MALFORMED;
	current->value			= 0;
	
	size_t length = strlen(_input);

	for (int x = 0; x < length; x++)
	{
		switch (_input[x])
		{
			CASEJSONARRAY
			CASEJSONOBJECT
			{
				x = jsonObject(&_input[x], current);
				break;
			}

			CASEJSONSTRING
			{
				x = jsonString(&_input[x + 1], current);
				break;
			}

			CASEJSONNUMBER
			{
				x = jsonNumber(&_input[x], current);
				break;
			}

			CASEJSONBOOLEAN
			{
				x = jsonBool(&_input[x + 1], current);
				break;
			}

			CASEJSONNULL
			{
				x = jsonNull(&_input[x + 1], current);
				break;
			}

			CASEJSONKEY
			{
				if (current->type == JSON_STRING)
				{
					current->name = current->value;
					current->value = 0;
				}
				else
				{
					current->type = JSON_MALFORMED;
				}
				break;
			}

			CASEJSONWHITESPACE
			{
				break;
			}

			default:
			{
				current->type = JSON_MALFORMED;
				break;
			}
		}
	}
	return(root);
}

enum JSONTYPE jsonCopy(LPJSONOBJECT _source, LPJSONOBJECT _dest)
{
	if (!_source || !_dest)
	{
		return(JSON_MALFORMED);
	}

	_dest->name = _strdup(_source->name);
	_dest->type = _source->type;
	_dest->datalength = _source->datalength;
	_dest->next = 0;

	switch (_source->type)
	{
		case JSON_STRING:
		{
			_dest->value = _strdup(_source->value);
			break;
		}

		case JSON_BOOLEAN:
		case JSON_NULL:
		case JSON_NUMBER:
		{
			_dest->value = malloc(_dest->datalength);
			if (_dest->value)
			{
				memcpy(_dest->value, _source->value, _source->datalength);
			}
			else
			{
				return(JSON_MEMDENIED);
			}
			break;
		}

		case JSON_ARRAY:
		case JSON_OBJECT:
		{
			_dest->value = malloc(sizeof(JSONOBJECT));
			jsonCopy(_source->value, _dest->value);
			break;
		}

		default:
		{
			return(JSON_MALFORMED);
		}
	}

	if (_source->next)
	{
		_dest->next = malloc(sizeof(JSONOBJECT));

		jsonCopy(_source->next, _dest->next);
	}

	return(JSON_NULL);
}

enum JSONTYPE jsonGet(LPJSONOBJECT _source, void* _dest)
{
	switch (_source->type)
	{
		case JSON_STRING:
		{
			strcpy(_dest, _source->value);
			break;
		}

		case JSON_NUMBER:
		{
			*((double*)_dest) = *((double*)_source->value);
			break;
		}

		case JSON_BOOLEAN:
		{
			*((unsigned char*)_dest) = *((unsigned char*)_source->value);
			break;
		}

		case JSON_NULL:
		{
			_dest = 0;
			break;
		}

		case JSON_ARRAY:
		case JSON_OBJECT:
		{
			jsonCopy(_source->value, _dest);
			break;
		}

		default:
		{
			return(JSON_MALFORMED);
			break;
		}
	}
	return(JSON_NULL);
}

/**
* Gets the object at position _index in _array
* This function does not remove objects from _array
* @param _array - The array object
* @param _index - the position in the array that is needed
* @returns on success, a pointer to the requested object, otherwise NULL (0)
*/
LPJSONOBJECT jsonArrayGet(LPJSONOBJECT _array, int _index)
{
	LPJSONOBJECT current = 0;
	if (_array && _array->value && _array->type == JSON_ARRAY)
	{
		current = _array->value;
		if (_index < 0)
		{
			_index += _array->datalength;
		}

		if (_index >= 0 && _index < _array->datalength)
		{
			for (int x = 0; x < _index; x++)
			{
				current = current->next;
			
			}
		}
	}
	return(current);
}

/**
* Gets a member from an object by its name
* This function does not remove objects from _object
* @param _source - The object
* @param _name - the name of the member
* @returns on success, a pointer to the requested member, otherwise NULL (0)
*/
LPJSONOBJECT jsonObjectGet(LPJSONOBJECT _source, char* _name)
{
	LPJSONOBJECT current = 0;
	if (_source && _source->type == JSON_OBJECT && _name)
	{
		current = _source->value;
		while (current && strcmp(current->name, _name))
		{
			current = current->next;
		}
	}
	return(current);
}

LPJSONOBJECT jsonGetByPath(LPJSONOBJECT _source, const char* _path)
{
	LPJSONOBJECT current = 0;
	if (!(_source && _path))
	{
		return(current);
	}

	char* path = _strdup(_path);
	if (!path)
	{
		return(current);
	}

	current = _source;
	char* name = strtok(path, ".");
	int index = 0;
	while (name)
	{
		int len = strlen(name);
		 
		if (name[len-1] == ']')
		{
			for(int y = len; y > 0; y--)
			{
				if (name[y-1] == '[')
				{
					name[y - 1] = '\0';
					index = atoi(&name[y]);
					y = 0;
				}
			}
			current = jsonArrayGet(jsonObjectGet(current, name), index);
			if (!current)
			{
				NULLIFY(path);
				return(0);
			}
		}
		else
		{
			current = jsonObjectGet(current, name);
			if (!current)
			{
				NULLIFY(path);
				return(0);
			}
		}
		name = strtok(0, ".");
	}

	free(path);
	return(current);
}

enum JSONTYPE jsonEncodeObject(LPJSONOBJECT _json, char** _buffer, size_t* _size);

/**
* Encodes primitives and strings into JSON formatted text and adds it the _buffer
* @param _json - The object being encoded
* @param _buffer - a double pointer to the text buffer this value will be added to
* @param _size - a pointer to the size of the buffer at _buffer
* @returns on success, JSON_NULL, otherwise non-zero
*/
enum JSONTYPE jsonEncodePrimitive(LPJSONOBJECT _json, char** _buffer, size_t* _size)
{
	if (!_json || !_buffer || !*_buffer || !_size)
	{
		return(JSON_MALFORMED);
	}
	size_t nsSize = 10;
	char* newString = calloc(nsSize, sizeof(char));
	if (!newString)
	{
		return(JSON_MEMDENIED);
	}
	switch (_json->type)
	{
		case JSON_NUMBER:
		{
			snprintf(newString, nsSize, "%lf", *((double*)_json->value));
			break;
		}

		case JSON_BOOLEAN:
		{
			snprintf(newString, nsSize, "%s", *((char*)_json->value) ? "true" : "false");
			break;
		}

		case JSON_NULL:
		{
			snprintf(newString, nsSize, "null");
			break;
		}

		case JSON_STRING:
		{
			if (jsonStringEnsureCapacity(&newString, (char*)_json->value, &nsSize))
			{
				return(JSON_MEMDENIED);
			}
			snprintf(newString, nsSize, "%s", (char*)_json->value);
			break;
		}

		default:
		{
			free(newString);
			return(JSON_MALFORMED);
		}
	}
	if (jsonStringEnsureCapacity(_buffer, newString, _size))
	{
		free(newString);
		return(JSON_MEMDENIED);
	}
	strcat(*_buffer, newString);
	free(newString);
	return(JSON_NULL);
}

/**
* Encodes an array into JSON formatted text and adds it the _buffer
* @param _json - The object being encoded
* @param _buffer - a double pointer to the text buffer this value will be added to
* @param _size - a pointer to the size of the buffer at buffer
* @returns on success, JSON_NULL, otherwise non-zero
*/
enum JSONTYPE jsonEncodeArray(LPJSONOBJECT _json, char** _buffer, size_t* _size)
{
	if (!_json || !_buffer || !*_buffer || !_size)
	{
		return(JSON_MALFORMED);
	}
	jsonStringEnsureCapacity(_buffer, "{", _size);
	strcat(*_buffer, "[");
	LPJSONOBJECT current = _json->value;
	while (current)
	{
		switch (current->type)
		{
			case JSON_OBJECT:
			{
				if (jsonEncodeObject(current, _buffer, _size))
				{
					return(JSON_MEMDENIED);
				}
				break;
			}

			case JSON_ARRAY:
			{
				if (jsonEncodeArray(current, _buffer, _size))
				{
					return(JSON_MEMDENIED);
				}
				break;
			}

			case JSON_STRING:
			case JSON_NUMBER:
			case JSON_BOOLEAN:
			case JSON_NULL:
			{
				if (jsonEncodePrimitive(current, _buffer, _size))
				{
					return(JSON_MEMDENIED);
				}
				break;
			}
		}
		current = current->next;
		jsonStringEnsureCapacity(_buffer, "{", _size);
		strcat(*_buffer, ",");
	}
	(*_buffer)[strlen(*_buffer)-1] = ']';
	return(JSON_NULL);
}

/**
* Encodes an object into JSON formatted text and adds it the _buffer
* @param _json - The object being encoded
* @param _buffer - a double pointer to the text buffer this value will be added to
* @param _size - a pointer to the size of the buffer at buffer
* @returns on success, JSON_NULL, otherwise non-zero
*/
enum JSONTYPE jsonEncodeObject(LPJSONOBJECT _json, char** _buffer, size_t* _size)
{
	if (!_json || !_buffer || !*_buffer || !_size)
	{
		return(JSON_MALFORMED);
	}
	jsonStringEnsureCapacity(_buffer, "{", _size);
	strcat(*_buffer, "{");
	LPJSONOBJECT current = _json->value;
	while (current)
	{
		if (current->name)
		{
			jsonStringEnsureCapacity(_buffer, "\"", _size);
			strcat(*_buffer, "\"");
			jsonStringEnsureCapacity(_buffer, current->name, _size);
			strcat(*_buffer, current->name);
			jsonStringEnsureCapacity(_buffer, "\":", _size);
			strcat(*_buffer, "\":");
		}

		switch (current->type)
		{
			case JSON_OBJECT:
			{
				if (jsonEncodeObject(current, _buffer, _size))
				{
					return(JSON_MEMDENIED);
				}
				break;
			}

			case JSON_ARRAY:
			{
				if (jsonEncodeArray(current, _buffer, _size))
				{
					return(JSON_MEMDENIED);
				}
				break;
			}

			case JSON_STRING:
			case JSON_NUMBER:
			case JSON_BOOLEAN:
			case JSON_NULL:
			{
				if (jsonEncodePrimitive(current, _buffer, _size))
				{
					return(JSON_MEMDENIED);
				}
				break;
			}
		}
		current = current->next;
		jsonStringEnsureCapacity(_buffer, "{", _size);
		strcat(*_buffer, ",");
	}
	(*_buffer)[strlen(*_buffer) - 1] = '}';
	return(JSON_NULL);
}

/**
* Encodes JSONOBJECT into JSON formatted text
* @param _json - The object being encoded
* @param _buffer - a double pointer to the text buffer this value will be added to
* @returns on success, JSON_NULL, otherwise non-zero
*/
enum JSONTYPE jsonEncode(LPJSONOBJECT _json, char** _buffer)
{
	if (!_json || !_buffer)
	{
		return(JSON_MALFORMED);
	}
	size_t size = 100;
	*_buffer = calloc(size, sizeof(char));
	switch (_json->type)
	{
		case JSON_OBJECT:
		{
			if (jsonEncodeObject(_json, _buffer, &size))
			{
				return(JSON_MEMDENIED);
			}
			break;
		}

		case JSON_ARRAY:
		{
			if (jsonEncodeArray(_json, _buffer, &size))
			{
				return(JSON_MEMDENIED);
			}
			break;
		}

		case JSON_STRING:
		case JSON_NUMBER:
		case JSON_BOOLEAN:
		case JSON_NULL:
		{
			if (jsonEncodePrimitive(_json, _buffer, &size))
			{
				return(JSON_MEMDENIED);
			}
			break;
		}
	}
	jsonStringTrimCapacity(_buffer, &size);
	return(JSON_NULL);
}

/**
* deletes objects containg strings, numbers, booleans, and nulls
* @param _json pointer to the object being deleted
* @returns On success, JSON_NULL; otherwise JSON_MALFORMED
*/
enum JSONTYPE jsonDestroyPrimitive(LPJSONOBJECT _json)
{
	if (_json)
	{
		NULLIFY(_json->name);
		NULLIFY(_json->value);
		NULLIFY(_json);
		return(JSON_NULL);
	}
	return(JSON_MALFORMED);
}

/**
* deletes objects and arrays
* @param _json pointer to the object being deleted
* @returns On success, JSON_NULL; otherwise JSON_MALFORMED
*/
enum JSONTYPE jsonDestroyObject(LPJSONOBJECT _json)
{
	LPJSONOBJECT current;
	LPJSONOBJECT next;
	if (_json)
	{
		NULLIFY(_json->name);
		current = _json->value;
		while (current)
		{
			next = current->next;
			switch (current->type)
			{
				case JSON_NULL:
				case JSON_BOOLEAN:
				case JSON_NUMBER:
				case JSON_STRING:
				{
					jsonDestroyPrimitive(current);
					break;
				}

				case JSON_ARRAY:
				case JSON_OBJECT:
				{
					jsonDestroyObject(current);
					break;
				}

				default:
				{
					return(JSON_MALFORMED);
				}
			}
			current = next;
		}
		NULLIFY(_json);
	}
	return(JSON_NULL);
}

enum JSONTYPE jsonDestroy(LPJSONOBJECT _json)
{
	switch (_json->type)
	{
		case JSON_NULL:
		case JSON_BOOLEAN:
		case JSON_NUMBER:
		case JSON_STRING:
		{
			return(jsonDestroyPrimitive(_json));
			break;
		}

		case JSON_ARRAY:
		case JSON_OBJECT:
		{
			return(jsonDestroyObject(_json));
			break;
		}
	}
	return(JSON_MALFORMED);
}

enum JSONTYPE jsonPush(LPJSONOBJECT _dest, LPJSONOBJECT _input)
{
	if (_dest && _input)
	{
		if (_dest->type != JSON_OBJECT && _dest->type != JSON_ARRAY)
		{
			return(JSON_MALFORMED);
		}
		LPJSONOBJECT current = _dest->value;
		while (current->next)
		{
			current = current->next;
		}
		current->next = _input;
		return(JSON_NULL);
	}
	return(JSON_MALFORMED);
}

enum JSONTYPE jsonUnshift(LPJSONOBJECT _dest, LPJSONOBJECT _input)
{
	if (_dest && _input)
	{
		if (_dest->type != JSON_OBJECT && _dest->type != JSON_ARRAY)
		{
			return(JSON_MALFORMED);
		}
		if (_input->next)
		{
			if (!jsonDestroy(_input->next))
			{
				return(JSON_MALFORMED);
			}
		}
		_input->next = _dest->value;
		_dest->value = _input;
		return(JSON_NULL);
	}
	return(JSON_MALFORMED);
}

LPJSONOBJECT jsonPop(LPJSONOBJECT _source)
{
	LPJSONOBJECT current = 0;
	LPJSONOBJECT prev = 0;
	if (_source && _source->value && (_source->type == JSON_ARRAY || _source->type == JSON_OBJECT))
	{
		current = _source->value;
		while (current->next)
		{
			prev = current;
			current = current->next;
		}
		if (prev)
		{
			prev->next = 0;
		}
	}
	return(current);
}

LPJSONOBJECT jsonShift(LPJSONOBJECT _source)
{
	LPJSONOBJECT current = 0;
	if (_source && _source->value && (_source->type == JSON_ARRAY || _source->type == JSON_OBJECT))
	{
		current = _source->value;
		_source->value = current->next;
		current->next = 0;
	}
	return(current);
}

/**
* Creates a JSON string object from a file
* @param _json - object encapsulating the string
* @param _file - Pointer to the file containing the string
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonLoadString(LPJSONOBJECT _json, FILE* _file)
{
	if (!_json || !_file)
	{
		return(JSON_MALFORMED);
	}
	size_t size = 256;
	char* stringRead = calloc(size, sizeof(char));
	if (!stringRead)
	{
		return(JSON_MEMDENIED);
	}
	int index = 0;
	char current;
	while (fscanf(_file, "%c", &current) == 1 && current != '\"')
	{
		stringRead[index++] = current;
		stringRead[index] = '\0';
		if (size - index <= 2)
		{
			if (jsonStringEnsureCapacity(&stringRead, "1", &size))
			{
				free(stringRead);
				return(JSON_MEMDENIED);
			}
		}
	}
	
	if(jsonStringTrimCapacity(&stringRead, &size))
	{
		free(stringRead);
		return(JSON_MEMDENIED);
	}
	_json->value = stringRead;
	_json->datalength = strlen(stringRead);
	return(JSON_NULL);
}

/**
* Creates a JSON boolean object from a file
* @param _json - object encapsulating the boolean
* @param _file - Pointer to the file containing the bool
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonLoadBoolean(LPJSONOBJECT _json, FILE* _file)
{
	fseek(_file, -1, SEEK_CUR);
	
	_json->datalength = sizeof(char);
	_json->value = malloc(sizeof(char));
	if (!_json->value)
	{
		return(JSON_MEMDENIED);
	}
	char temp[6];
	fscanf(_file, "%c%c%c%c", &temp[0], &temp[1], &temp[2], &temp[3]);
	temp[4] = '\0';
	if (!strcmp(temp, "true"))
	{
		(*(char*)_json->value) = 1;
		return(JSON_NULL);
	}
	fscanf(_file, "%c", &temp[4]);
	temp[5] = '\0';
	if (!strcmp(temp, "false"))
	{
		(*(char*)_json->value) = 0;
		return(JSON_NULL);
	}
	return(JSON_MALFORMED);
}

/**
* Creates a JSON string object from a file
* @param _json - object encapsulating the string
* @param _file - Pointer to the file containing the string
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonLoadNumber(LPJSONOBJECT _json, FILE* _file)
{
	fseek(_file, -1, SEEK_CUR);

	_json->datalength = sizeof(double);
	_json->value = malloc(_json->datalength);
	if (!_json->value)
	{
		return(JSON_MEMDENIED);
	}
	fscanf(_file, "%lf", (double*)_json->value);
	return(JSON_NULL);
}

/**
* Creates a JSON null object from a file
* @param _json - object encapsulating the string
* @param _file - Pointer to the file containing the string
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonLoadNull(LPJSONOBJECT _json, FILE* _file)
{
	fseek(_file, -1, SEEK_CUR);
	char temp[5];
	int x = 0;
	fscanf(_file, "%c%c%c%c", &temp[0], &temp[1], &temp[2], &temp[3]);
	temp[4] = '\0';
	if (!strcmp(temp, "null"))
	{
		_json->datalength = 0;
		_json->value = 0;
		return(JSON_NULL);
	}
	return(JSON_MALFORMED);
}

/**
* Creates a JSON object from a file
* @param _json - encapsulating object
* @param _file - Pointer to the file containing the string
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonLoadObject(LPJSONOBJECT _json, FILE* _file)
{
	if (!_json || !_file)
	{
		return(JSON_MALFORMED);
	}

	LPJSONOBJECT current = malloc(sizeof(JSONOBJECT));
	if (!current)
	{
		return(JSON_MEMDENIED);
	}
	_json->value = current;
	current->next = 0;
	char delimiter = 0;
	char end;
	int* counter = 0;
	_json->datalength = 0;
	if (_json->type == JSON_ARRAY)
	{
		end = ']';
		counter = &_json->datalength;
	}
	else if (_json->type == JSON_OBJECT)
	{
		end = '}';
	}
	else
	{
		return(JSON_MALFORMED);
	}

	while (delimiter != end)
	{
		if (end == ']')
		{
			current->name = 0;
		}
		fscanf(_file, "%c", &delimiter);
		switch (delimiter)
		{
			CASEJSONOBJECT
			{
				current->type = JSON_OBJECT;
				if (jsonLoadObject(current, _file))
				{
					return(JSON_MALFORMED);
				}
				if (end == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONARRAY
			{
				current->type = JSON_ARRAY;
				if (jsonLoadObject(current, _file))
				{
					return(JSON_MALFORMED);
				}
				if (end == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONSTRING
			{
				current->type = JSON_STRING;
				if(jsonLoadString(current, _file))
				{
					return(JSON_MALFORMED);
				}
				if (end == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONKEY
			{
				if (current->type == JSON_STRING)
				{
					current->name = current->value;
					current->value = 0;
				}
				else
				{
					return(JSON_MALFORMED);
				}
				break;
			}

			CASEJSONNUMBER
			{
				current->type = JSON_NUMBER;
				if (jsonLoadNumber(current, _file))
				{
					return(JSON_MALFORMED);
				}
				if (end == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONBOOLEAN
			{
				current->type = JSON_BOOLEAN;
				if (jsonLoadBoolean(current, _file))
				{
					return(JSON_MALFORMED);
				}
				if (end == ']')
				{
					(*counter)++;
				}
				break;
			}

			CASEJSONNULL
			{
				current->type = JSON_NULL;
				if (jsonLoadNull(current, _file))
				{
					return(JSON_MALFORMED);
				}
				if (end == ']')
				{
					(*counter)++;
				}
				break;
			}

			case '}':
			case ']':
			CASEJSONWHITESPACE
			{
				break;
			}

			CASEJSONNEXT
			{
				current->next = malloc(sizeof(JSONOBJECT));
				if (!current->next)
				{
					return(JSON_MEMDENIED);
				}
				current = current->next;
				current->next = 0;
				break;
			}

			default:
			{
				return(JSON_MALFORMED);
			}
		}
	}
	return(JSON_NULL);
}

LPJSONOBJECT jsonLoad(const char* _fileName)
{
	FILE* f = fopen(_fileName, "r");
	if (!f)
	{
		return(0);
	}

	char delimiter;

	LPJSONOBJECT job = malloc(sizeof(JSONOBJECT));
	job->name = 0;
	job->next = 0;
	if (!job)
	{
		fclose(f);
		return(0);
	}
	fseek(f, 0, SEEK_END);
	int end = ftell(f);
	fseek(f, 0, SEEK_SET);
	while (ftell(f) < end)
	{
		fscanf(f, "%c", &delimiter);
		
		switch (delimiter)
		{
			CASEJSONOBJECT
			{
				job->type = JSON_OBJECT;
				if (jsonLoadObject(job, f))
				{
					fclose(f);
					return(0);
				}
				break;
			}

			CASEJSONARRAY
			{
				job->type = JSON_ARRAY;
				if (jsonLoadObject(job, f))
				{
					fclose(f);
					return(0);
				}
				break;
			}

			CASEJSONSTRING
			{
				job->type = JSON_STRING;
				if (jsonLoadString(job, f))
				{
					fclose(f);
					return(0);
				}
				break;
			}

			CASEJSONNUMBER
			{
				job->type = JSON_NUMBER;
				if (jsonLoadNumber(job, f))
				{
					fclose(f);
					return(0);
				}
				break;
			}

			CASEJSONBOOLEAN
			{
				job->type = JSON_BOOLEAN;
				if (jsonLoadBoolean(job, f))
				{
					fclose(f);
					return(0);
				}
				break;
			}

			CASEJSONNULL
			{
				job->type = JSON_NULL;
				if (jsonLoadNull(job, f))
				{
					fclose(f);
					return(0);
				}
				break;
			}

			CASEJSONWHITESPACE
			{
				break;
			}

			default:
			{
				break;
			}
		}
	}
	fclose(f);
	return(job);
}

/**
* Saves a JSON formatted string, containing a string or primative
* from a JSONOBJECT, to a file
* @param _json - object encapsulating the name and value being saved
* @param _file - Pointer to the file
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonSavePrimitive(LPJSONOBJECT _json, FILE* _file)
{
	if (!_json || !_file)
	{
		return(JSON_MALFORMED);
	}

	if (_json->name)
	{
		fprintf(_file, "\"%s\":", _json->name);
	}

	switch (_json->type)
	{
		case JSON_STRING:
		{
			fprintf(_file, "\"%s\"", (char*)_json->value);
			break;
		}

		case JSON_BOOLEAN:
		{
			fprintf(_file, "%s", (*(char*)_json->value) ? "true" : "false");
			break;
		}

		case JSON_NULL:
		{
			fprintf(_file, "null");
			break;
		}

		case JSON_NUMBER:
		{
			fprintf(_file, "%lf", *(double*)_json->value);
			break;
		}

		default:
		{
			return(JSON_MALFORMED);
		}
	}
	return(JSON_NULL);
}

/**
* Saves a JSON formatted string, containing an object or array, to a file
* @param _json - object encapsulating the name and value being saved
* @param _file - Pointer to the file
* @return JSON_NULL if successful, otherwise non-zero
*/
enum JSONTYPE jsonSaveObject(LPJSONOBJECT _json, FILE* _file)
{
	if (!_json || !_file)
	{
		return(JSON_MALFORMED);
	}

	if (_json->name)
	{
		fprintf(_file, "\"%s\":", _json->name);
	}

	LPJSONOBJECT current = _json->value;
	int skip = 0;
	char start, end;

	if (_json->type == JSON_ARRAY)
	{
		start = '[';
		end = ']';
	}
	else if (_json->type == JSON_OBJECT)
	{
		start = '{';
		end = '}';
	}
	else
	{
		return(JSON_MALFORMED);
	}

	fprintf(_file, "%c", start);
	while (current)
	{
		if (skip)
		{
			fprintf(_file, ",");
		}
		skip = 1;
		switch (current->type)
		{
			case JSON_OBJECT:
			case JSON_ARRAY:
			{
				if (jsonSaveObject(current, _file))
				{
					return(JSON_MALFORMED);
				}
				break;
			}

			case JSON_STRING:
			case JSON_BOOLEAN:
			case JSON_NUMBER:
			case JSON_NULL:
			{
				if (jsonSavePrimitive(current, _file))
				{
					return(JSON_MALFORMED);
				}
				break;
			}
		}
		current = current->next;
	}
	fprintf(_file, "%c", end);
	return(JSON_NULL);
}

enum JSONTYPE jsonSave(LPJSONOBJECT _json, const char* _fileName)
{
	if (!_json)
	{
		return(JSON_MALFORMED);
	}
	FILE* f = fopen(_fileName, "w");
	if (!f)
	{
		return(JSON_FILENF);
	}
	switch (_json->type)
	{
		case JSON_OBJECT:
		case JSON_ARRAY:
		{
			if (jsonSaveObject(_json, f))
			{
				fclose(f);
				return(JSON_MALFORMED);
			}
			break;
		}

		case JSON_STRING:
		case JSON_BOOLEAN:
		case JSON_NUMBER:
		case JSON_NULL:
		{
			if (jsonSavePrimitive(_json, f))
			{
				fclose(f);
				return(JSON_MALFORMED);
			}
			break;
		}
	}
	fclose(f);
	return(JSON_NULL);
}
