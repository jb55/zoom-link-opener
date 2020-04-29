
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
		&& link->url_data
		&& (streq(link->url_data->protocol, "http") ||
		    streq(link->url_data->protocol, "https"))
		&& (link->meeting_id && link->meeting_id[0] != 0)
		&& (strstr(link->hostname, "zoom.us") != 0);
}


int parse_zoom_link(char *url, struct zoom_link *link)
{
	url_data_t *parsed = url_parse(url);

	link->url_data = parsed;

	if (parsed && strlen(parsed->pathname) > 3)
		link->meeting_id = parsed->pathname + 3;
	else {
		link->meeting_id = "";
	}

	if (parsed)
		link->hostname = parsed->hostname;
	else
		link->hostname = "";

	if (parsed)
		parse_querystring(parsed->query, link,
				find_querystring_password);

	if (!parsed || (link && !link->password))
		link->password = "";

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
	if (link && link->url_data)
		url_free(link->url_data);
}
