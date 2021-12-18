/**@file argp.c
 * Support for argp in program.
 */

#include "argp_data.h"

#include <string.h>
#include <stdlib.h>

#include <libintl.h>
#define _(string) gettext(string) /**< Alias for gettext */

/**
 * Fill data in required variables from `argp_data.h`.
 * Must be called before working with argp.
 */
void init_argp_data(void) {
	argp_program_version = PACKAGE_VERSION;
	argp_program_bug_address = PACKAGE_BUGREPORT;
	options[0] = (struct argp_option){"roman", 'r', 0, 0, _("Produce output in roman form instead of decimal"), 0};
	options[1] = (struct argp_option){0};
	argp = (struct argp){options, parse_opt, 0, _("A game, guessing a number you think out"), 0, help_filter, 0};
}

/**
 * Parsing options for program (just '-r' in our case)
 * For arguments description see argp documentation
 */
error_t parse_opt(int key, char *arg, struct argp_state *state) {
	(void)arg; // it's unused
	struct arguments *args = state->input;
	switch (key) {
		case 'r':
			args->parse_roman = 1;
			break;
		case ARGP_KEY_ARG:
			argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * Filtering help messages for argp to translate help.
 * For arguments description see argp documentation.
 * TODO: make '--usage' option description translating.
 */
char *help_filter(int key, const char *text, void *input) {
	(void)input; // it's unused
	switch (key) {
		case ARGP_KEY_HELP_PRE_DOC: return malloc_copy(_("Usage:"));
		case ARGP_KEY_HELP_POST_DOC: return (char *)text;
		case '?': return malloc_copy(_("Give this help list"));
		case 'V': return malloc_copy(_("Print program version"));
		// No idea, how to translate --usage option
		default:
			return malloc_copy(_(text));
	}
}

/**
 * Support function for help_filter. Allocates string and stores \p str contents there
 * @param[in] str Input string for copy creation
 * @returns Copy of \p str, allocated in dynamic memory
 */
char *malloc_copy(const char *str) {
	if (!str) {
		return NULL;
	}
	size_t len = strlen(str) + 1; // +1 - for '\0'
	char *res = malloc(sizeof(char) * len);
	if (!res) {
		return NULL;
	}
	memcpy(res, str, len);
	return res;
}
