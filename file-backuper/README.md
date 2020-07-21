# file-backuper

Task configuration is being performed in the file *tasks.json* in the folder with the program.

Syntax:

	[
		{
			"name" : "Documents",
			"from" : "C:\docs",
			"to"   : "D:\backup\docs",
			"options" : [
				"", ""
			],
			"excludePaths" : [
				"finance", "confidential"
			]
		},
		{
			"name" : "Photo",
			"from" : "C:\photo",
			"to"   : "D:\backup\photo",
			"options" : [
				"", ""
			]
		}
	]

- *name* - name of the backup task
- *from* - source folder
- *to* - destination folder
- *options*:
    - 1
    - 2
- *excludePaths* - list of *relative* paths to be *excluded*
