#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum error_list
{
	SOURCE_OR_DEST_ERROR = 1,
	SOURCE_OPTION_EMPTY,
	DESTINATION_OPTION_EMPTY,
	PARTITION_OPTION_EMPTY,
	PARTITION_SIZE_ERROR,
	SOURCE_ERROR,
	DESTINATION_ERROR,
	FILE_COUNT_LIMIT_ERROR,
	FILE_CREATION_ERROR,
	FILE_LIMIT_OPTION_EMPTY,
	FILE_LIMIT_ZERO,
	FILE_VALID_ERROR,
	FILE_NAME_ERROR,
	DUPLICATE_FILES_ERROR,
};

//#define SOURCE_OR_DEST_ERROR 1
//#define SOURCE_OPTION_EMPTY 2
//#define DESTINATION_OPTION_EMPTY 3
//#define PARTITION_OPTION_EMPTY 4
//#define PARTITION_SIZE_ERROR 5
//#define SOURCE_ERROR 6
//#define DESTINATION_ERROR 7
//#define FILE_COUNT_LIMIT_ERROR 8
//#define FILE_CREATION_ERROR 9
//#define FILE_LIMIT_OPTION_EMPTY 10
//#define FILE_LIMIT_ZERO 11
//#define FILE_VALID_ERROR 12
//#define FILE_NAME_ERROR 13
//#define DUPLICATE_FILES_ERROR 14

char* concatStr(char* first_str, char* second_str, char* third_str)
{
	int i, j;
     char* concatstring;

     if(first_str==0||second_str==0)
     {
          printf("empty string\n");
          return 0;
     }

     if(third_str==0)
     {
          concatstring = (char*)malloc(strlen(first_str) + strlen(second_str) + 1);
          strcpy(concatstring, first_str);
     	strcat(concatstring, second_str);
     }
     else
     {
          concatstring = (char*)malloc(strlen(first_str) + strlen(second_str) + strlen(third_str) + 1);
          strcpy(concatstring, first_str);
          strcat(concatstring, second_str);
          strcat(concatstring, third_str);
     }

	return concatstring;
}

unsigned long long int stringToInt(char str[])
{
	long long int converted=0;
	int i = 0;
	while (str[i] != '\0')
	{

		switch (str[i])
		{
			case '0': converted *= 10; converted += 0; break;
			case '1': converted *= 10; converted += 1; break;
			case '2': converted *= 10; converted += 2; break;
			case '3': converted *= 10; converted += 3; break;
			case '4': converted *= 10; converted += 4; break;
			case '5': converted *= 10; converted += 5; break;
			case '6': converted *= 10; converted += 6; break;
			case '7': converted *= 10; converted += 7; break;
			case '8': converted *= 10; converted += 8; break;
			case '9': converted *= 10; converted += 9; break;

			default:
				return 0;
		}
		i++;
	}
	return converted;
}

char* intToString(int value)
{
	int i, size = 1, number_to_convert;
	char* stringToReturn;

	//calculating size for the string
	for (i = 1; (value / i) != 0; i *= 10)
		size++;

	stringToReturn = (char*)malloc(size);

	if (stringToReturn != NULL)
	{
		stringToReturn[0] = '\0';
		for (i = size - 2; i >= 0; i--)
		{
			number_to_convert = value % 10;
			value /= 10;
			switch (number_to_convert)
			{
				case 0: stringToReturn[i] = '0'; break;
				case 1: stringToReturn[i] = '1'; break;
				case 2: stringToReturn[i] = '2'; break;
				case 3: stringToReturn[i] = '3'; break;
				case 4: stringToReturn[i] = '4'; break;
				case 5: stringToReturn[i] = '5'; break;
				case 6: stringToReturn[i] = '6'; break;
				case 7: stringToReturn[i] = '7'; break;
				case 8: stringToReturn[i] = '8'; break;
				case 9: stringToReturn[i] = '9'; break;
				default: free(stringToReturn); return NULL;
			}
		}
		stringToReturn[size - 1] = '\0';
	}

	return stringToReturn;
}

int charToInt(char number)
{
     switch(number)
     {
          case '0': return 0; case '1': return 1;
          case '2': return 2; case '3': return 3;
          case '4': return 4; case '5': return 5;
          case '6': return 6; case '7': return 7;
          case '8': return 8; case '9': return 9;
     }

     return -1;
}

int sortFileNames(int filescount, char* files[])
{
     int* fileslist_int, i, j, inttemp;
     fileslist_int = (int*)malloc(sizeof(int)*(filescount)); //excluding executable filename
     char* strtemp = (char*)malloc(strlen(files[0])+3);


     for(i=0;i<filescount;i++)
     {
          fileslist_int[i] = charToInt(files[i][strlen(files[i])-6]);
          //extracts file part numbers e.g. 2 from something_2.part
     }

     //validating file numbers
     for(i=0;i<filescount;i++)
     {
          if(fileslist_int[i]==-1)
               return FILE_VALID_ERROR;
     }

     //sorting using Bubble Sort
     for(i=0;i<filescount-1;i++)
     {
          for(j=0;j<filescount-i-1;j++)
          {
               if(fileslist_int[j]>fileslist_int[j+1])
               {
                    inttemp = fileslist_int[j]; strcpy(strtemp, files[j]);
                    fileslist_int[j] = fileslist_int[j+1]; strcpy(files[j], files[j+1]);
                    fileslist_int[j+1] = inttemp; strcpy(files[j+1], strtemp);
               }
          }
     }
	return 0;
}
