#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <regex.h>

enum { MATCH_NUM_MAX = 10 };

int print_reg_error(int reg_errno, regex_t *regex);

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <regexp> <substitution> <string>\n", argv[0]);
		return 1;
	}
	const char *regexp = argv[1], *substitution = argv[2], *string = argv[3];
	regex_t regex;
	int reg_errno = 0;
	if ((reg_errno = regcomp(&regex, regexp, REG_EXTENDED))) {
		if (print_reg_error(reg_errno, &regex) >= 0) {
			return 2;
		} else {
			return 3;
		}
	}

	regmatch_t matches[MATCH_NUM_MAX];
	if (regexec(&regex, string, MATCH_NUM_MAX, matches, 0) != REG_NOMATCH) {
		// check:
		size_t result_len = matches[0].rm_so;
		_Bool mark_started = 0;
		for (int i = 0; substitution[i]; i++) {
			if (mark_started) {
				if (substitution[i] == '\\') {
					result_len++; // we'll get one '\\' symbol
				} else if (isdigit(substitution[i])) {
					int id = substitution[i] - '0';
					if (matches[id].rm_so >= 0) {
						result_len += matches[id].rm_eo - matches[id].rm_so; // match inserted
					} else {
						fprintf(stderr, "esub: nothing to match \\%d in regexp %s\n", id, regexp);
						return 4;
					}
				} else {
					fprintf(stderr, "esub: unknown control sequence '\\%c'\n", substitution[i]);
					return 5;
				}
				mark_started = 0;
			} else {
				if (substitution[i] == '\\') {
					mark_started = 1;
				} else {
					result_len++;
				}
			}
		}
		result_len += (strlen(string) - matches[0].rm_eo + 1) + 1; // for end and '\0'
		if (mark_started) {
			fprintf(stderr, "esub: unexpected end of substitution %s in regexp %s\n", substitution, regexp);
			return 6;
		}

		char *result = malloc(sizeof(char) * result_len);
		if (!result) {
			perror("esub: result buffer allocation\n");
			return 7;
		}

		mark_started = 0;
		int result_id = matches[0].rm_so;
		strncpy(result, string, matches[0].rm_so);
		for (int i = 0; substitution[i]; i++) {
			if (mark_started) {
				if (substitution[i] == '\\') {
					result[result_id] = '\\';
					result_id++;
				} else { // errors checked before
					int id = substitution[i] - '0';
					strncpy(result + result_id, string + matches[id].rm_so, matches[id].rm_eo - matches[id].rm_so); // match inserted
					result_id += matches[id].rm_eo - matches[id].rm_so;
					// errors checked before
				}
				mark_started = 0;
			} else {
				if (substitution[i] == '\\') {
					mark_started = 1;
				} else {
					result[result_id] = substitution[i];
					result_id++;
				}
			}
		}
		strncpy(result + result_id, string + matches[0].rm_eo, strlen(string) - matches[0].rm_eo + 1);
		result[result_len - 1] = '\0';
		printf("%s\n", result);
	} else {
		printf("%s\n", string);
	}

	regfree(&regex);
	return 0;
}

int print_reg_error(int reg_errno, regex_t *regex) {
	size_t errbuf_size = regerror(reg_errno, regex, NULL, 0);
	char *errbuf = malloc(sizeof(char) * errbuf_size);
	if (!errbuf) {
		perror("esub: error buffer allocation\n");
		return -1;
	}
	regerror(reg_errno, regex, errbuf, errbuf_size);
	fprintf(stderr, "esub: %s\n", errbuf);
	free(errbuf);
	return 0;
}
