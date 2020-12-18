# DivideAFile
Divides a file into multiple files.
Could be useful if you use FAT32 file system and want to transfer files that are larger than 4GB.

## Requirements
* Requires C compiler (Tested on GCC compiler)

---

## Installation
* Compile it using the C compiler. For example: `gcc DivideAFile.c -o DivideAFile`

---

## Usage
```
 [/P | /C]

/P      Divides the input file by size (selected by default)

        /PS <Individual file Size in bytes>
        Default Partition Size: 4294967295 bytes

        /L <Filecreation Limit>
        Default File Creation Limit: 9

        /S <source_file>

        /D <output_file>

        /I      Ignores File Creation limit


/C      Concatenates the input files
        * This options should be specified first (when concatenating files)
        Usage: /C <output_filename> <input_file_1.part> <input_file_2.part> <input_file_3.part> ...

```

---

## Example Usage:

- `DivideAFile /PS 20000000 /S largefile.7z /D file` command will take largefile.7z and create multiple divied files 'file_1.part, file_2.part, ...' of 20000000 bytes of file size.

- `DivideAFile /C largefileback.7z file_1.part file_2.part` will take 'file_1.part and file_2.part' files and merge them into one single file 'largefileback.7z' (Order of those divided files is important).