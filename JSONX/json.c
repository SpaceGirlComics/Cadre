#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "json.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef UNICODE
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
#else
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
#endif

int jsonObject(char* json, LPJSONOBJECT _jo)
{
	_jo->type = JSON_OBJECT;
	_jo->value = malloc(sizeof(JSONOBJECT));
	_jo->next = 0;
	_jo = _jo->value;
	int x;
	for (x = 0; json[x] != '}'; x++)
	{
		switch (json[x])
		{
			CASEJSONOBJECT
			{
				x += jsonObject(&json[x+1], _jo);
				break;
			}

			CASEJSONARRAY
			{
				x += jsonArray(&json[x + 1], _jo);
				break;
			}

			CASEJSONSTRING
			{
				x += jsonString(&json[x+1], _jo);
				break;
			}

			CASEJSONNUMBER
			{
				x += jsonNumber(&json[x], _jo);
				break;
			}

			CASEJSONBOOLEAN
			{
				x += jsonBool(&json[x], _jo);
				break;
			}

			CASEJSONNULL
			{
				x += jsonNull(&json[x], _jo);
				break;
			}

			CASEJSONKEY
			{
				if (_jo->type == JSON_STRING)
				{
					_jo->name = _jo->value;
					_jo->value = 0;
				}
				else
				{
					_jo->type = JSON_MALFORMED;
				}
				break;
			}

			CASEJSONWHITESPACE
			{
				break;
			}

			case ',':
			{
				//_jo->type = JSON_OBJECT;
				_jo->next = malloc(sizeof(JSONOBJECT));
				_jo = _jo->next;
				_jo->next = 0;
				break;
			}

			default:
			{
				_jo->type = JSON_MALFORMED;
				break;
			}
		}
	}
	return(x+1);
}

int jsonArray(char* json, LPJSONOBJECT _jo)
{
	_jo->type = JSON_ARRAY;
	_jo->value = malloc(sizeof(JSONOBJECT));
	_jo->next = 0;
	_jo = _jo->value;
	int x;// , ob = 0;
	//_jo->name = malloc(sizeof(int));
	//((int*)_jo->name) = ob;
	for (x = 0; json[x] != ']'; x++)
	{
		switch (json[x])
		{
			CASEJSONOBJECT
			{
				x += jsonObject(&json[x + 1], _jo);
				break;
			}

			CASEJSONARRAY
			{
				x += jsonArray(&json[x + 1], _jo);
				break;
			}

			CASEJSONSTRING
			{
				x += jsonString(&json[x + 1], _jo);
				break;
			}

			CASEJSONNUMBER
			{
				x += jsonNumber(&json[x], _jo);
				break;
			}

			CASEJSONBOOLEAN
			{
				x += jsonBool(&json[x], _jo);
				break;
			}

			CASEJSONNULL
			{
				x += jsonNull(&json[x], _jo);
				break;
			}

			CASEJSONWHITESPACE
			{
				break;
			}

			case ',':
			{
				//_jo->type = JSON_OBJECT;
				_jo->next = malloc(sizeof(JSONOBJECT));
				_jo = _jo->next;
				_jo->next = 0;
				break;
			}

			default:
			{
				_jo->type = JSON_MALFORMED;
				break;
			}
		}
	}
	return(x + 1);
}

int jsonString(char* json, LPJSONOBJECT _jo)
{
	_jo->type = JSON_STRING;
	int size = 0;
	while (json[size] != '\"')
	{
		if (json[size] == '\\')
		{
			size++;
			if (json[size] == 'u')
			{
				size += 4;
			}
		}
		size++;
	}
	_jo->datalength = size+1;
	_jo->value = malloc(_jo->datalength);
	memcpy(_jo->value, json, size);
	((char*)_jo->value)[size] = '\0';
	return(++size);
}

int jsonNumber(char* json, LPJSONOBJECT _jo)
{
	_jo->type = JSON_NUMBER;
	_jo->datalength = sizeof(double);
	_jo->value = malloc(sizeof(double));
	sscanf(json, "%lf", (double*)_jo->value);
	int count = 0;
	while (json[count] != ',' && json[count] != '}' && json[count] != ']')
	{
		count++;
	}
	return(count-1);
}

int jsonBool(char* json, LPJSONOBJECT _jo)
{
	_jo->type = JSON_BOOLEAN;
	_jo->datalength = sizeof(unsigned char);
	_jo->value = malloc(sizeof(unsigned char));
	int count = 0;
	if (json[0] == 't' && json[1] == 'r' && json[2] == 'u' && json[3] == 'e')
	{
		*(char*)_jo->value = 1;
		count += 3;
	}
	else if (json[0] == 'f' && json[1] == 'a' && json[2] == 'l' && json[3] == 's' && json[4] == 'e')
	{
		*(char*)_jo->value = 0;
		count += 4;
	}
	else
	{
		free(_jo->value);
		_jo->datalength = 0;
		_jo->type = JSON_MALFORMED;
	}
	return(count);
}

int jsonNull(char* json, LPJSONOBJECT _jo)
{
	int count = 0;
	if (json[0] == 'n' && json[1] == 'u' && json[2] == 'l' && json[3] == 'l')
	{
		_jo->type = JSON_NULL;
		_jo->datalength = 0;
		_jo->value = 0;
		count += 3;
	}
	else
	{
		_jo->type = JSON_MALFORMED;
	}
	return(count);
}

LPJSONOBJECT jsonParse(char* json)
{
	LPJSONOBJECT root		= malloc(sizeof(JSONOBJECT));
	LPJSONOBJECT current	= root;
	current->datalength		= 0;
	current->name			= 0;
	current->next			= 0;
	current->type			= JSON_MALFORMED;
	current->value			= 0;

	for (int x = 0; x < strlen(json); x++)
	{
		switch (json[x])
		{
			CASEJSONOBJECT
			{
				x = jsonObject(&json[x+1], current);
				break;
			}

			CASEJSONARRAY
			{
				x = jsonArray(&json[x + 1], current);
				break;
			}

			CASEJSONSTRING
			{
				x = jsonString(&json[x + 1], current);
				break;
			}

			CASEJSONNUMBER
			{
				x = jsonNumber(&json[x], current);
				break;
			}

			CASEJSONBOOLEAN
			{
				x = jsonBool(&json[x + 1], current);
				break;
			}

			CASEJSONNULL
			{
				x = jsonNull(&json[x + 1], current);
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

enum JSONTYPE jsonGet(LPJSONOBJECT source, void* destination)
{
	return(JSON_NULL);
}

enum JSONTYPE jsonPathGet(LPJSONOBJECT source, void* destination, char* path)
{
	return(JSON_NULL);
}

int jsonEncode(LPJSONOBJECT json, char* output)
{
	return(JSON_NULL);
}

int jsonDestroyNumber(LPJSONOBJECT num)
{
	free(num->name);
	free((double*)num->value);
	free(num);
	return(0);
}

int jsonDestroyString(LPJSONOBJECT str)
{
	free(str->name);
	free((char*)str->value);
	free(str);
	return(0);
}

int jsonDestroyNull(LPJSONOBJECT nul)
{
	free(nul->name);
	free(nul);
	return(0);
}

int jsonDestroyBool(LPJSONOBJECT bol)
{
	free(bol->name);
	free(bol);
	return(0);
}

int jsonDestroyArray(LPJSONOBJECT arr)
{
	LPJSONOBJECT current = arr->value;
	LPJSONOBJECT next;
	while (current)
	{
		next = current->next;
		switch (current->type)
		{
			case JSON_STRING:
			{
				jsonDestroyString(current);
				break;
			}

			case JSON_NUMBER:
			{
				jsonDestroyNumber(current);
				break;
			}

			case JSON_OBJECT:
			{
				break;
			}

			case JSON_ARRAY:
			{
				jsonDestroyArray(current);
				break;
			}

			case JSON_BOOLEAN:
			{
				jsonDestroyBool(current);
				break;
			}

			case JSON_NULL:
			{
				jsonDestroyNull(current);
				break;
			}

			case JSON_MALFORMED:
			{
				break;
			}
		}
		//free(current);
		current = next;
	}
	free(arr->name);
	free(arr);
	return(0);
}

int jsonDestroyObject()
{

}

enum JSONTYPE jsonDestroy(LPJSONOBJECT json)
{
	return(JSON_NULL);
}

