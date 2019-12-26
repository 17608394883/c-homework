#include "stdafx.h"
#include "entity.h"
#include <vector>

#pragma once
char* G2U(const char* gb2312);

BOOL  sqlOperation(char *SQL, sqlite3 **conn);

BOOL QueryUser(sqlite3 *db, std::string sql, std::vector<User> &u);

BOOL QueryDep(sqlite3 *db, std::string sql, std::vector<Department> &d);

std::string searchDepNameById(int depId);