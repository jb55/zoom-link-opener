
#include "zoom-link-opener.c"

int main(int argc, char *argv[])
{
	const char *browser = "/home/jb55/bin/browser";

#ifdef _WIN32
	launch_default_apps_dialog();
#endif

	if (argc == 2) {
		open_link(argv[1], browser);
		return 0;
	}

	return run_tray();
}