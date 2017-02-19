#pragma once

#include "../common.h"

#include "../Map.h"
#include "../SqliteVars.h"

int callback(void *map, int argc, char **argv, char **azColName);
int32_t get_sqlite_rows_count(void);
void increment_sqlite_rows_count(void);
void set_sqlite_rows_count(const int32_t newval);
void reset_sqlite_rows_count(void);
