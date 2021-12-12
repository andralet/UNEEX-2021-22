/**@file argp_data.h
 * Data for argp support.
 * It is set up using function init_argp_data() from `argp.c`
 */

#ifndef ARGP_DATA_H
	#define ARGP_DATA_H

	#include <argp.h>

	#include "config.h"

	const char *argp_program_version, /**<Package version*/
		       *argp_program_bug_address /**Email for bug reports*/;
	struct argp_option options[2] /**Argp options (just '-r' for now)*/;

	/**Program arguments structure used by parse_opt*/
	struct arguments {
		_Bool parse_roman /**Boolean flag determining whether to use roman number form (when true) or just use decimal (when false)*/;
	};

	/**Argp control data structure*/
	struct argp argp;

	/**
	 * Fill data in required variables from `argp_data.h`.
	 * Must be called before working with argp.
	 */
	void init_argp_data(void);
	/**
	 * Parsing options for program (just '-r' in our case)
	 * For arguments description see argp documentation
	 */
	error_t parse_opt(int key, char *arg, struct argp_state *state);
	/**
	 * Filtering help messages for argp to translate help.
	 * For arguments description see argp documentation.
	 * TODO: make '--usage' option description translating.
	 */
	char *help_filter(int key, const char *text, void *input);
	/**
	 * Support function for help_filter. Allocates string and stores \p str contents there
	 * @param[in] str Input string for copy creation
	 * @returns Copy of \p str, allocated in dynamic memory
	 */
	char *malloc_copy(const char *str);
#endif // ARGP_DATA_H
