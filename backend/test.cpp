#include <iostream>
#include <string>
#include <cstdio>
#include "sqlite3.h"

static void run(sqlite3* db, const char* sql) {
    char* err = nullptr;
    int rc = sqlite3_exec(db, sql, 0, 0, &err);
    std::cout << "exec rc=" << rc << " " << (err ? err : "ok") << "\n";
    sqlite3_free(err);
}

int main() {
    remove("t.db");
    sqlite3* db = nullptr;
    std::cout << "open rc=" << sqlite3_open("t.db", &db) << "\n";

    run(db, "PRAGMA foreign_keys = ON");
    run(db, "CREATE TABLE students (id TEXT PRIMARY KEY, name TEXT NOT NULL, "
            "email TEXT NOT NULL UNIQUE, salt TEXT NOT NULL, "
            "password_hash TEXT NOT NULL, joined TEXT NOT NULL)");
    run(db, "CREATE TABLE profiles (student_id TEXT PRIMARY KEY "
            "REFERENCES students(id) ON DELETE CASCADE, "
            "university TEXT NOT NULL DEFAULT '', degree TEXT NOT NULL DEFAULT '', "
            "interests TEXT NOT NULL DEFAULT '', bio TEXT NOT NULL DEFAULT '')");

    sqlite3_stmt* st = nullptr;
    int rc = sqlite3_prepare_v2(db,
        "INSERT INTO students (id, name, email, salt, password_hash, joined) "
        "VALUES (?, ?, ?, ?, ?, ?)", -1, &st, 0);
    std::cout << "prepare rc=" << rc << " msg=" << sqlite3_errmsg(db) << "\n";
}