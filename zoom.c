
#include <string.h>

#include "zoom.h"

static void parse_querystring(char *qs, void *data, void (*pair)(void *, char *,char *))
{
	char *ands, *ands_r;
	char *equals_r;
	char *first;
	char *second;

	if (qs && qs[0] == '?')
		qs++;

	ands = strtok_r(qs, "&", &ands_r);
	while (ands != NULL) {
		first = strtok_r(ands, "=", &equals_r);
		second = strtok_r(NULL, "=", &equals_r);
		pair(data, first, second);
		ands = strtok_r(NULL, "&", &ands_r);
	}
}

static void find_querystring_password(void* voidlink, char *key,
				      char *val)
{
	struct zoom_link *link = (struct zoom_link*) voidlink;

	if (streq(key, "pwd"))
		link->password = val;
}

bool validate_zoom_link(struct zoom_link *link)
{
	return link
		&& (link->url.protocol && !streq(link->url.protocol, "zoommtg"))
		&& (link->meeting_id && link->meeting_id[0] != 0)
		&& (strstr(link->hostname, "zoom.us") != 0);
}


void print_zoom_link(struct zoom_link *link) 
{
	printf("proto %s path %s hostname %s meeting_id %s\n",
	       link->url.protocol,
	       link->url.path,
	       link->hostname,
		link->meeting_id);
}

int parse_zoom_link(char *url, struct zoom_link *link)
{
	memset(&link->url, 0, sizeof(link->url));
	int error = parse_url(url, &link->url);
	if (error != 0) {
		memset(&link->url, 0, sizeof(link->url));
		return 0;
	}

	if (strlen(link->url.path) > 3)
		link->meeting_id = link->url.path + 3;
	else {
		link->meeting_id = (char*)"";
	}

	link->hostname = link->url.host;

	if (link->url.query_string)
		parse_querystring(link->url.query_string, link, 
			find_querystring_password);

	if (!link->url.query_string || (link && !link->password))
		link->password = (char*)"";

	return validate_zoom_link(link);
}

void make_zoommtg(struct zoom_link *link, char *buffer, size_t bufsize)
{
	bool has_password = link->password && link->password[0] != 0;
	const char *fmt = has_password?
		"zoommtg://%s/start?confno=%s&pwd=%s" :
		"zoommtg://%s/start?confno=%s%s";

	snprintf(buffer, bufsize, fmt, link->hostname,
		 link->meeting_id, link->password);
}

void init_zoom_link(struct zoom_link *link)
{
	memset(link, 0, sizeof(*link));
}

void free_zoom_link(struct zoom_link *link)
{
	free_parsed_url(&link->url);
}
