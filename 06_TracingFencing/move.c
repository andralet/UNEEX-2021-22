#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
		return 1;
	}

	// Opening input file:
	int fin = open(argv[1], O_RDONLY);
	if (fin < 0) {
		perror("Source file open failed");
		return 2;
	}
	struct stat fin_stat = {};
	if (fstat(fin, &fin_stat) < 0) {
		perror("Cannot retrieve source file mode data");
		close(fin);
		return 3;
	}

	// Creating buffer to copy data:
	unsigned long long buf_size = 100ULL<<20; // 100MB
	if (buf_size > fin_stat.st_size / 10) {
		buf_size = fin_stat.st_size / 10; // We don't want to use much memory, do we?
	}
	char *buf = malloc(buf_size);
	if (!buf) {
		perror("Could not allocate buffer for copying");
		close(fin);
		return 4;
	}

	// Opening output file:
	int fout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, fin_stat.st_mode);
	if (fout < 0) {
		perror("Destination file open failed");
		close(fin);
		free(buf);
		return 5;
	}

	// Time for copy!
	if (fin_stat.st_size > 0) { // if we have anything to copy
		ssize_t read_count = -1;
		_Bool had_intr = 0; // to prevent infinite loop
		do {
			errno = 0;
			while((read_count = read(fin, buf, buf_size)) > 0) {
				had_intr = 0;
				while (read_count > 0) {
					ssize_t write_count = write(fout, buf, read_count);
					if (write_count < 0) {
						perror("Write to destination file failed");
						free(buf);
						close(fin); close(fout);
						unlink(argv[2]); // trying to remove destination file
						return 6;
					}
					read_count -= write_count; // it may take couple writes
				}
			}
			if (had_intr) {
				break;
			} else {
				had_intr = 1;
			}
		} while(errno == EINTR); // we shall retry reading if we got just signal interruption
		if (read_count != 0) { // if the reason is not EOF
			perror("Read from source file failed");
			free(buf);
			close(fin); close(fout);
			unlink(argv[2]); // trying to remove destination file
			return 7;
		}
	}

	// Done. Freeing and closing everything
	free(buf);
	if (fsync(fout) < 0) {
		perror("Could not syncronise output with disk");
		close(fout); close(fin);
		unlink(argv[2]); // trying to remove destination file
		return 8;
	}
	if (close(fout) < 0) {
		// Well... I hope we're running not on HP-UX and I shall not write defines for platform and retry close() if errno = EINR
		perror("Could not close destination file");
		close(fin);
		unlink(argv[2]); // trying to remove destination file
		return 9;
	}
	// Result saved succesfully, so we can unlink source file:
	if (unlink(argv[1]) < 0) {
		perror("Could not delete source file");
		// Maybe it had protected in name:) So, probably, we don't want to lose destination:
		fprintf(stderr, "Destination file kept saved.\n");
		close(fin);
		return 10;
	}
	if (close(fin) < 0) {
		perror("Could not close source file");
		return 11;
	}

	// WE MADE IT!!!
	return 0;
}

