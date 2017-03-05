#pragma once

#pragma GCC system_header

#include "../common.h"

#include "explicit_bzero.h"
#include "timingsafe_bcmp.h"
#include "blf.h"

int bcrypt_newhash(const char *pass, int log_rounds, char *hash, size_t hashlen);
int bcrypt_checkpass(const char *pass, const char *goodhash);
char *bcrypt_gensalt(uint8_t log_rounds);
char *bcrypt(const char *pass, const char *salt);
