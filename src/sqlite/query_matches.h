#pragma once

#include "../common.h"

#include "../sqlite/sqlite_init_dbs.h"

struct query_matches {
	size_t matches;
	int32_t *ids;
};

void add_query_match(struct query_matches *qmatches, const char *id);
struct query_matches *players_in_room(const int32_t r_id);
struct query_matches *init_query(void);
