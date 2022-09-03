#include <stdio.h>
#include <stdlib.h>

struct xy {
	int x;
	int y;
};

#define UNUSED_MARKER -1

#define SIDE 8

#define N_JUMP_DISTANCES 8
int jump_distances_x[N_JUMP_DISTANCES] = {
	-1,
	1,
	-2,
	2,
	-2,
	2,
	-1,
	1,
};

int jump_distances_y[N_JUMP_DISTANCES] = {
	-2,
	-2,
	-1,
	-1,
	1,
	1,
	2,
	2,
};

void print_board(int *board) {
	for (int y = 0; y < SIDE; y++) {
		for (int x = 0; x < SIDE; x++) {
			printf("%2d, ", board[SIDE * y + x]);
		}
		printf("\n");
	}
}

int *create_board() {
	int *board = malloc(SIDE * SIDE * sizeof(int));
	for (int i = 0; i < SIDE * SIDE; i++)
		board[i] = UNUSED_MARKER;
	return board;
}

int is_jump_in_bounds(struct xy pos, int i) {
	int target_y = pos.y + jump_distances_y[i];
	int target_x = pos.x + jump_distances_x[i];
	if (target_y < 0) return 0;
	if (target_y >= SIDE) return 0;
	if (target_x < 0) return 0;
	if (target_x >= SIDE) return 0;
	return 1;
}

void mark_jump(struct xy pos, int jump_i, int *board, int origin_value) {
	int i = (pos.y + jump_distances_y[jump_i]) * SIDE + (pos.x + jump_distances_x[jump_i]);
	if (board[i] >= 0)
		return; // Already marked
	board[i] = origin_value + 1;
}

int count_unused_spaces(int *board) {
	int n_unused = 0;
	for (int i = 0; i < SIDE * SIDE; i++)
		if (UNUSED_MARKER == board[i])
			n_unused++;
	return n_unused;
}

void mark_jumps(int origin_value, int *board) {
	if (0 == count_unused_spaces(board))
		return;

	for (int y = 0; y < SIDE; y++) {
		for (int x = 0; x < SIDE; x++) {
			if (origin_value == board[y * SIDE + x]) {
				struct xy pos = {
					.x = x,
					.y = y,
				};
				for (int i = 0; i < N_JUMP_DISTANCES; i++) {
					if (is_jump_in_bounds(pos, i)) {
						mark_jump(pos, i, board, origin_value);
					}
				}
			}
		}
	}


	mark_jumps(origin_value + 1, board);
}

int main(int argc, char **argv) {
	int *board = create_board();
	board[7] = 0; // Mark start position
	mark_jumps(0, board);
	print_board(board);
	free(board);
	return 0;
}
