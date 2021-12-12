/**@file guess.c
 * Main file with game code
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <locale.h>
#include <libintl.h>
#define _(string) gettext(string) /**< Alias for gettext */

#include "argp_data.h"
#include "roman.h"

#include "config.h"

// just converts n to n to make parsing system easier (Mmm... function pointers:))
int dec2str(char *buf, int n); 
typedef int (*DecParser)(char *, int); /**< Typedef for parser creating (it makes work in different number forms a bit easier) */

/** Low and high limits for number guessing. If you change them, make sure you fit the borders*/
enum {
	LOW = 1, /**< Must be in \[1; 3998\] (for reason see `roman.c`).*/
	HIGH = 3999 /**< Must be in \[2;3999\] (for reason see `roman.c`) and greater than LOW. */
};

/**
 * Main functions, implementing game logic. P.S. beware of fork)
 * @param[in] argc Argument number
 * @param[in] argv Arguments
 * @returns 0
 */
int main(int argc, char **argv) {
	setlocale(LC_ALL, "");

	bindtextdomain("guess", "po");
	textdomain("guess");

	init_argp_data();
	struct arguments args;
	args.parse_roman = 0;
	argp_parse(&argp, argc, argv, 0, 0, &args);

	char buf[32] = "", buf2[32] = "";
	DecParser parse = NULL;
	if (args.parse_roman) {
		parse = dec2roman;
	} else {
		parse = dec2str;
	}

	parse(buf, LOW);
	// parse(buf + 16, HIGH); // if you read docs properly, but who does?
	parse(buf2, HIGH);
	printf(_("Think out a number between %s and %s, and I'll guess it with power of my QUESTIONS!\n"), buf, buf2);
	int low = LOW, high = HIGH;
	char ans[20] = {};

	while (high - low > 1) {
		int med = (low + high) / 2;
		parse(buf, med);
		printf(_("Is it bigger than %s (yes/no)? "), buf);
		scanf("%10s", ans);
		ans[19] = '\0';
		if (!strcmp(ans, _("yes"))) {
			low = med + 1;
		} else if (!strcmp(ans, _("no"))) {
			high = med;
		} else {
			printf(_("yes/no, please\n"));
			ungetc('x', stdin);
			scanf("%*s"); // clearing buffer just in case
		}
	}

	while(1) {
		parse(buf, low);
		printf(_("Is your number %s (yes/no)? "), buf);
		scanf("%10s", ans);
		ans[19] = '\0';
		if (!strcmp(ans, _("yes"))) {
			break;
		} else if (!strcmp(ans, _("no"))) {
			if (low < high) {
				low++;
			} else {
				printf(_("Liar! Never liked liars...\n"));
				if (!fork()) {
					while(1) {
						sleep(1);
						printf(_("LIAR!\n"));
					}
				}
				return 1;
			}
		} else {
			printf(_("yes/no, please\n"));
			ungetc('x', stdin);
			scanf("%*s"); // clearing buffer just in case
		}
	}
	printf(_("Just like I thought. That was simple!\n"));
	return 0;
}

/**Function, identically transforming decimal number into its string representation. Useful for parsing system (see `guess.c`)
 * @param[out] buf Output buffer (must be long enough to hold the number)
 * @param[in] n Number to transform
 * @returns 0
 */
int dec2str(char *buf, int n) {
	sprintf(buf, "%d", n);
	return 0;
}
