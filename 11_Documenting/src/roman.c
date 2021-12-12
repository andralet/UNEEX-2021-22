/**@file roman.c
 * Support for roman number form
 */

#include "roman.h"

/**
 * Transform decimal number \p n and store result in buffer \p buf
 * @param[in] n Source number in decimal form (must be from [1; 3999])
 * @param[out] buf Buffer to save the result in roman form (must be at least 16 symbols long)
 * @return Status: -1 - fault, 0 - success
 */
int dec2roman(char *buf, int n) {
	if (n >= 4000 || n <= 0) {
		return -1;
	}
	size_t sym_id = 0;
	while (n >= 1000) {
		buf[sym_id] = 'M'; sym_id++;
		n -= 1000;
	}

	// now n < 1000
	if (n >= 900) {
		buf[sym_id] = 'C'; sym_id++;
		buf[sym_id] = 'M'; sym_id++;
		n -= 900;
	} else if (n >= 500) {
		buf[sym_id] = 'D'; sym_id++;
		n -= 500;
	} else if (n >= 400) {
		buf[sym_id] = 'C'; sym_id++;
		buf[sym_id] = 'D'; sym_id++;
		n -= 400;
	}
	while (n >= 100) {
		buf[sym_id] = 'C'; sym_id++;
		n -= 100;
	}

	// now n < 100
	if (n >= 90) {
		buf[sym_id] = 'X'; sym_id++;
		buf[sym_id] = 'C'; sym_id++;
		n -= 90;
	} else if (n >= 50) {
		buf[sym_id] = 'L'; sym_id++;
		n -= 50;
	} else if (n >= 40) {
		buf[sym_id] = 'X'; sym_id++;
		buf[sym_id] = 'L'; sym_id++;
		n -= 40;
	}
	while (n >= 10) {
		buf[sym_id] = 'X'; sym_id++;
		n -= 10;
	}

	// now n < 10
	if (n >= 9) {
		buf[sym_id] = 'I'; sym_id++;
		buf[sym_id] = 'X'; sym_id++;
		n -= 9;
	} else if (n >= 5) {
		buf[sym_id] = 'V'; sym_id++;
		n -= 5;
	} else if (n >= 4) {
		buf[sym_id] = 'I'; sym_id++;
		buf[sym_id] = 'V'; sym_id++;
		n -= 4;
	}
	while (n >= 1) {
		buf[sym_id] = 'I'; sym_id++;
		n -= 1;
	}

	buf[sym_id] = '\0';
	return 0;
}
