#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "interact.hpp"
#include <bits/stdc++.h>
#include "nlohmann/json.hpp"
using namespace std;

int main(int argc, const char** argv) {
	char buf[BUFSIZ];
	bool isDone = false;
	Interact in;

	while (!isDone) {
		printf("> ");
		scanf(" %s", buf); // reads command
		if (!strcmp(buf, "exit")) {
			isDone  = true; // end loop
		} else if (!strcmp(buf, "register")) {
			in.reg_user(); // try to register user
		} else if (!strcmp(buf, "login")) {
			in.login(); // try to login user
		} else if (!strcmp(buf, "enter_library")) {
			if (!in.isCookieActive()) { // check if we are logged in
				printf("Login first\n");
				continue;
			}
			in.enter_library(); // try to enter library
		} else if (!strcmp(buf, "add_book")) {
			if (!in.hasToken()) { // check if we entered the library
				printf("Enter library first\n");
				continue;
			}
			in.add_book(); // try to add book
		} else if (!strcmp(buf, "get_books")) {
			if (!in.hasToken()) { // check if we entered the library
				printf("Enter library first\n");
				continue;
			}
			in.get_books(); // try to print all books
		} else if (!strcmp(buf, "get_book")) {
			if (!in.hasToken()) { // check if we entered the library
				printf("Enter library first\n");
				continue;
			}
			in.get_book(); // try to get a book
		} else if (!strcmp(buf, "delete_book")) {
			if (!in.hasToken()) { // check if we entered the library
				printf("Enter library first\n");
				continue;
			}
			in.delete_book(); // try to delete a book
		} else if (!strcmp(buf, "logout")) {
			if (!in.isCookieActive()) { // check if we are logged in
				printf("Login first\n");
				continue;
			}
			in.logout(); // try to login
		} else {
			printf("Incorrect command!\n"); // typed wrong command
		}
	}
	printf("Closed\n");
	return 0;
}