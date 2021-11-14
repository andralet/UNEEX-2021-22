#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <rhash.h>

#include "config.h"

#ifdef USE_READLINE
	#include <readline/readline.h>
	#include <readline/history.h>
#endif

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef MAX_HASH_NAME_LEN
	#define MAX_HASH_NAME_LEN 10
#endif

enum { HASH_NUM = 3 };

typedef enum { false, true } bool;

int main(int argc, char **argv) {
	const char *hash_names[HASH_NUM] = {"md5", "sha1", "tth"};
	const int hash_name_len[HASH_NUM] = {3, 4, 3};
	const int hash_codes[HASH_NUM] = {RHASH_MD5, RHASH_SHA1, RHASH_TTH};
	char hash_name[MAX_HASH_NAME_LEN];
	char hash_name_length_format[10];
	sprintf(hash_name_length_format, "%%%ds", MAX_HASH_NAME_LEN);

	#ifndef USE_READLINE
		char *cmd = malloc(BUF_SIZE * sizeof(char));
		size_t cmd_len = BUF_SIZE;
	#else
		char *cmd = NULL;
		size_t cmd_len = 0;
	#endif

	bool quiet = 0;
	if (argc > 1 && !strncmp(argv[1], "-q", 3)) {
		quiet = 1;
	}

	rhash_library_init(); /* initialize static data */
   	unsigned char digest[64];
	char output[130];

	if (!quiet) {
		printf("Welcome to hasher!\n");
	}
	while (true) {
		#ifndef USE_READLINE
			// getline version:
			if (!quiet) {
				printf("hasher> ");
			}
			errno = 0;
			if (getline(&cmd, &cmd_len, stdin) < 0) {
				if (errno) {
					perror("hasher");
					free(cmd);
					return 1;
				}
				break;
			}
			// removing '\n':
			*strchr(cmd, '\n') = '\0';
		#else
			errno = 0;
			if (cmd) {
				free(cmd);
			}
			if (!(cmd = readline(quiet ? "" : "hasher> "))) {
				if (errno) {
					perror("hasher");
					return 1;
				}
				break;
			}
			cmd_len = strlen(cmd);
		#endif

		// executing command:
		if (!strncmp(cmd, "quit", cmd_len) || !strncmp(cmd, "q", cmd_len)) {
			break;
		}
		bool unknown_command = true;
		for (int i = 0; i < HASH_NUM; i++) {
			sscanf(cmd, hash_name_length_format, hash_name);
			if (!strncasecmp(hash_name, hash_names[i], hash_name_len[i])) {
				bool use_hex = isupper(hash_name[0]);
				const char* data = strstr(cmd, hash_name) + hash_name_len[i];
				while (isspace(*data)) {
					data++;
				}

				if (data[0] == '\"') {
					// string:
					data += 1;
   					int res = rhash_msg(hash_codes[i], data, strlen(data), digest);
   					if(res < 0) {
     					fprintf(stderr, "LibRHash error: '%s': %s\n", data, strerror(errno));
     					break;
   					}
				} else {
					// file:
					int res = rhash_file(hash_codes[i], data, digest);
					if(res < 0) {
					    fprintf(stderr, "LibRHash error: %s: %s\n", data, strerror(errno));
					    break;
   					}
				}
   				rhash_print_bytes(output, digest, rhash_get_digest_size(hash_codes[i]),
					(use_hex ? (RHPR_HEX | RHPR_UPPERCASE) : RHPR_BASE64));
   				printf("%s\n", output);
				unknown_command = false;
			}
		}
		if (unknown_command) {
			fprintf(stderr, "Unknown command: '%s'\n", cmd);
		}
	}
	free(cmd);
	if (!quiet) {
		printf("Exit\n");
	}
	return 0;
}
