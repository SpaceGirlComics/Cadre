#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "json.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

LPJSONOBJECT jsonArray(int count, char* json)
{
	// LPJSONOBJECT job = new JSONOBJECT;														//	c++
	LPJSONOBJECT job = (LPJSONOBJECT)malloc(sizeof(JSONOBJECT));								// C
	LPJSONOBJECT ret = job;
	job->datalength = 0;
	job->name = (char*)count;
	job->type = JSON_NULL;
	job->value = (void*)0;
	int hold;

	for (unsigned int x = 0; x <= strlen(json); x++)
	{
		switch (json[x])
		{
		case '\"':
		{
			x++;
			job->type = JSON_STRING;
			int y = 0;
			while (json[x + y] != '\"')
			{
				if (json[x + y] == '\\')
				{
					y++;
					if (json[x + y] == 'u')
					{
						y += 4;
					}
				}
				y++;
			}
			//job->value = new char[y + 1];														//	c++
			job->value = (void*)malloc(y + 1);													// C
			memcpy((char*)job->value, &json[x], y);
			((char*)job->value)[y] = '\0';
			x += y;
			job->datalength = y + 1;
			break;
		}

		case '{':
		{
			job->type = JSON_OBJECT;
			job->value = jsonParse(&json[++x]);
			break;
		}

		case '[':
		{
			job->type = JSON_ARRAY;
			job->value = jsonArray(0, json);
			break;
		}

		case 't':
		{
			job->type = JSON_BOOLEAN;
			if (json[++x] == 'r' && json[++x] == 'u' && json[++x] == 'e')
			{
				//job->value = new bool;														//	c++
				//*((bool*)job->value) = true;													//	c++
				job->value = (void*)malloc(sizeof(unsigned char));								//	C
				*((unsigned char*)job->value) = 1;
				job->datalength = sizeof(unsigned char);
			}
			else
			{
				job->type = JSON_MALFORMED;
				return((LPJSONOBJECT)JSON_NULL);
			}
			break;
		}

		case 'f':
		{
			job->type = JSON_BOOLEAN;
			if (json[++x] == 'a' && json[++x] == 'l' && json[++x] == 's' && json[++x] == 'e')
			{
				//job->value = new bool;														//	c++
				//*((bool*)job->value) = false;													//	c++
				job->value = (void*)malloc(sizeof(unsigned char));								//	C
				*((unsigned char*)job->value) = 0;
				job->datalength = sizeof(unsigned char);
			}
			else
			{
				job->type = JSON_MALFORMED;
				return((LPJSONOBJECT)JSON_NULL);
			}
			break;
		}

		case 'n':
		{
			job->type = JSON_NULL;
			if (json[++x] == 'u' && json[++x] == 'l' && json[++x] == 'l')
			{
				job->value = (void*)0;
			}
			else
			{
				job->type = JSON_MALFORMED;
				return((LPJSONOBJECT)JSON_NULL);
			}
			break;
		}

		case '}':
		case ':':
		case ' ':
		case '\n':
		case '\r':
		case '\t':
		{
			//x++;
			break;
		}

		case ',':
		{
			// job->next = new JSONOBJECT;														//	c++
			job->next = (LPJSONOBJECT)malloc(sizeof(JSONOBJECT));								// C
			job = job->next;
			job->name = (char*)++count;
			break;
		}

		case ']':
		{
			job->datalength = count;
			return(job);
			break;
		}

		default:
		{
			if ((json[x] >= 45 && json[x] <= 57) && json[x] != 47)
			{
				job->type = JSON_NUMBER;
				//job->value = new double;													// c++
				job->value = (void*)malloc(sizeof(double));									// C
				hold = sscanf(&json[x], "%lf", (double*)job->value);
				job->datalength = sizeof(double);
				while ((json[x] >= 45 && json[x] <= 57) && json[x] != 47)
				{
					x++;
				}
				x--;
			}
			break;
		}
		}
	}

	//delete(job);																				// c++
	free(job);																					// C
	return((LPJSONOBJECT)JSON_NULL);
}

LPJSONOBJECT jsonParse(char* json)
{
	// LPJSONOBJECT job = new JSONOBJECT;														//	c++
	LPJSONOBJECT job = (LPJSONOBJECT)malloc(sizeof(JSONOBJECT));								// C
	LPJSONOBJECT ret = job;
	job->datalength = 0;
	job->name = (char*)0;
	job->type = JSON_NULL;
	job->value = (void*)0;
	int hold;

	for (unsigned int x = 0; x <= strlen(json); x++)
	{
		switch (json[x])
		{
		case '\"':
		{
			x++;
			job->type = JSON_STRING;
			int y = 0;
			while (json[x + y] != '\"')
			{
				if (json[x + y] == '\\')
				{
					y++;
					if (json[x + y] == 'u')
					{
						y += 4;
					}
				}
				y++;
			}
			//job->value = new char[y + 1];														//	c++
			job->value = (void*)malloc(y + 1);													// C
			memcpy((char*)job->value, &json[x], y);
			((char*)job->value)[y] = '\0';
			x += y;
			job->datalength = y + 1;
			break;
		}
		case '{':
		{
			job->type = JSON_OBJECT;
			job->value = jsonParse(&json[++x]);
			for (int y = 1; y > 0; y)
			{
				if (json[x] == '{')
				{
					y++;
				}
				if (json[x++] == '}')
				{
					y--;
				}
			}
			x--;
			break;
		}
		case '[':
		{
			job->type = JSON_ARRAY;
			job->value = jsonArray(0, &json[++x]);
			break;
		}
		case 't':
		{
			job->type = JSON_BOOLEAN;
			if (json[++x] == 'r' && json[++x] == 'u' && json[++x] == 'e')
			{
				//job->value = new bool;														//	c++
				//*((bool*)job->value) = true;													//	c++
				job->value = (void*)malloc(sizeof(unsigned char));								//	C
				*((unsigned char*)job->value) = 1;
				job->datalength = sizeof(unsigned char);
			}
			else
			{
				job->type = JSON_MALFORMED;
				return((LPJSONOBJECT)JSON_NULL);
			}
			break;
		}
		case 'f':
		{
			job->type = JSON_BOOLEAN;
			if (json[++x] == 'a' && json[++x] == 'l' && json[++x] == 's' && json[++x] == 'e')
			{
				//job->value = new bool;														//	c++
				//*((bool*)job->value) = false;													//	c++
				job->value = (void*)malloc(sizeof(unsigned char));								//	C
				*((unsigned char*)job->value) = 0;
				job->datalength = sizeof(unsigned char);
			}
			else
			{
				job->type = JSON_MALFORMED;
				return((LPJSONOBJECT)JSON_NULL);
			}
			break;
		}
		case 'n':
		{
			job->type = JSON_NULL;
			if (json[++x] == 'u' && json[++x] == 'l' && json[++x] == 'l')
			{
				job->value = (void*)0;
			}
			else
			{
				job->type = JSON_MALFORMED;
				return((LPJSONOBJECT)JSON_NULL);
			}
			break;
		}
		case ':':
		{
			job->name = (char*)job->value;
			job->value = (void*)0;
			job->type = JSON_NULL;
			job->datalength = 0;
			break;
		}
		case ' ':
		case '\n':
		case '\r':
		case '\t':
		{
			//x++;
			break;
		}

		case ',':
		{
			// job->next = new JSONOBJECT;														//	c++
			job->next = (LPJSONOBJECT)malloc(sizeof(JSONOBJECT));								// C
			job = job->next;
			break;
		}

		case '}':
		{
			job->next = NULL;
			return(ret);
			break;
		}
		default:
		{
			if ((json[x] >= 45 && json[x] <= 57) && json[x] != 47)
			{
				job->type = JSON_NUMBER;
				//job->value = new double;														// c++
				job->value = (void*)malloc(sizeof(double));										// C
				hold = sscanf(&json[x], "%lf", (double*)job->value);
				job->datalength = sizeof(double);
				while ((json[x] >= 45 && json[x] <= 57) && json[x] != 47)
				{
					x++;
				}
				x--;
			}
			break;
		}
		}
	}

	//delete(job);																					// c++
	free(job);																						// C
	return((LPJSONOBJECT)JSON_NULL);
}

enum JSONTYPE jsonGet(LPJSONOBJECT source, void* destination)
{
	return(JSON_NULL);
}

enum JSONTYPE jsonPathGet(LPJSONOBJECT source, void* destination, char* path)
{
	return(JSON_NULL);
}

enum JSONTYPE jsonEncode(LPJSONOBJECT json, char* output)
{
	return(JSON_NULL);
}

enum JSONTYPE jsonDestroy(LPJSONOBJECT json)
{
	return(JSON_NULL);
}

