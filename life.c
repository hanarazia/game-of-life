#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "universe.h"

#define OPTIONS "tsn:i:o:H"
#define DELAY 50000

int main(int argc, char ** argv) {
	int opt;
	int toroidal = 0;
	int silence = 0;
	int generations = 100;
	FILE *input = stdin;
	FILE *output = stdout;

	while ((opt = getopt (argc, argv, OPTIONS)) != -1) {
		switch (opt) {
        	case 't': {
				toroidal = 1;
				break;
			}

			case 's': {
				silence = 1;
				break;
			}

			case 'n': {
				generations = atoi(optarg);
				break;
			}

			case 'i': {
                input = fopen(optarg, "r");
                if (!input) { 
					printf("Error: File %s not found.\n", optarg); 
					exit(1);
				}
				break;
            }

			case 'o': {
                output = fopen(optarg, "w");
                break;
            }
			
			case 'H': {
                printf("Game of life - options:\n");
                printf("\t-t: use a toroidal universe\n");
                printf("\t-s: silent mode\n");
                printf("\t-n <number>:  number of generations to play\n");
                printf("\t-i <file>: read universe from input file\n");
                printf("\t-o <file>: write final universe to output file\n");
                printf("\t-h: print help\n");
                exit(0);
            }
		}	
	}

	uint32_t rows;
	uint32_t cols;
	fscanf(input, "%u %u", &rows, &cols);

	Universe *ua = uv_create(rows, cols, toroidal);	
	Universe *ub = uv_create(rows, cols, toroidal);
	uv_populate(ua, input);
	if (input != stdin) { fclose(input); }
	// uv_print(ua, output);

	if (!silence) {
		initscr();
		curs_set(FALSE);
	}

	for (uint32_t i = 0; i < generations; i += 1) {
		if (!silence) { clear(); }
		
		for (uint32_t r = 0; r < uv_rows(ua); r++) {
			for (uint32_t c = 0; c < uv_cols(ua); c++) {
				uint32_t census = uv_census(ua, r, c);
				if (uv_get_cell(ua, r, c)) {
				// Processing live cell
					if (!silence) { mvprintw(r, c, "o"); }
					if (census == 2 || census == 3) {
						uv_live_cell(ub, r, c);
					}
					else { uv_dead_cell(ub, r, c); }
				}
				else {
				// This cell is dead
					if (census == 3) {
						uv_live_cell(ub, r, c);
					}
					else { uv_dead_cell(ub, r, c); }
				}
			}
		}

		if (!silence) {
            // Refresh the window
            refresh();

            // Sleep for 50000 microseconds
            usleep(DELAY);
        }
		Universe *temp = ua;
		ua = ub;
		ub = temp;
	}
	if (!silence) { endwin(); }
	uv_print(ua, output);

	if (output != stdout) { fclose(output); }
	
	uv_delete(ua);
	uv_delete(ub);

	return 0;

}





