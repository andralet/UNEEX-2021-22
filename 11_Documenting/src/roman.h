/**@file roman.h
 * Header file for roman number form support.
 */

#ifndef ROMAN_H
	#define ROMAN_H

	#include <stddef.h>

	/**
	 * Transform decimal number \p n and store result in buffer \p buf
	 * @param[in] n Source number in decimal form (must be from [1; 3999])
	 * @param[out] buf Buffer to save the result in roman form (must be at least 16 symbols long)
	 * @return Status: -1 - fault, 0 - success
	 */
	int dec2roman(char *buf, int n);
#endif // ROMAN_H
