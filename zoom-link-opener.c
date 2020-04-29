#include <stdio.h>
#include <string.h>

#if defined (_WIN32) || defined (_WIN64)
#define TRAY_WINAPI 1
#include "win.c"
#elif defined (__linux__) || defined (linux) || defined (__linux)
#include <unistd.h>
#define TRAY_APPINDICATOR 1
#elif defined (__APPLE__) || defined (__MACH__)
#include <unistd.h>
#define TRAY_APPKIT 1
#endif

#include "tray.h"

#if TRAY_APPINDICATOR
#define TRAY_ICON1 "indicator-messages"
#define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
#define TRAY_ICON1 "icon.png"
#define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
#define TRAY_ICON1 "icon.ico"
#define TRAY_ICON2 "icon.ico"
#endif

#include "zoom.c"

static struct tray tray;

static void toggle_cb(struct tray_menu *item) {
	printf("toggle cb\n");
	item->checked = !item->checked;
	tray_update(&tray);
}

static void hello_cb(struct tray_menu *item) {
	(void)item;
	printf("hello cb\n");
	if (strcmp(tray.icon, TRAY_ICON1) == 0) {
		tray.icon = TRAY_ICON2;
	} else {
		tray.icon = TRAY_ICON1;
	}
	tray_update(&tray);
}

static void quit_cb(struct tray_menu *item) {
	(void)item;
	printf("quit cb\n");
	tray_exit();
}

static void submenu_cb(struct tray_menu *item) {
	(void)item;
	printf("submenu: clicked on %s\n", item->text);
	tray_update(&tray);
}

// Test tray init
static struct tray tray = {
    .icon = TRAY_ICON1,
    .menu =
        (struct tray_menu[]){
            {.text = "Hello", .cb = hello_cb},
            {.text = "Checked", .checked = 1, .cb = toggle_cb},
            {.text = "Disabled", .disabled = 1},
            {.text = "-"},
            {.text = "SubMenu",
             .submenu =
                 (struct tray_menu[]){
                     {.text = "FIRST", .checked = 1, .cb = submenu_cb},
                     {.text = "SECOND",
                      .submenu =
                          (struct tray_menu[]){
                              {.text = "THIRD",
                               .submenu =
                                   (struct tray_menu[]){
                                       {.text = "7", .cb = submenu_cb},
                                       {.text = "-"},
                                       {.text = "8", .cb = submenu_cb},
                                       {.text = NULL}}},
                              {.text = "FOUR",
                               .submenu =
                                   (struct tray_menu[]){
                                       {.text = "5", .cb = submenu_cb},
                                       {.text = "6", .cb = submenu_cb},
                                       {.text = NULL}}},
                              {.text = NULL}}},
                     {.text = NULL}}},
            {.text = "-"},
            {.text = "Quit", .cb = quit_cb},
            {.text = NULL}},
};

static int run_tray()
{
	int count = 0;

	if (tray_init(&tray) < 0) {
		printf("failed to create tray\n");
		return 1;
	}
	while (tray_loop(1) == 0) {
		printf("iteration %d\n", count++);
	}

	return 0;
}

static int do_browser_open(char *url, const char *browser)
{
	printf("opening browser: '%s %s'\n", browser, url);
	FILE *log = fopen("/tmp/zoom-link-opener.txt", "a");
	fprintf(log, "do_browser_open %s %s\n", browser, url);
	fclose(log);


	#ifdef _WIN32
		ShellExecute(NULL, browser, url, NULL, NULL, SW_SHOWNORMAL);
	#else
		execlp(browser, browser, (const char *)url, NULL);
	#endif

	return 0;
}

static int do_open(char *url)
{
	printf("opening %s\n", url);

	#ifdef _WIN32
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
	#elif __APPLE__
	execlp("open", "open", (const char *)url, NULL);
	#else
	execlp("xdg-open", "xdg-open", (const char *)url, NULL);
	#endif

	return 0;
}

static bool is_http(char *protocol)
{
	return streq(protocol, "http") || streq(protocol, "https");
}

static int open_link(char *url, const char *browser)
{
	char mtg[4096];
	int ok = 0;

	struct zoom_link link;

	ok = parse_zoom_link(url, &link);

	if (!ok) {
		if (link.url.protocol && is_http(link.url.protocol))
			return do_browser_open(url, browser);
		else
			return do_open(url);
	}

	make_zoommtg(&link, mtg, sizeof(mtg));

	ok = do_open(mtg);

	free_zoom_link(&link);

	return ok;
}
