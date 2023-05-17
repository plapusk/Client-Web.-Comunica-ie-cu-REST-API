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
		scanf(" %s", buf);
		if (!strcmp(buf, "exit")) {
			in.closing();
			isDone  = true;
		} else if (!strcmp(buf, "register")) {
			in.reg_user();
		} else if (!strcmp(buf, "login")) {
			in.login();
		} else if (!strcmp(buf, "enter_library")) {
			if (!in.isCookieActive()) {
				printf("Login first\n");
				continue;
			}
			in.enter_library();
		} else if (!strcmp(buf, "add_book")) {
			if (!in.hasToken()) {
				printf("Enter library first\n");
				continue;
			}
			in.add_book();
		} else if (!strcmp(buf, "get_books")) {
			if (!in.hasToken()) {
				printf("Enter library first\n");
				continue;
			}
			in.get_books();
		} else if (!strcmp(buf, "get_book")) {
			if (!in.hasToken()) {
				printf("Enter library first\n");
				continue;
			}
			in.get_book();
		} else if (!strcmp(buf, "delete_book")) {
			if (!in.hasToken()) {
				printf("Enter library first\n");
				continue;
			}
			in.delete_book();
		} else if (!strcmp(buf, "logout")) {
			if (!in.isCookieActive()) {
				printf("Login first\n");
				continue;
			}
			in.logout();
		} else {
			printf("Learn to write!\n");
		}
	}
	printf("Closed\n");
	return 0;
}