
#include "zoom-link-opener.c"

int main(int argc, char *argv[])
{
	// TODO: browser
	const char *browser = "/home/jb55/bin/browser";
	struct tray tray;

#ifdef _WIN32
	launch_default_apps_dialog();
#endif

	if (argc == 2) {
		open_link(argv[1], browser);
		return 0;
	}

	return 1;

	//setup_tray(&tray);
	//return run_tray(&tray);
}
