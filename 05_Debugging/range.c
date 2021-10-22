#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum { CURRENT_UNDEFINED = INT_MIN };
enum {
	BAD_USAGE = 1,
	BAD_RANGE = 2
};

typedef struct {
	int start;
	int stop;
	int step;
	int current;
} range;

void argparse(int argc, char *argv[], int *start, int *stop, int *step) {
	switch (argc) {
		case 2: // [0, 1, ... N-1]
			*start = 0;
			*stop = atoi(argv[1]) - 1;
			*step = 1;
			break;
		case 3: // [M, M+1, ... N-1]
			*start = atoi(argv[1]);
			*stop = atoi(argv[2]) - 1;
			*step = 1;
			break;
		case 4: // [M, M+S, M+2S, ... N-1]
			*start = atoi(argv[1]);
			*stop = atoi(argv[2]) - 1;
			*step = atoi(argv[3]);
			break;
		default:
			printf("Usage:\n\
					\t%s N \t- generate series [0, 1, ... N-1]\n\
					\t%s N M \t- generate series [M, M+1, ... N-1]\n\
					\t%s N M S \t- generate series [M, M+S, ... N-1]\n\
					", argv[0], argv[0], argv[0]);
			exit((argc != 1) * BAD_USAGE);
	}
}

void range_init(range *I) {
	if (I) {
		I->current = I->start;
	}
}

_Bool range_run(const range *I) {
	if (!I || I->current == CURRENT_UNDEFINED || I->current < I->start || I->current > I->stop) {
		return 0;
	}
	// else
	return 1;
}

void range_next(range *I) {
	if (range_run(I)) { // to check runnable conditions
		I->current += I->step;
		if (I->current > I->stop) { // that's all for this range
			I->current = CURRENT_UNDEFINED;
		}
	}
}

int range_get(const range *I) {
	if (range_run(I)) { // check, whether the range is runnable
		return I->current;
	}
	// else
	fprintf(stderr, "range: Error: trying to get a value of nonrunnable range\n");
	exit(BAD_RANGE);
}

int main(int argc, char *argv[]) {
	range I;
	argparse(argc, argv, &I.start, &I.stop, &I.step);
	for(range_init(&I); range_run(&I); range_next(&I))
		printf("%d\n", range_get(&I));
	return 0;
}
