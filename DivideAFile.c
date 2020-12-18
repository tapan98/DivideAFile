#include "sup.c"
/*
		-----error list-----
		1: Incomplete Source/Destination value
		2: No Source switch/flag argument's value
		3: No Destination switch/flag argument's value
		4: No PartitionSize switch/flag argument's value
		5: Invalid PartitionSize value
		6: Invalid Source File Location
		7: Invalid Destination File Location
		8: File creation limit reached
		9: File creation Error
		10: No File limit switch/flag argument's value
		11: Invalid File limit value
	*/

unsigned long long int psize = 4294967295, total_file_size;
char* source, * dest, ext[] = {".part"};
FILE* infile = NULL, * outfile = NULL;
unsigned int filelimit = 9, ignore_file_limit;

void showHelp()
{
	printf("\nUsage:\n\n [/P | /C] \n");

	printf("\n/P\tDivides the input file by size (selected by default)\n\n");
	printf("\t/PS <Individual file Size in bytes>\n\tDefault Partition Size: 4294967295 bytes\n\n");
	printf("\t/L <Filecreation Limit>\n\tDefault File Creation Limit: 9\n\n");
	printf("\t/S <source_file>\n\n");
	printf("\t/D <output_file>\n\n");
	printf("\t/I\tIgnores File Creation limit\n\n");

	printf("\n/C\tConcatenates the input files\n");
	printf("\t* This options should be specified first (when concatenating files)\n");
     printf("\tUsage: /C <output_filename> <input_file_1.part> <input_file_2.part> <input_file_3.part> ...\n");
}

int psizeValid()
{
	if (psize <= 10 || psize > 4294967295)
		return 0;
	else
		return 1;
}

int validateFile(int count, char *files[])
{
     int i, j;
     FILE* file_to_check;
     int validationfail = 0;

     for(i=0;i<count;i++)
     {
          file_to_check = fopen(files[i],"rb");
          if(file_to_check==NULL)
          {
               printf("Failed to validate file: %s\n", files[i]);
               validationfail = SOURCE_ERROR;
          }
          //calculating total file size;
          fseek(file_to_check, 0, SEEK_END);
          total_file_size += ftell(file_to_check);
          fclose(file_to_check);
     }

	// checking duplicates
	for(i = 0; i<count; i++)
     {
          for(j = i+1; j<count; j++)
               if(strcmp(files[i],files[j])==0)
               {
				validationfail = DUPLICATE_FILES_ERROR;
                    goto outofloop;
               }
     }outofloop:

     return validationfail;
}

int concatfiles(int numberOfFiles, char* files[], char* outfilename) //caller should only include list of fileenames & ArgCounts
{
     unsigned long long int bytes_count = 0;
     int char_read, i;

     FILE* infile = NULL, *outfile = fopen(outfilename, "wb");
     if(outfile==NULL)
          return DESTINATION_ERROR;

     printf("[0.00%%]\r");

     for(i = 0;i<numberOfFiles;i++)
     {
          infile = fopen(files[i], "rb");
          if(infile==NULL)
               return SOURCE_ERROR;

          char_read = fgetc(infile);
          while(char_read!=EOF)
          {
               bytes_count++;
               fputc(char_read, outfile);
               char_read = fgetc(infile);

               if(bytes_count % 5242880 == 0)
               {
                    printf("[%.2f%%]\r", (float)bytes_count/total_file_size*100);
               }
          }
          fclose(infile);
     }

     printf("[%.2f%%]\n", (float)bytes_count/total_file_size*100);

     fclose(outfile);

     return 0;

}

int initValues(int argc, char* args[])
{

	int i, returned_value;

	for (i = 1; i < argc; i++)
	{
		// '/C' option should be given first
		if ((strcmp(args[i], "/c") == 0 || strcmp(args[i], "/C") == 0) && i == 1 )
		{
			i++; //incrementing to access switch/flag values

			if (args[i] == NULL || strcmp(args[i], "/s")==0 || strcmp(args[i], "/S") == 0 || strcmp(args[i], "/d") == 0
				|| strcmp(args[i], "/D") == 0 || strcmp(args[i], "/ps") == 0 || strcmp(args[i], "/PS") == 0
				|| strcmp(args[i], "/l") == 0 || strcmp(args[i], "/L") == 0 || strcmp(args[i], "/p") == 0
				|| strcmp(args[i], "/P") == 0 || strcmp(args[i], "/c") == 0 || strcmp(args[i], "/C") == 0
				|| strcmp(args[i], "/i") == 0 || strcmp(args[i], "/I") == 0)
			{
				//source switch/flag error
				return SOURCE_OPTION_EMPTY;
			}

			returned_value = validateFile(argc-3, &args[3]);
			if(returned_value)
				return returned_value;

			//sorting file names in ascending order
			/*returned_value = sortFileNames(argc-3, &args[3]);
			if(returned_value)
			{
				return SOURCE_ERROR;
			}
			*/

			returned_value = concatfiles(argc-3, &args[3], args[2]);

			if(returned_value)
				return returned_value;
			else
				printf("Files Concatenated\n");

			exit(0);
		}else

		if(strcmp(args[i], "/p") == 0 || strcmp(args[i], "/P") == 0)
			i++;

		if (strcmp(args[i],"/s")==0 || strcmp(args[i], "/S") == 0)
		{
			i++; //incrementing to access switch/flag values

			if (args[i] == NULL || strcmp(args[i], "/s")==0 || strcmp(args[i], "/S") == 0 || strcmp(args[i], "/d") == 0
				|| strcmp(args[i], "/D") == 0 || strcmp(args[i], "/ps") == 0 || strcmp(args[i], "/PS") == 0
				|| strcmp(args[i], "/l") == 0 || strcmp(args[i], "/L") == 0 || strcmp(args[i], "/p") == 0
				|| strcmp(args[i], "/P") == 0 || strcmp(args[i], "/c") == 0 || strcmp(args[i], "/C") == 0
				|| strcmp(args[i], "/i") == 0 || strcmp(args[i], "/I") == 0)
			{
				//source switch/flag error
				return SOURCE_OPTION_EMPTY;
			}

			//updating source------------------------
			source = (char*)malloc(strlen(args[i]) + 1);
			strcpy(source, args[i]);
			infile = fopen(source, "rb");
			if(infile == NULL) return SOURCE_ERROR;

		}

		if (strcmp(args[i], "/d") == 0 || strcmp(args[i], "/D") == 0)
		{
			i++;
			if (args[i] == NULL || strcmp(args[i], "/s")==0 || strcmp(args[i], "/S") == 0 || strcmp(args[i], "/d") == 0
				|| strcmp(args[i], "/D") == 0 || strcmp(args[i], "/ps") == 0 || strcmp(args[i], "/PS") == 0
				|| strcmp(args[i], "/l") == 0 || strcmp(args[i], "/L") == 0 || strcmp(args[i], "/p") == 0
				|| strcmp(args[i], "/P") == 0 || strcmp(args[i], "/c") == 0 || strcmp(args[i], "/C") == 0
				|| strcmp(args[i], "/i") == 0 || strcmp(args[i], "/I") == 0)
			{
				//destination switch/flag error
				return DESTINATION_OPTION_EMPTY;
			}
			//updaing destination-----------------------
			dest = (char*)malloc(strlen(args[i]) + 1);
			strcpy(dest, args[i]);
		}

		if (strcmp(args[i], "/ps") == 0 || strcmp(args[i], "/PS") == 0)
		{
			i++;

			if (args[i] == NULL || strcmp(args[i], "/s")==0 || strcmp(args[i], "/S") == 0 || strcmp(args[i], "/d") == 0
				|| strcmp(args[i], "/D") == 0 || strcmp(args[i], "/ps") == 0 || strcmp(args[i], "/PS") == 0
				|| strcmp(args[i], "/l") == 0 || strcmp(args[i], "/L") == 0 || strcmp(args[i], "/p") == 0
				|| strcmp(args[i], "/P") == 0 || strcmp(args[i], "/c") == 0 || strcmp(args[i], "/C") == 0
				|| strcmp(args[i], "/i") == 0 || strcmp(args[i], "/I") == 0)
				{
				//parititonSize switch/flag error
				return PARTITION_OPTION_EMPTY;
			}

			psize = stringToInt(args[i]);

			if (!psizeValid())
			{
				//parititonSize invalid error
				return PARTITION_SIZE_ERROR;
			}
		}

		if (strcmp(args[i], "/l") == 0 || strcmp(args[i], "/L") == 0)
		{
			i++;

			if (args[i] == NULL || strcmp(args[i], "/s")==0 || strcmp(args[i], "/S") == 0 || strcmp(args[i], "/d") == 0
				|| strcmp(args[i], "/D") == 0 || strcmp(args[i], "/ps") == 0 || strcmp(args[i], "/PS") == 0
				|| strcmp(args[i], "/l") == 0 || strcmp(args[i], "/L") == 0 || strcmp(args[i], "/p") == 0
				|| strcmp(args[i], "/P") == 0 || strcmp(args[i], "/c") == 0 || strcmp(args[i], "/C") == 0
				|| strcmp(args[i], "/i") == 0 || strcmp(args[i], "/I") == 0)
			{
				//file limit switch/flag error
				return FILE_LIMIT_OPTION_EMPTY;
			}

			if (*args[i] == '0')

				//file limit value error
				return FILE_LIMIT_ZERO;
			else
			{
				filelimit = stringToInt(args[i]);
			}
		}

		if(strcmp(args[i], "/i") == 0 || strcmp(args[i], "/I") == 0)
		{
			i++;
			ignore_file_limit++;
		}
	}


	if (source == 0 || dest == 0)
	{
		//Incomplete source/destination value(s)
		return SOURCE_OR_DEST_ERROR;
	}
	else
		//arguments assignment successful
		return 0;
}

int Divide()
{
	char *destModified, *temp;
	unsigned long long int byte_count = 0, total_bytes_count = 0, source_file_size = 0;
	int char_read, filescount = 1;
	if (infile == NULL)
	{
		infile = fopen(source, "rb");
		if(infile == NULL) return SOURCE_ERROR; //Invalid Source File Location
	}
	else
	{
		//reading source file size
		fseek(infile, 0, SEEK_END);
		source_file_size = ftell(infile);
		rewind(infile);

		//calculating number of files required
		if(((float)source_file_size/psize)>filelimit && ignore_file_limit == 0)
		{
			printf("\n * Warning: Number of files exceeding the file creation limit: %u\n", filelimit);
			if(((float)source_file_size/psize)>source_file_size/psize)
				printf("   Required number of files: %llu\n", source_file_size/psize+1);
			else
				printf("   Required number of files: %llu\n", source_file_size/psize);

			printf("\n * Use /L to specify number of files \n\n");

		}

		temp = intToString(filescount); // temp = number in string
		destModified = concatStr(dest, "_", temp); // destModified = destination+number in string e.g. file1
		free(temp); // temp is empty
		temp = destModified; // temp is destination+number in string e.g. file1
		destModified = concatStr(destModified, ext, 0); //destModified = file1.part
		free(temp); // temp is empty
		printf("Creating a file (%s)...\n", destModified);
		outfile = fopen(destModified, "wb");
	}
	if (outfile == NULL)
	{
		return FILE_CREATION_ERROR;
	}

	printf("[0.00%%]\r");

	char_read = fgetc(infile);
	while (char_read != EOF)
	{
		byte_count++;
		total_bytes_count++;

		if (byte_count == psize)
		{
			fclose(outfile);
			filescount++;
			free(destModified);

			temp = intToString(filescount);
			destModified = concatStr(dest, "_", temp);
			free(temp);
			temp = destModified;
			destModified = concatStr(destModified, ext, 0);
			free(temp);

			printf("\rPartition Size limit reached.\n");
			printf("[%.2f%%]\r", (float)total_bytes_count/source_file_size*100);
			printf("Creating a new file (%s)...\n", destModified);

			outfile = fopen(destModified, "wb");

			if (filescount >= filelimit && ignore_file_limit==0) //restriction of file creation limit
			{
				printf("[%.2f%%]\r", (float)total_bytes_count/source_file_size*100);
				return FILE_COUNT_LIMIT_ERROR;
			}
			else if(outfile == NULL)
			{
				return FILE_CREATION_ERROR;
			}
			else
			{
				printf("[%.2f%%]\r", (float)total_bytes_count / source_file_size * 100);
			}

			byte_count = 0;

		}

		fputc(char_read, outfile);
		char_read = fgetc(infile);


		if (total_bytes_count % 5242880 == 0){ printf("[%.2f%%]\r", (float)total_bytes_count/source_file_size*100);}
	}

	printf("[%.2f%%]\n", (float)total_bytes_count / source_file_size * 100);

	fclose(outfile);
	fclose(infile);
	return 0;
}

int main(int argc, char* argv[])
{
	int ReturnValue = 1;
	if (argc == 1)
	{
		printf("\nNo arguments found.\n");
		showHelp();
	}
	else if(strcmp(argv[1],"/?")==0)
		showHelp();
	else
	{
		ReturnValue = initValues(argc, argv);
		switch (ReturnValue)
		{
			case 0: printf("Values Initialized.\n"); break;
			case SOURCE_OR_DEST_ERROR: printf("Incomplete Source/Destination value"); break;
			case SOURCE_OPTION_EMPTY: printf("No Source option argument's value"); break;
			case DESTINATION_OPTION_EMPTY: printf("No Destination option argument's value"); break;
			case PARTITION_OPTION_EMPTY: printf("No PartitionSize option argument's value"); break;
			case PARTITION_SIZE_ERROR: printf("Invalid PartitionSize value"); break;
			case SOURCE_ERROR: printf("Invalid Source File Location"); break;
			case DESTINATION_ERROR: printf("Invalid Destination File Location"); break;
			case FILE_LIMIT_OPTION_EMPTY: printf("No File limit option's value"); break;
			case FILE_LIMIT_ZERO: printf("File Limit cannot be zero"); break;
			case DUPLICATE_FILES_ERROR: printf("Duplicate files are not allowed"); break;

			default:
				printf("Unknown Error: %d", ReturnValue); break;
		}
		printf("\n");
	}

	if (ReturnValue == 0)
	{
		printf("Final Values\n----------------------\n");
		printf("Source: %s\n", source);
		printf("Destination: %s\n", dest);
		printf("Partition Size: %llu\n----------------------\n", psize);

		ReturnValue = Divide();
		switch (ReturnValue) {
			case 0: printf("Partitioning Done.\n"); break;
			case SOURCE_ERROR: printf("Invalid Source File Location\n"); break;
			case FILE_COUNT_LIMIT_ERROR: printf("\nCould not create more file partitions.\n"); break;
			case FILE_CREATION_ERROR: printf("\nError Creating a new file.\n"); break;
			default: printf("Unknown Error: %d\n", ReturnValue);
		}
	}


	return 0;
}
