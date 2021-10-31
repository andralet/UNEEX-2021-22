#include <string.h>
#include <errno.h>

#define __USE_GNU
#include <dlfcn.h>
typedef int (*unlink_type)(const char *);

int unlink(const char *pathname) {
	if (strstr(pathname, "PROTECT")) { // if filename conatins PROTECT
		errno = EPERM;
		return -1;
	}
	// else - default behaviour
	unlink_type true_unlink = (unlink_type)dlsym(RTLD_NEXT, "unlink");
	return true_unlink(pathname);
}
