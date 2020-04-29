#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct url {
	char *protocol;
	char *host;
	int port;
	char *path;
	char *query_string;
	int host_exists;
	char *host_ip;
};

void free_parsed_url(struct url *url_parsed) {
	if (url_parsed->protocol)
		free(url_parsed->protocol);
	if (url_parsed->host)
		free(url_parsed->host);
	if (url_parsed->path)
		free(url_parsed->path);
	if (url_parsed->query_string)
		free(url_parsed->query_string);

	//free(url_parsed);
}

int parse_url(char *url, struct url *parsed_url) {
	char *local_url = (char *) malloc(sizeof(char) * (strlen(url) + 1));
	char *token;
	char *token_host;
	char *host_port;

	char *token_ptr;
	char *host_token_ptr;

	char *path = NULL;

	// Copy our string
	strcpy(local_url, url);

	token = strtok_r(local_url, ":", &token_ptr);
	if (!token)
		return 1;
	parsed_url->protocol = (char *) malloc(sizeof(char) * strlen(token) + 1);
	strcpy(parsed_url->protocol, token);

	// Host:Port
	token = strtok_r(NULL, "/", &token_ptr);
	if (token) {
		host_port = (char *) malloc(sizeof(char) * (strlen(token) + 1));
		strcpy(host_port, token);
	} else {
		host_port = (char *) malloc(sizeof(char) * 1);
		strcpy(host_port, "");
	}

	token_host = strtok_r(host_port, ":", &host_token_ptr);
	parsed_url->host_ip = NULL;
	if (token_host) {
		parsed_url->host = (char *) malloc(sizeof(char) * strlen(token_host) + 1);
		strcpy(parsed_url->host, token_host);

		parsed_url->host_exists = -1;
	} else {
		parsed_url->host_exists = -1;
		parsed_url->host = NULL;
	}

	// Port
	token_host = strtok_r(NULL, ":", &host_token_ptr);
	if (token_host)
		parsed_url->port = atoi(token_host);
	else
		parsed_url->port = 0;

	token_host = strtok_r(NULL, ":", &host_token_ptr);
	assert(token_host == NULL);

	token = strtok_r(NULL, "?", &token_ptr);
	parsed_url->path = NULL;
	if (token) {
		path = (char *) realloc(path, sizeof(char) * (strlen(token) + 2));
		strcpy(path, "/");
		strcat(path, token);

		size_t s = sizeof(char) * strlen(path) + 1;
		parsed_url->path = (char *) malloc(s);
		strcpy(parsed_url->path, path);

		free(path);
	} else {
		parsed_url->path = (char *) malloc(sizeof(char) * 2);
		strcpy(parsed_url->path, "/");
	}

	token = strtok_r(NULL, "?", &token_ptr);
	if (token) {
		size_t s = sizeof(char) * (strlen(token) + 1);
		parsed_url->query_string = (char *) malloc(s);
		strcpy(parsed_url->query_string, token);
	} else {
		parsed_url->query_string = NULL;
	}

	token = strtok_r(NULL, "?", &token_ptr);
	assert(token == NULL);

	free(local_url);
	free(host_port);
	return 0;
}
