#include <windows.h>
#include <iostream>
#include <string>
static char buffer[10 * 1024];

int main() {
	FreeConsole();
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
		AllocConsole();
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	DWORD mode{};
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode);
	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	SetConsoleTitleA("ConsoleHost");
	HANDLE pipe = CreateNamedPipeA("\\\\.\\pipe\\consolePipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,
		sizeof(buffer),
		sizeof(buffer),
		0,
		nullptr);
	if (pipe == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create named pipe. Error: " << GetLastError() << std::endl;
		return 1;
	}
	std::cout << "Waiting for connection..." << std::endl;
	ConnectNamedPipe(pipe, nullptr);
	DWORD bytes_read;
	while (true) {
		if (ReadFile(pipe, buffer, sizeof(buffer), &bytes_read, nullptr)) {
			buffer[bytes_read] = '\0';
			std::cout << buffer;
		}
		else {
			break;
		}
	}
	CloseHandle(pipe);
	return 0;
}