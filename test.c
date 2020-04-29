
#include "zoom.c"
#include <assert.h>

struct test_case {
	char *url;
	char *expected_meeting_id;  //expected meeting id
	char *expected_password;  //expected password
	char *expected_hostname;  //expected password
	char *expected_mtg;  //expected password
	bool expected_valid;  //expected password
};

static struct test_case test_cases[] = {
	{ .url = "https://us04web.zoom.us/j/1234512345?pwd=derp",
	  .expected_meeting_id = "1234512345",
	  .expected_password = "derp",
	  .expected_hostname = "us04web.zoom.us",
	  .expected_mtg = "zoommtg://us04web.zoom.us/start?confno=1234512345&pwd=derp",
	  .expected_valid = true,
	},

	{ .url = "https://zoom.us/j/12345125",
	  .expected_meeting_id = "12345125",
	  .expected_password = "",
	  .expected_hostname = "zoom.us",
	  .expected_mtg = "zoommtg://zoom.us/start?confno=12345125",
	  .expected_valid = true,
	},

	{ .url = "derp.zoom.us/j/12345125",
	  .expected_meeting_id = "12345125",
	  .expected_password = "",
	  .expected_hostname = "derp.zoom.us",
	  .expected_mtg = "zoommtg://derp.zoom.us/start?confno=12345125",
	  .expected_valid = true,
	},

	{ .url = "derpy.zoom.us/j/12345125?pwd=hi",
	  .expected_meeting_id = "12345125",
	  .expected_password = "hi",
	  .expected_hostname = "derpy.zoom.us",
	  .expected_mtg = "zoommtg://derpy.zoom.us/start?confno=12345125&pwd=hi",
	  .expected_valid = true,
	},

	{ .url = "zoommtg://derpy.zoom.us/start?confid=12345125&pwd=hi",
	  .expected_meeting_id = "",
	  .expected_password = "hi",
	  .expected_hostname = "derpy.zoom.us",
	  .expected_mtg = "",
	  .expected_valid = false,
	},

	{ .url = "h",
	  .expected_meeting_id = "",
	  .expected_password = "",
	  .expected_hostname = "h",
	  .expected_mtg = "",
	  .expected_valid = false,
	},

	{ .url = "",
	  .expected_meeting_id = "",
	  .expected_password = "",
	  .expected_hostname = "",
	  .expected_mtg = "",
	  .expected_valid = false,
	},
};

static int test_parse()
{
	int failed = 0;
	char buffer[256];
	struct zoom_link link;

	for (size_t i = 0; i < sizeof(test_cases) / sizeof(struct test_case); i++) {
		struct test_case *test_case = &test_cases[i];
		init_zoom_link(&link);
		parse_zoom_link(test_case->url, &link);

		bool valid = validate_zoom_link(&link);
		if (valid != test_case->expected_valid) {
			printf("FAIL expected %s zoom link for '%s', got %s\n",
			       test_case->expected_valid?"valid":"invalid",
			       test_case->url,
			       test_case->expected_valid?"invalid":"valid");
		}

		if (valid) {
			if (!streq(link.meeting_id, test_case->expected_meeting_id)) {
				printf("FAIL expected meeting_id '%s', got '%s'\n",
				       test_case->expected_meeting_id, link.meeting_id);
			}

			if (!streq(link.password, test_case->expected_password)) {
				printf("FAIL expected password '%s', got '%s'\n",
				       test_case->expected_password, link.password);
			}

			if (!streq(link.hostname, test_case->expected_hostname)) {
				printf("FAIL expected hostname '%s', got '%s'\n",
				       test_case->expected_hostname, link.hostname);
			}

			make_zoommtg(&link, buffer, sizeof(buffer));
			if (!streq(buffer, test_case->expected_mtg)) {
				printf("FAIL expected mtg '%s', got '%s'\n",
				       test_case->expected_mtg, buffer);
			}
		}

		free_zoom_link(&link);
	}


	return failed;
}

int main()
{
	return test_parse();
}
