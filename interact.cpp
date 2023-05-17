#include "interact.hpp"

bool Interact::isCookieActive() {
	return cookie.length() > 0;
}

bool Interact::hasToken() {
	return token.length() > 0;
}

void Interact::reg_user() {
	string user, pass;
	getline(cin, user, '\n');
	printf("user = ");
	getline(cin, user, '\n');
	printf("password = ");
	getline(cin, pass, '\n');

	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		exit(-1);

	json reg_data = {
		{ "username", user },
		{ "password", pass }
	};

	string reg_string = reg_data.dump();
	char *message = compute_post_request(HOST, REGISTER, "application/json", false, NULL, reg_string.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);

	int code = getCode(response);

	switch (code) {
		case CODE_SUC1:
			printf("Success %s\n", user.c_str());
			break;

		case CODE_SUC2:
			printf("Success %s\n", user.c_str());
			break;

		case CODE_BAD_REQUEST:
			printf("Bad request\n");
			break;

		case CODE_TOO_MANY:
			printf("Too many request\n");
			break;

		default:
			printf("error\n");
			break;
	}

	close_connection(fd);
	
}

void Interact::login() {
	string user, pass;
	getline(cin, user, '\n');
	printf("user = ");
	getline(cin, user, '\n');
	printf("password = ");
	getline(cin, pass, '\n');

	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		exit(-1);

	json log_data = {
		{ "username", user },
		{ "password", pass }
	};

	string reg_string = log_data.dump();
	char *message = compute_post_request(HOST, LOGIN, "application/json", false, NULL, reg_string.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ];
	strcpy(copy, response);

	int code = getCode(response);

	switch (code) {
		case CODE_SUC1:
			get_new_cookie(copy);
			printf("Success %s\n", user.c_str());
			break;

		case CODE_SUC2:
			get_new_cookie(copy);
			printf("Success %s\n", user.c_str());
			break;

		case CODE_BAD_REQUEST:
			printf("Bad request\n");
			break;

		case CODE_TOO_MANY:
			printf("Too many request\n");
			break;

		default:
			printf("error\n");
			break;
	}

	close_connection(fd);
}

void Interact::enter_library() {
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);

	char **cookies = (char**)malloc(1 * sizeof(char*));
	cookies[0] = (char *)malloc(BUFSIZ);
	strcpy(cookies[0], cookie.c_str());

	char *message = compute_get_request((char *)HOST, ACCESS, NULL, false, NULL, cookies, 1);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ];
	strcpy(copy, response);
	int code = getCode(response);

	json jsonVal = json::parse(basic_extract_json_response(copy));

	switch (code) {
		case CODE_SUC1:
			token = "Bearer " + jsonVal["token"].get<string>();
			printf("Success, Token: %s\n", token.c_str());
			break;

		default:
			printf("error\n");
			break;
	}

	free(cookies[0]);
	free(cookies);
	close_connection(fd);
}

void Interact::add_book() {
	string title, author, genre, publisher, page_count;
	getline(cin, title, '\n');
	cout << "Title = "; 
	getline(cin, title, '\n');
	cout << "Author = "; 
	getline(cin, author, '\n');
	cout << "Genre = "; 
	getline(cin, genre, '\n');
	cout << "Publisher = "; 
	getline(cin, publisher, '\n');
	cout << "Page count = ";
	getline(cin, page_count, '\n');
	int page_nr = atoi(page_count.c_str());
	if (page_nr < 0) {
		printf("Try positive\n");
		return;
	}

	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);

	json book = {
		{ "title", title },
		{ "author", author },
		{ "genre", genre },
		{ "publisher", publisher },
		{ "page_count", page_nr }
	};

	string payload = book.dump();
	char *message = compute_post_request(HOST, ADD_BOOK, "application/json", true, token.c_str(), payload.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	int code = getCode(response);

	switch (code) {
		case CODE_SUC1:
			printf("Success\n");
			break;

		default:
			printf("error\n");
			break;
	}

	close_connection(fd);
}

void Interact::get_book() {
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	int id;
	printf("id = ");
	scanf("%d", &id);

	string book_path = string(ADD_BOOK) + string("/") + to_string(id);

	char *message = compute_get_request(HOST, book_path.c_str(), NULL, true, token.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ];
	strcpy(copy, response);

	int code = getCode(response);

	switch (code) {
		case CODE_SUC1:
			printf("%s\n", basic_extract_json_response(copy));
			break;

		default:
			printf("error\n");
			break;
	}
	close_connection(fd);
}

void Interact::get_books() {
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	char *message = compute_get_request(HOST, ADD_BOOK, NULL, true, token.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ];
	strcpy(copy, response);

	int code = getCode(response);
	char *bookList;
	switch (code) {
		case CODE_SUC1:
			bookList = basic_extract_json_response(copy);
			if (bookList == NULL) {
				printf("No books\n");
				break;
			}
			printf("[%s\n", bookList);
			break;

		default:
			printf("error\n");
			break;
	}
	close_connection(fd);
}

void Interact::delete_book() {
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	int id;
	printf("id = ");
	scanf("%d", &id);

	string book_path = string(ADD_BOOK) + string("/") + to_string(id);
	char *message = compute_delete_request(HOST, book_path.c_str(), NULL, true, token.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);

	int code = getCode(response);

	switch (code) {
		case CODE_SUC1:
			printf("Deleted\n");
			break;

		default:
			printf("error\n");
			break;
	}
	close_connection(fd);
}

void Interact::logout() {
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);

	char **cookies = (char**)malloc(1 * sizeof(char*));
	cookies[0] = (char *)malloc(BUFSIZ);
	strcpy(cookies[0], cookie.c_str());

	char *message = compute_get_request(HOST, LOGOUT, NULL, false, NULL, cookies, 1);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);

	int code = getCode(response);
	switch (code) {
		case CODE_SUC1:
			printf("Succes get out\n");
			break;

		default:
			printf("error\n");
			break;
	}
	token.clear();
	cookie.clear();

	free(cookies[0]);
	free(cookies);
	close_connection(fd);
}

void Interact::get_new_cookie(char *buf) {
	char *p = strstr(buf, "Set-Cookie: ");
	p = strtok(p, " :;");
	p = strtok(NULL, " ");
	cookie = p;
	cookie.erase(cookie.length()-1);
}


int Interact::getCode(char *buf) {
	char *p = strtok(buf, " ");
	while(p != NULL) {
		p = strtok(NULL, " ");
		if ((*p) >= '1' && (*p) <= '9' )
			return atoi(p);
	}
	return -1;
}

void Interact::closing() {
	if (fd >= 0)
		close_connection(fd);
}