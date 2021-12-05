#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <locale.h>
#include <libintl.h>

#define _(string) gettext(string)

int main(void) {
	setlocale(LC_ALL, "");

	bindtextdomain("guess", "po");
	textdomain("guess");

	printf(_("Think out a number between 1 and 100, and I'll guess it with power of my QUESTIONS!\n"));
	int low = 1, high = 100;
	char ans[20] = {};

	while (high - low > 1) {
		int med = (low + high) / 2;
		printf(_("Is it bigger than %d (yes/no)? "), med);
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
		printf(_("Is your number %d (yes/no)? "), low);
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
