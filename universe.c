#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "universe.h"

struct Universe {
	uint32_t rows;
	uint32_t cols;
	bool **grid;
	bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) calloc(1, sizeof(Universe));
	u->rows = rows;
	u->cols = cols;
	u->toroidal = toroidal;
	u->grid = (bool **) calloc(rows, sizeof(bool *));
	for (uint32_t r = 0; r < rows; r++) {
		u->grid[r] = (bool *) calloc(cols, sizeof(bool));
	}
	return u;
}

void uv_delete(Universe *u) {
	for (uint32_t i = 0; i < u->rows; ++i) {
		free(u->grid[i]);
	}	
	free(u->grid);
	free(u);
}

uint32_t uv_rows(Universe *u) {
	return u->rows;
}

uint32_t uv_cols(Universe *u) {
	return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
	u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
	if (r < 0 || r >= u->rows || c < 0 || c >= u->cols) {
		return false;
	}	
	return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
	uint32_t row;
	uint32_t col;

	while (!feof(infile)) {
		fscanf(infile, "%u %u", &row, &col); 
		uv_live_cell(u, row, col);
	}
	return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
	uint32_t census = 0;
	// Neighbors coordinates
	uint32_t n_row;
	uint32_t n_col;
	
	// East
	n_row = r;
	n_col = c + 1;
	if (u->toroidal) { n_row = n_row % uv_rows(u); }
	if (u->toroidal) { n_col = n_col % uv_cols(u); }
	if (uv_get_cell(u, n_row, n_col)) { census += 1;  }

	// Northeast
    n_row = r - 1;
    n_col = c + 1;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }
	
	// North
    n_row = r - 1;
    n_col = c;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }
	
	// North-west
    n_row = r - 1;
    n_col = c - 1;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }

	// West
    n_row = r;
    n_col = c - 1;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }

	// South-west
    n_row = r + 1;
    n_col = c - 1;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }

	// South
    n_row = r + 1;
    n_col = c;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }

	// South-east
    n_row = r + 1;
    n_col = c + 1;
    if (u->toroidal) { n_row = n_row % uv_rows(u); }
    if (u->toroidal) { n_col = n_col % uv_cols(u); }
    if (uv_get_cell(u, n_row, n_col)) { census += 1;  }

	return census;
}

void uv_print(Universe *u, FILE *outfile) {
	for (uint32_t r = 0; r < uv_rows(u); r++) {
		for (uint32_t c = 0; c < uv_cols(u); c++) {
			if (uv_get_cell(u, r, c)) {
				fprintf(outfile, "o");
			}
			else {
				fprintf(outfile, ".");
			}
		}
		fprintf(outfile, "\n");
	}
}

