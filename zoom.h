
#ifndef ZOOMLO_H
#define ZOOMLO_H

#include "url.h"

#define streq(a,b) (strcmp(a,b) == 0)

struct zoom_link {
	url_data_t *url_data;
	char *meeting_id;
	char *hostname;
	char *password;
};

void init_zoom_link(struct zoom_link *link);
void free_zoom_link(struct zoom_link *link);
int parse_zoom_link(char *url, struct zoom_link *link);
bool validate_zoom_link(struct zoom_link *link);
void make_zoommtg(struct zoom_link *link, char *buffer, size_t bufsize);

#endif /* ZOOM_H */
