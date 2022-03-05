/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Main execution entry point for Wanderers program.                         *
 * This program simulates a procedurally generated solarsystem.
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Internal Includes */
#include "wanderers.h"

/* Main function, entry point for execution. */
int main(int argc, char** args) {
	// Run the Wanderers program.
	wanderers::run();

	return 0;
}