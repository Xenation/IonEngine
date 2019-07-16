#pragma once

const unsigned int zorder_x[256] = {
	0b000000000000000000000000,
	0b000000000000000000000001,
	0b000000000000000000001000,
	0b000000000000000000001001,
	0b000000000000000001000000,
	0b000000000000000001000001,
	0b000000000000000001001000,
	0b000000000000000001001001,
	0b000000000000001000000000,
	0b000000000000001000000001,
	0b000000000000001000001000,
	0b000000000000001000001001,
	0b000000000000001001000000,
	0b000000000000001001000001,
	0b000000000000001001001000,
	0b000000000000001001001001,
	0b000000000001000000000000,
	0b000000000001000000000001,
	0b000000000001000000001000,
	0b000000000001000000001001,
	0b000000000001000001000000,
	0b000000000001000001000001,
	0b000000000001000001001000,
	0b000000000001000001001001,
	0b000000000001001000000000,
	0b000000000001001000000001,
	0b000000000001001000001000,
	0b000000000001001000001001,
	0b000000000001001001000000,
	0b000000000001001001000001,
	0b000000000001001001001000,
	0b000000000001001001001001,
	0b000000001000000000000000,
	0b000000001000000000000001,
	0b000000001000000000001000,
	0b000000001000000000001001,
	0b000000001000000001000000,
	0b000000001000000001000001,
	0b000000001000000001001000,
	0b000000001000000001001001,
	0b000000001000001000000000,
	0b000000001000001000000001,
	0b000000001000001000001000,
	0b000000001000001000001001,
	0b000000001000001001000000,
	0b000000001000001001000001,
	0b000000001000001001001000,
	0b000000001000001001001001,
	0b000000001001000000000000,
	0b000000001001000000000001,
	0b000000001001000000001000,
	0b000000001001000000001001,
	0b000000001001000001000000,
	0b000000001001000001000001,
	0b000000001001000001001000,
	0b000000001001000001001001,
	0b000000001001001000000000,
	0b000000001001001000000001,
	0b000000001001001000001000,
	0b000000001001001000001001,
	0b000000001001001001000000,
	0b000000001001001001000001,
	0b000000001001001001001000,
	0b000000001001001001001001,
	0b000001000000000000000000,
	0b000001000000000000000001,
	0b000001000000000000001000,
	0b000001000000000000001001,
	0b000001000000000001000000,
	0b000001000000000001000001,
	0b000001000000000001001000,
	0b000001000000000001001001,
	0b000001000000001000000000,
	0b000001000000001000000001,
	0b000001000000001000001000,
	0b000001000000001000001001,
	0b000001000000001001000000,
	0b000001000000001001000001,
	0b000001000000001001001000,
	0b000001000000001001001001,
	0b000001000001000000000000,
	0b000001000001000000000001,
	0b000001000001000000001000,
	0b000001000001000000001001,
	0b000001000001000001000000,
	0b000001000001000001000001,
	0b000001000001000001001000,
	0b000001000001000001001001,
	0b000001000001001000000000,
	0b000001000001001000000001,
	0b000001000001001000001000,
	0b000001000001001000001001,
	0b000001000001001001000000,
	0b000001000001001001000001,
	0b000001000001001001001000,
	0b000001000001001001001001,
	0b000001001000000000000000,
	0b000001001000000000000001,
	0b000001001000000000001000,
	0b000001001000000000001001,
	0b000001001000000001000000,
	0b000001001000000001000001,
	0b000001001000000001001000,
	0b000001001000000001001001,
	0b000001001000001000000000,
	0b000001001000001000000001,
	0b000001001000001000001000,
	0b000001001000001000001001,
	0b000001001000001001000000,
	0b000001001000001001000001,
	0b000001001000001001001000,
	0b000001001000001001001001,
	0b000001001001000000000000,
	0b000001001001000000000001,
	0b000001001001000000001000,
	0b000001001001000000001001,
	0b000001001001000001000000,
	0b000001001001000001000001,
	0b000001001001000001001000,
	0b000001001001000001001001,
	0b000001001001001000000000,
	0b000001001001001000000001,
	0b000001001001001000001000,
	0b000001001001001000001001,
	0b000001001001001001000000,
	0b000001001001001001000001,
	0b000001001001001001001000,
	0b000001001001001001001001,
	0b001000000000000000000000,
	0b001000000000000000000001,
	0b001000000000000000001000,
	0b001000000000000000001001,
	0b001000000000000001000000,
	0b001000000000000001000001,
	0b001000000000000001001000,
	0b001000000000000001001001,
	0b001000000000001000000000,
	0b001000000000001000000001,
	0b001000000000001000001000,
	0b001000000000001000001001,
	0b001000000000001001000000,
	0b001000000000001001000001,
	0b001000000000001001001000,
	0b001000000000001001001001,
	0b001000000001000000000000,
	0b001000000001000000000001,
	0b001000000001000000001000,
	0b001000000001000000001001,
	0b001000000001000001000000,
	0b001000000001000001000001,
	0b001000000001000001001000,
	0b001000000001000001001001,
	0b001000000001001000000000,
	0b001000000001001000000001,
	0b001000000001001000001000,
	0b001000000001001000001001,
	0b001000000001001001000000,
	0b001000000001001001000001,
	0b001000000001001001001000,
	0b001000000001001001001001,
	0b001000001000000000000000,
	0b001000001000000000000001,
	0b001000001000000000001000,
	0b001000001000000000001001,
	0b001000001000000001000000,
	0b001000001000000001000001,
	0b001000001000000001001000,
	0b001000001000000001001001,
	0b001000001000001000000000,
	0b001000001000001000000001,
	0b001000001000001000001000,
	0b001000001000001000001001,
	0b001000001000001001000000,
	0b001000001000001001000001,
	0b001000001000001001001000,
	0b001000001000001001001001,
	0b001000001001000000000000,
	0b001000001001000000000001,
	0b001000001001000000001000,
	0b001000001001000000001001,
	0b001000001001000001000000,
	0b001000001001000001000001,
	0b001000001001000001001000,
	0b001000001001000001001001,
	0b001000001001001000000000,
	0b001000001001001000000001,
	0b001000001001001000001000,
	0b001000001001001000001001,
	0b001000001001001001000000,
	0b001000001001001001000001,
	0b001000001001001001001000,
	0b001000001001001001001001,
	0b001001000000000000000000,
	0b001001000000000000000001,
	0b001001000000000000001000,
	0b001001000000000000001001,
	0b001001000000000001000000,
	0b001001000000000001000001,
	0b001001000000000001001000,
	0b001001000000000001001001,
	0b001001000000001000000000,
	0b001001000000001000000001,
	0b001001000000001000001000,
	0b001001000000001000001001,
	0b001001000000001001000000,
	0b001001000000001001000001,
	0b001001000000001001001000,
	0b001001000000001001001001,
	0b001001000001000000000000,
	0b001001000001000000000001,
	0b001001000001000000001000,
	0b001001000001000000001001,
	0b001001000001000001000000,
	0b001001000001000001000001,
	0b001001000001000001001000,
	0b001001000001000001001001,
	0b001001000001001000000000,
	0b001001000001001000000001,
	0b001001000001001000001000,
	0b001001000001001000001001,
	0b001001000001001001000000,
	0b001001000001001001000001,
	0b001001000001001001001000,
	0b001001000001001001001001,
	0b001001001000000000000000,
	0b001001001000000000000001,
	0b001001001000000000001000,
	0b001001001000000000001001,
	0b001001001000000001000000,
	0b001001001000000001000001,
	0b001001001000000001001000,
	0b001001001000000001001001,
	0b001001001000001000000000,
	0b001001001000001000000001,
	0b001001001000001000001000,
	0b001001001000001000001001,
	0b001001001000001001000000,
	0b001001001000001001000001,
	0b001001001000001001001000,
	0b001001001000001001001001,
	0b001001001001000000000000,
	0b001001001001000000000001,
	0b001001001001000000001000,
	0b001001001001000000001001,
	0b001001001001000001000000,
	0b001001001001000001000001,
	0b001001001001000001001000,
	0b001001001001000001001001,
	0b001001001001001000000000,
	0b001001001001001000000001,
	0b001001001001001000001000,
	0b001001001001001000001001,
	0b001001001001001001000000,
	0b001001001001001001000001,
	0b001001001001001001001000,
	0b001001001001001001001001
};

const unsigned int zorder_y[256] = {
	0b000000000000000000000000,
	0b000000000000000000000010,
	0b000000000000000000010000,
	0b000000000000000000010010,
	0b000000000000000010000000,
	0b000000000000000010000010,
	0b000000000000000010010000,
	0b000000000000000010010010,
	0b000000000000010000000000,
	0b000000000000010000000010,
	0b000000000000010000010000,
	0b000000000000010000010010,
	0b000000000000010010000000,
	0b000000000000010010000010,
	0b000000000000010010010000,
	0b000000000000010010010010,
	0b000000000010000000000000,
	0b000000000010000000000010,
	0b000000000010000000010000,
	0b000000000010000000010010,
	0b000000000010000010000000,
	0b000000000010000010000010,
	0b000000000010000010010000,
	0b000000000010000010010010,
	0b000000000010010000000000,
	0b000000000010010000000010,
	0b000000000010010000010000,
	0b000000000010010000010010,
	0b000000000010010010000000,
	0b000000000010010010000010,
	0b000000000010010010010000,
	0b000000000010010010010010,
	0b000000010000000000000000,
	0b000000010000000000000010,
	0b000000010000000000010000,
	0b000000010000000000010010,
	0b000000010000000010000000,
	0b000000010000000010000010,
	0b000000010000000010010000,
	0b000000010000000010010010,
	0b000000010000010000000000,
	0b000000010000010000000010,
	0b000000010000010000010000,
	0b000000010000010000010010,
	0b000000010000010010000000,
	0b000000010000010010000010,
	0b000000010000010010010000,
	0b000000010000010010010010,
	0b000000010010000000000000,
	0b000000010010000000000010,
	0b000000010010000000010000,
	0b000000010010000000010010,
	0b000000010010000010000000,
	0b000000010010000010000010,
	0b000000010010000010010000,
	0b000000010010000010010010,
	0b000000010010010000000000,
	0b000000010010010000000010,
	0b000000010010010000010000,
	0b000000010010010000010010,
	0b000000010010010010000000,
	0b000000010010010010000010,
	0b000000010010010010010000,
	0b000000010010010010010010,
	0b000010000000000000000000,
	0b000010000000000000000010,
	0b000010000000000000010000,
	0b000010000000000000010010,
	0b000010000000000010000000,
	0b000010000000000010000010,
	0b000010000000000010010000,
	0b000010000000000010010010,
	0b000010000000010000000000,
	0b000010000000010000000010,
	0b000010000000010000010000,
	0b000010000000010000010010,
	0b000010000000010010000000,
	0b000010000000010010000010,
	0b000010000000010010010000,
	0b000010000000010010010010,
	0b000010000010000000000000,
	0b000010000010000000000010,
	0b000010000010000000010000,
	0b000010000010000000010010,
	0b000010000010000010000000,
	0b000010000010000010000010,
	0b000010000010000010010000,
	0b000010000010000010010010,
	0b000010000010010000000000,
	0b000010000010010000000010,
	0b000010000010010000010000,
	0b000010000010010000010010,
	0b000010000010010010000000,
	0b000010000010010010000010,
	0b000010000010010010010000,
	0b000010000010010010010010,
	0b000010010000000000000000,
	0b000010010000000000000010,
	0b000010010000000000010000,
	0b000010010000000000010010,
	0b000010010000000010000000,
	0b000010010000000010000010,
	0b000010010000000010010000,
	0b000010010000000010010010,
	0b000010010000010000000000,
	0b000010010000010000000010,
	0b000010010000010000010000,
	0b000010010000010000010010,
	0b000010010000010010000000,
	0b000010010000010010000010,
	0b000010010000010010010000,
	0b000010010000010010010010,
	0b000010010010000000000000,
	0b000010010010000000000010,
	0b000010010010000000010000,
	0b000010010010000000010010,
	0b000010010010000010000000,
	0b000010010010000010000010,
	0b000010010010000010010000,
	0b000010010010000010010010,
	0b000010010010010000000000,
	0b000010010010010000000010,
	0b000010010010010000010000,
	0b000010010010010000010010,
	0b000010010010010010000000,
	0b000010010010010010000010,
	0b000010010010010010010000,
	0b000010010010010010010010,
	0b010000000000000000000000,
	0b010000000000000000000010,
	0b010000000000000000010000,
	0b010000000000000000010010,
	0b010000000000000010000000,
	0b010000000000000010000010,
	0b010000000000000010010000,
	0b010000000000000010010010,
	0b010000000000010000000000,
	0b010000000000010000000010,
	0b010000000000010000010000,
	0b010000000000010000010010,
	0b010000000000010010000000,
	0b010000000000010010000010,
	0b010000000000010010010000,
	0b010000000000010010010010,
	0b010000000010000000000000,
	0b010000000010000000000010,
	0b010000000010000000010000,
	0b010000000010000000010010,
	0b010000000010000010000000,
	0b010000000010000010000010,
	0b010000000010000010010000,
	0b010000000010000010010010,
	0b010000000010010000000000,
	0b010000000010010000000010,
	0b010000000010010000010000,
	0b010000000010010000010010,
	0b010000000010010010000000,
	0b010000000010010010000010,
	0b010000000010010010010000,
	0b010000000010010010010010,
	0b010000010000000000000000,
	0b010000010000000000000010,
	0b010000010000000000010000,
	0b010000010000000000010010,
	0b010000010000000010000000,
	0b010000010000000010000010,
	0b010000010000000010010000,
	0b010000010000000010010010,
	0b010000010000010000000000,
	0b010000010000010000000010,
	0b010000010000010000010000,
	0b010000010000010000010010,
	0b010000010000010010000000,
	0b010000010000010010000010,
	0b010000010000010010010000,
	0b010000010000010010010010,
	0b010000010010000000000000,
	0b010000010010000000000010,
	0b010000010010000000010000,
	0b010000010010000000010010,
	0b010000010010000010000000,
	0b010000010010000010000010,
	0b010000010010000010010000,
	0b010000010010000010010010,
	0b010000010010010000000000,
	0b010000010010010000000010,
	0b010000010010010000010000,
	0b010000010010010000010010,
	0b010000010010010010000000,
	0b010000010010010010000010,
	0b010000010010010010010000,
	0b010000010010010010010010,
	0b010010000000000000000000,
	0b010010000000000000000010,
	0b010010000000000000010000,
	0b010010000000000000010010,
	0b010010000000000010000000,
	0b010010000000000010000010,
	0b010010000000000010010000,
	0b010010000000000010010010,
	0b010010000000010000000000,
	0b010010000000010000000010,
	0b010010000000010000010000,
	0b010010000000010000010010,
	0b010010000000010010000000,
	0b010010000000010010000010,
	0b010010000000010010010000,
	0b010010000000010010010010,
	0b010010000010000000000000,
	0b010010000010000000000010,
	0b010010000010000000010000,
	0b010010000010000000010010,
	0b010010000010000010000000,
	0b010010000010000010000010,
	0b010010000010000010010000,
	0b010010000010000010010010,
	0b010010000010010000000000,
	0b010010000010010000000010,
	0b010010000010010000010000,
	0b010010000010010000010010,
	0b010010000010010010000000,
	0b010010000010010010000010,
	0b010010000010010010010000,
	0b010010000010010010010010,
	0b010010010000000000000000,
	0b010010010000000000000010,
	0b010010010000000000010000,
	0b010010010000000000010010,
	0b010010010000000010000000,
	0b010010010000000010000010,
	0b010010010000000010010000,
	0b010010010000000010010010,
	0b010010010000010000000000,
	0b010010010000010000000010,
	0b010010010000010000010000,
	0b010010010000010000010010,
	0b010010010000010010000000,
	0b010010010000010010000010,
	0b010010010000010010010000,
	0b010010010000010010010010,
	0b010010010010000000000000,
	0b010010010010000000000010,
	0b010010010010000000010000,
	0b010010010010000000010010,
	0b010010010010000010000000,
	0b010010010010000010000010,
	0b010010010010000010010000,
	0b010010010010000010010010,
	0b010010010010010000000000,
	0b010010010010010000000010,
	0b010010010010010000010000,
	0b010010010010010000010010,
	0b010010010010010010000000,
	0b010010010010010010000010,
	0b010010010010010010010000,
	0b010010010010010010010010
};

const unsigned int zorder_z[256] = {
	0b000000000000000000000000,
	0b000000000000000000000100,
	0b000000000000000000100000,
	0b000000000000000000100100,
	0b000000000000000100000000,
	0b000000000000000100000100,
	0b000000000000000100100000,
	0b000000000000000100100100,
	0b000000000000100000000000,
	0b000000000000100000000100,
	0b000000000000100000100000,
	0b000000000000100000100100,
	0b000000000000100100000000,
	0b000000000000100100000100,
	0b000000000000100100100000,
	0b000000000000100100100100,
	0b000000000100000000000000,
	0b000000000100000000000100,
	0b000000000100000000100000,
	0b000000000100000000100100,
	0b000000000100000100000000,
	0b000000000100000100000100,
	0b000000000100000100100000,
	0b000000000100000100100100,
	0b000000000100100000000000,
	0b000000000100100000000100,
	0b000000000100100000100000,
	0b000000000100100000100100,
	0b000000000100100100000000,
	0b000000000100100100000100,
	0b000000000100100100100000,
	0b000000000100100100100100,
	0b000000100000000000000000,
	0b000000100000000000000100,
	0b000000100000000000100000,
	0b000000100000000000100100,
	0b000000100000000100000000,
	0b000000100000000100000100,
	0b000000100000000100100000,
	0b000000100000000100100100,
	0b000000100000100000000000,
	0b000000100000100000000100,
	0b000000100000100000100000,
	0b000000100000100000100100,
	0b000000100000100100000000,
	0b000000100000100100000100,
	0b000000100000100100100000,
	0b000000100000100100100100,
	0b000000100100000000000000,
	0b000000100100000000000100,
	0b000000100100000000100000,
	0b000000100100000000100100,
	0b000000100100000100000000,
	0b000000100100000100000100,
	0b000000100100000100100000,
	0b000000100100000100100100,
	0b000000100100100000000000,
	0b000000100100100000000100,
	0b000000100100100000100000,
	0b000000100100100000100100,
	0b000000100100100100000000,
	0b000000100100100100000100,
	0b000000100100100100100000,
	0b000000100100100100100100,
	0b000100000000000000000000,
	0b000100000000000000000100,
	0b000100000000000000100000,
	0b000100000000000000100100,
	0b000100000000000100000000,
	0b000100000000000100000100,
	0b000100000000000100100000,
	0b000100000000000100100100,
	0b000100000000100000000000,
	0b000100000000100000000100,
	0b000100000000100000100000,
	0b000100000000100000100100,
	0b000100000000100100000000,
	0b000100000000100100000100,
	0b000100000000100100100000,
	0b000100000000100100100100,
	0b000100000100000000000000,
	0b000100000100000000000100,
	0b000100000100000000100000,
	0b000100000100000000100100,
	0b000100000100000100000000,
	0b000100000100000100000100,
	0b000100000100000100100000,
	0b000100000100000100100100,
	0b000100000100100000000000,
	0b000100000100100000000100,
	0b000100000100100000100000,
	0b000100000100100000100100,
	0b000100000100100100000000,
	0b000100000100100100000100,
	0b000100000100100100100000,
	0b000100000100100100100100,
	0b000100100000000000000000,
	0b000100100000000000000100,
	0b000100100000000000100000,
	0b000100100000000000100100,
	0b000100100000000100000000,
	0b000100100000000100000100,
	0b000100100000000100100000,
	0b000100100000000100100100,
	0b000100100000100000000000,
	0b000100100000100000000100,
	0b000100100000100000100000,
	0b000100100000100000100100,
	0b000100100000100100000000,
	0b000100100000100100000100,
	0b000100100000100100100000,
	0b000100100000100100100100,
	0b000100100100000000000000,
	0b000100100100000000000100,
	0b000100100100000000100000,
	0b000100100100000000100100,
	0b000100100100000100000000,
	0b000100100100000100000100,
	0b000100100100000100100000,
	0b000100100100000100100100,
	0b000100100100100000000000,
	0b000100100100100000000100,
	0b000100100100100000100000,
	0b000100100100100000100100,
	0b000100100100100100000000,
	0b000100100100100100000100,
	0b000100100100100100100000,
	0b000100100100100100100100,
	0b100000000000000000000000,
	0b100000000000000000000100,
	0b100000000000000000100000,
	0b100000000000000000100100,
	0b100000000000000100000000,
	0b100000000000000100000100,
	0b100000000000000100100000,
	0b100000000000000100100100,
	0b100000000000100000000000,
	0b100000000000100000000100,
	0b100000000000100000100000,
	0b100000000000100000100100,
	0b100000000000100100000000,
	0b100000000000100100000100,
	0b100000000000100100100000,
	0b100000000000100100100100,
	0b100000000100000000000000,
	0b100000000100000000000100,
	0b100000000100000000100000,
	0b100000000100000000100100,
	0b100000000100000100000000,
	0b100000000100000100000100,
	0b100000000100000100100000,
	0b100000000100000100100100,
	0b100000000100100000000000,
	0b100000000100100000000100,
	0b100000000100100000100000,
	0b100000000100100000100100,
	0b100000000100100100000000,
	0b100000000100100100000100,
	0b100000000100100100100000,
	0b100000000100100100100100,
	0b100000100000000000000000,
	0b100000100000000000000100,
	0b100000100000000000100000,
	0b100000100000000000100100,
	0b100000100000000100000000,
	0b100000100000000100000100,
	0b100000100000000100100000,
	0b100000100000000100100100,
	0b100000100000100000000000,
	0b100000100000100000000100,
	0b100000100000100000100000,
	0b100000100000100000100100,
	0b100000100000100100000000,
	0b100000100000100100000100,
	0b100000100000100100100000,
	0b100000100000100100100100,
	0b100000100100000000000000,
	0b100000100100000000000100,
	0b100000100100000000100000,
	0b100000100100000000100100,
	0b100000100100000100000000,
	0b100000100100000100000100,
	0b100000100100000100100000,
	0b100000100100000100100100,
	0b100000100100100000000000,
	0b100000100100100000000100,
	0b100000100100100000100000,
	0b100000100100100000100100,
	0b100000100100100100000000,
	0b100000100100100100000100,
	0b100000100100100100100000,
	0b100000100100100100100100,
	0b100100000000000000000000,
	0b100100000000000000000100,
	0b100100000000000000100000,
	0b100100000000000000100100,
	0b100100000000000100000000,
	0b100100000000000100000100,
	0b100100000000000100100000,
	0b100100000000000100100100,
	0b100100000000100000000000,
	0b100100000000100000000100,
	0b100100000000100000100000,
	0b100100000000100000100100,
	0b100100000000100100000000,
	0b100100000000100100000100,
	0b100100000000100100100000,
	0b100100000000100100100100,
	0b100100000100000000000000,
	0b100100000100000000000100,
	0b100100000100000000100000,
	0b100100000100000000100100,
	0b100100000100000100000000,
	0b100100000100000100000100,
	0b100100000100000100100000,
	0b100100000100000100100100,
	0b100100000100100000000000,
	0b100100000100100000000100,
	0b100100000100100000100000,
	0b100100000100100000100100,
	0b100100000100100100000000,
	0b100100000100100100000100,
	0b100100000100100100100000,
	0b100100000100100100100100,
	0b100100100000000000000000,
	0b100100100000000000000100,
	0b100100100000000000100000,
	0b100100100000000000100100,
	0b100100100000000100000000,
	0b100100100000000100000100,
	0b100100100000000100100000,
	0b100100100000000100100100,
	0b100100100000100000000000,
	0b100100100000100000000100,
	0b100100100000100000100000,
	0b100100100000100000100100,
	0b100100100000100100000000,
	0b100100100000100100000100,
	0b100100100000100100100000,
	0b100100100000100100100100,
	0b100100100100000000000000,
	0b100100100100000000000100,
	0b100100100100000000100000,
	0b100100100100000000100100,
	0b100100100100000100000000,
	0b100100100100000100000100,
	0b100100100100000100100000,
	0b100100100100000100100100,
	0b100100100100100000000000,
	0b100100100100100000000100,
	0b100100100100100000100000,
	0b100100100100100000100100,
	0b100100100100100100000000,
	0b100100100100100100000100,
	0b100100100100100100100000,
	0b100100100100100100100100
};

const unsigned short zorder2_x[] = {
	0b0000000000000000,
	0b0000000000000001,
	0b0000000000000100,
	0b0000000000000101,
	0b0000000000010000,
	0b0000000000010001,
	0b0000000000010100,
	0b0000000000010101,
	0b0000000001000000,
	0b0000000001000001,
	0b0000000001000100,
	0b0000000001000101,
	0b0000000001010000,
	0b0000000001010001,
	0b0000000001010100,
	0b0000000001010101,
	0b0000000100000000,
	0b0000000100000001,
	0b0000000100000100,
	0b0000000100000101,
	0b0000000100010000,
	0b0000000100010001,
	0b0000000100010100,
	0b0000000100010101,
	0b0000000101000000,
	0b0000000101000001,
	0b0000000101000100,
	0b0000000101000101,
	0b0000000101010000,
	0b0000000101010001,
	0b0000000101010100,
	0b0000000101010101,
	0b0000010000000000,
	0b0000010000000001,
	0b0000010000000100,
	0b0000010000000101,
	0b0000010000010000,
	0b0000010000010001,
	0b0000010000010100,
	0b0000010000010101,
	0b0000010001000000,
	0b0000010001000001,
	0b0000010001000100,
	0b0000010001000101,
	0b0000010001010000,
	0b0000010001010001,
	0b0000010001010100,
	0b0000010001010101,
	0b0000010100000000,
	0b0000010100000001,
	0b0000010100000100,
	0b0000010100000101,
	0b0000010100010000,
	0b0000010100010001,
	0b0000010100010100,
	0b0000010100010101,
	0b0000010101000000,
	0b0000010101000001,
	0b0000010101000100,
	0b0000010101000101,
	0b0000010101010000,
	0b0000010101010001,
	0b0000010101010100,
	0b0000010101010101,
	0b0001000000000000,
	0b0001000000000001,
	0b0001000000000100,
	0b0001000000000101,
	0b0001000000010000,
	0b0001000000010001,
	0b0001000000010100,
	0b0001000000010101,
	0b0001000001000000,
	0b0001000001000001,
	0b0001000001000100,
	0b0001000001000101,
	0b0001000001010000,
	0b0001000001010001,
	0b0001000001010100,
	0b0001000001010101,
	0b0001000100000000,
	0b0001000100000001,
	0b0001000100000100,
	0b0001000100000101,
	0b0001000100010000,
	0b0001000100010001,
	0b0001000100010100,
	0b0001000100010101,
	0b0001000101000000,
	0b0001000101000001,
	0b0001000101000100,
	0b0001000101000101,
	0b0001000101010000,
	0b0001000101010001,
	0b0001000101010100,
	0b0001000101010101,
	0b0001010000000000,
	0b0001010000000001,
	0b0001010000000100,
	0b0001010000000101,
	0b0001010000010000,
	0b0001010000010001,
	0b0001010000010100,
	0b0001010000010101,
	0b0001010001000000,
	0b0001010001000001,
	0b0001010001000100,
	0b0001010001000101,
	0b0001010001010000,
	0b0001010001010001,
	0b0001010001010100,
	0b0001010001010101,
	0b0001010100000000,
	0b0001010100000001,
	0b0001010100000100,
	0b0001010100000101,
	0b0001010100010000,
	0b0001010100010001,
	0b0001010100010100,
	0b0001010100010101,
	0b0001010101000000,
	0b0001010101000001,
	0b0001010101000100,
	0b0001010101000101,
	0b0001010101010000,
	0b0001010101010001,
	0b0001010101010100,
	0b0001010101010101,
	0b0100000000000000,
	0b0100000000000001,
	0b0100000000000100,
	0b0100000000000101,
	0b0100000000010000,
	0b0100000000010001,
	0b0100000000010100,
	0b0100000000010101,
	0b0100000001000000,
	0b0100000001000001,
	0b0100000001000100,
	0b0100000001000101,
	0b0100000001010000,
	0b0100000001010001,
	0b0100000001010100,
	0b0100000001010101,
	0b0100000100000000,
	0b0100000100000001,
	0b0100000100000100,
	0b0100000100000101,
	0b0100000100010000,
	0b0100000100010001,
	0b0100000100010100,
	0b0100000100010101,
	0b0100000101000000,
	0b0100000101000001,
	0b0100000101000100,
	0b0100000101000101,
	0b0100000101010000,
	0b0100000101010001,
	0b0100000101010100,
	0b0100000101010101,
	0b0100010000000000,
	0b0100010000000001,
	0b0100010000000100,
	0b0100010000000101,
	0b0100010000010000,
	0b0100010000010001,
	0b0100010000010100,
	0b0100010000010101,
	0b0100010001000000,
	0b0100010001000001,
	0b0100010001000100,
	0b0100010001000101,
	0b0100010001010000,
	0b0100010001010001,
	0b0100010001010100,
	0b0100010001010101,
	0b0100010100000000,
	0b0100010100000001,
	0b0100010100000100,
	0b0100010100000101,
	0b0100010100010000,
	0b0100010100010001,
	0b0100010100010100,
	0b0100010100010101,
	0b0100010101000000,
	0b0100010101000001,
	0b0100010101000100,
	0b0100010101000101,
	0b0100010101010000,
	0b0100010101010001,
	0b0100010101010100,
	0b0100010101010101,
	0b0101000000000000,
	0b0101000000000001,
	0b0101000000000100,
	0b0101000000000101,
	0b0101000000010000,
	0b0101000000010001,
	0b0101000000010100,
	0b0101000000010101,
	0b0101000001000000,
	0b0101000001000001,
	0b0101000001000100,
	0b0101000001000101,
	0b0101000001010000,
	0b0101000001010001,
	0b0101000001010100,
	0b0101000001010101,
	0b0101000100000000,
	0b0101000100000001,
	0b0101000100000100,
	0b0101000100000101,
	0b0101000100010000,
	0b0101000100010001,
	0b0101000100010100,
	0b0101000100010101,
	0b0101000101000000,
	0b0101000101000001,
	0b0101000101000100,
	0b0101000101000101,
	0b0101000101010000,
	0b0101000101010001,
	0b0101000101010100,
	0b0101000101010101,
	0b0101010000000000,
	0b0101010000000001,
	0b0101010000000100,
	0b0101010000000101,
	0b0101010000010000,
	0b0101010000010001,
	0b0101010000010100,
	0b0101010000010101,
	0b0101010001000000,
	0b0101010001000001,
	0b0101010001000100,
	0b0101010001000101,
	0b0101010001010000,
	0b0101010001010001,
	0b0101010001010100,
	0b0101010001010101,
	0b0101010100000000,
	0b0101010100000001,
	0b0101010100000100,
	0b0101010100000101,
	0b0101010100010000,
	0b0101010100010001,
	0b0101010100010100,
	0b0101010100010101,
	0b0101010101000000,
	0b0101010101000001,
	0b0101010101000100,
	0b0101010101000101,
	0b0101010101010000,
	0b0101010101010001,
	0b0101010101010100,
	0b0101010101010101
};

const unsigned short zorder2_y[] = {
	0b0000000000000000,
	0b0000000000000010,
	0b0000000000001000,
	0b0000000000001010,
	0b0000000000100000,
	0b0000000000100010,
	0b0000000000101000,
	0b0000000000101010,
	0b0000000010000000,
	0b0000000010000010,
	0b0000000010001000,
	0b0000000010001010,
	0b0000000010100000,
	0b0000000010100010,
	0b0000000010101000,
	0b0000000010101010,
	0b0000001000000000,
	0b0000001000000010,
	0b0000001000001000,
	0b0000001000001010,
	0b0000001000100000,
	0b0000001000100010,
	0b0000001000101000,
	0b0000001000101010,
	0b0000001010000000,
	0b0000001010000010,
	0b0000001010001000,
	0b0000001010001010,
	0b0000001010100000,
	0b0000001010100010,
	0b0000001010101000,
	0b0000001010101010,
	0b0000100000000000,
	0b0000100000000010,
	0b0000100000001000,
	0b0000100000001010,
	0b0000100000100000,
	0b0000100000100010,
	0b0000100000101000,
	0b0000100000101010,
	0b0000100010000000,
	0b0000100010000010,
	0b0000100010001000,
	0b0000100010001010,
	0b0000100010100000,
	0b0000100010100010,
	0b0000100010101000,
	0b0000100010101010,
	0b0000101000000000,
	0b0000101000000010,
	0b0000101000001000,
	0b0000101000001010,
	0b0000101000100000,
	0b0000101000100010,
	0b0000101000101000,
	0b0000101000101010,
	0b0000101010000000,
	0b0000101010000010,
	0b0000101010001000,
	0b0000101010001010,
	0b0000101010100000,
	0b0000101010100010,
	0b0000101010101000,
	0b0000101010101010,
	0b0010000000000000,
	0b0010000000000010,
	0b0010000000001000,
	0b0010000000001010,
	0b0010000000100000,
	0b0010000000100010,
	0b0010000000101000,
	0b0010000000101010,
	0b0010000010000000,
	0b0010000010000010,
	0b0010000010001000,
	0b0010000010001010,
	0b0010000010100000,
	0b0010000010100010,
	0b0010000010101000,
	0b0010000010101010,
	0b0010001000000000,
	0b0010001000000010,
	0b0010001000001000,
	0b0010001000001010,
	0b0010001000100000,
	0b0010001000100010,
	0b0010001000101000,
	0b0010001000101010,
	0b0010001010000000,
	0b0010001010000010,
	0b0010001010001000,
	0b0010001010001010,
	0b0010001010100000,
	0b0010001010100010,
	0b0010001010101000,
	0b0010001010101010,
	0b0010100000000000,
	0b0010100000000010,
	0b0010100000001000,
	0b0010100000001010,
	0b0010100000100000,
	0b0010100000100010,
	0b0010100000101000,
	0b0010100000101010,
	0b0010100010000000,
	0b0010100010000010,
	0b0010100010001000,
	0b0010100010001010,
	0b0010100010100000,
	0b0010100010100010,
	0b0010100010101000,
	0b0010100010101010,
	0b0010101000000000,
	0b0010101000000010,
	0b0010101000001000,
	0b0010101000001010,
	0b0010101000100000,
	0b0010101000100010,
	0b0010101000101000,
	0b0010101000101010,
	0b0010101010000000,
	0b0010101010000010,
	0b0010101010001000,
	0b0010101010001010,
	0b0010101010100000,
	0b0010101010100010,
	0b0010101010101000,
	0b0010101010101010,
	0b1000000000000000,
	0b1000000000000010,
	0b1000000000001000,
	0b1000000000001010,
	0b1000000000100000,
	0b1000000000100010,
	0b1000000000101000,
	0b1000000000101010,
	0b1000000010000000,
	0b1000000010000010,
	0b1000000010001000,
	0b1000000010001010,
	0b1000000010100000,
	0b1000000010100010,
	0b1000000010101000,
	0b1000000010101010,
	0b1000001000000000,
	0b1000001000000010,
	0b1000001000001000,
	0b1000001000001010,
	0b1000001000100000,
	0b1000001000100010,
	0b1000001000101000,
	0b1000001000101010,
	0b1000001010000000,
	0b1000001010000010,
	0b1000001010001000,
	0b1000001010001010,
	0b1000001010100000,
	0b1000001010100010,
	0b1000001010101000,
	0b1000001010101010,
	0b1000100000000000,
	0b1000100000000010,
	0b1000100000001000,
	0b1000100000001010,
	0b1000100000100000,
	0b1000100000100010,
	0b1000100000101000,
	0b1000100000101010,
	0b1000100010000000,
	0b1000100010000010,
	0b1000100010001000,
	0b1000100010001010,
	0b1000100010100000,
	0b1000100010100010,
	0b1000100010101000,
	0b1000100010101010,
	0b1000101000000000,
	0b1000101000000010,
	0b1000101000001000,
	0b1000101000001010,
	0b1000101000100000,
	0b1000101000100010,
	0b1000101000101000,
	0b1000101000101010,
	0b1000101010000000,
	0b1000101010000010,
	0b1000101010001000,
	0b1000101010001010,
	0b1000101010100000,
	0b1000101010100010,
	0b1000101010101000,
	0b1000101010101010,
	0b1010000000000000,
	0b1010000000000010,
	0b1010000000001000,
	0b1010000000001010,
	0b1010000000100000,
	0b1010000000100010,
	0b1010000000101000,
	0b1010000000101010,
	0b1010000010000000,
	0b1010000010000010,
	0b1010000010001000,
	0b1010000010001010,
	0b1010000010100000,
	0b1010000010100010,
	0b1010000010101000,
	0b1010000010101010,
	0b1010001000000000,
	0b1010001000000010,
	0b1010001000001000,
	0b1010001000001010,
	0b1010001000100000,
	0b1010001000100010,
	0b1010001000101000,
	0b1010001000101010,
	0b1010001010000000,
	0b1010001010000010,
	0b1010001010001000,
	0b1010001010001010,
	0b1010001010100000,
	0b1010001010100010,
	0b1010001010101000,
	0b1010001010101010,
	0b1010100000000000,
	0b1010100000000010,
	0b1010100000001000,
	0b1010100000001010,
	0b1010100000100000,
	0b1010100000100010,
	0b1010100000101000,
	0b1010100000101010,
	0b1010100010000000,
	0b1010100010000010,
	0b1010100010001000,
	0b1010100010001010,
	0b1010100010100000,
	0b1010100010100010,
	0b1010100010101000,
	0b1010100010101010,
	0b1010101000000000,
	0b1010101000000010,
	0b1010101000001000,
	0b1010101000001010,
	0b1010101000100000,
	0b1010101000100010,
	0b1010101000101000,
	0b1010101000101010,
	0b1010101010000000,
	0b1010101010000010,
	0b1010101010001000,
	0b1010101010001010,
	0b1010101010100000,
	0b1010101010100010,
	0b1010101010101000,
	0b1010101010101010
};

inline unsigned int zorder(char x, char y, char z) {
	return zorder_x[x] | zorder_y[y] | zorder_z[z];
}

inline unsigned short zorder(char x, char y) {
	return zorder2_x[x] | zorder2_y[y];
}
