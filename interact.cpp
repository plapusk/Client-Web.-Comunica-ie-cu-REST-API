#include "interact.hpp"

bool Interact::isCookieActive() { // is loged in
	return cookie.length() > 0;
}

bool Interact::hasToken() { // has entered library
	return token.length() > 0;
}

void Interact::reg_user() {
	string user, pass; // read creditentials
	getline(cin, user, '\n');
	printf("user = ");
	getline(cin, user, '\n');
	printf("password = ");
	getline(cin, pass, '\n');

	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		exit(-1);

	json reg_data = { // create json for register
		{ "username", user },
		{ "password", pass }
	};

	string reg_string = reg_data.dump();
	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_post_request(HOST, REGISTER, "application/json", false, NULL, reg_string.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);

	int code = getCode(response);

	switch (code) { // check for errors
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

	// close the fd
	close_connection(fd);
}

void Interact::login() {
	string user, pass; // read creditentials
	getline(cin, user, '\n');
	printf("user = ");
	getline(cin, user, '\n');
	printf("password = ");
	getline(cin, pass, '\n');

	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		exit(-1);

	json log_data = { // create json for login
		{ "username", user },
		{ "password", pass }
	};

	string reg_string = log_data.dump();
	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_post_request(HOST, LOGIN, "application/json", false, NULL, reg_string.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ]; // copy the response
	strcpy(copy, response);

	int code = getCode(response);

	switch (code) {  // check for errors
		case CODE_SUC1:
			get_new_cookie(copy); // get the new cookie from the server response
			printf("Success %s\n", user.c_str());
			break;

		case CODE_SUC2:
			get_new_cookie(copy); // get the new cookie from the server response
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

	// close the fd
	close_connection(fd);
}

void Interact::enter_library() {
	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);

	char **cookies = (char**)malloc(1 * sizeof(char*));
	cookies[0] = (char *)malloc(BUFSIZ);
	strcpy(cookies[0], cookie.c_str()); // copy the current cookie into a char ** for skel

	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_get_request((char *)HOST, ACCESS, NULL, false, NULL, cookies, 1);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ]; // copy the response
	strcpy(copy, response);
	int code = getCode(response);

	// convert the response of the server into a json
	json jsonVal = json::parse(basic_extract_json_response(copy));

	switch (code) {  // check for errors
		case CODE_SUC1:
			token = "Bearer " + jsonVal["token"].get<string>(); // get the new token from the json we just parsed
			printf("Success, Token: %s\n", token.c_str());
			break;

		default:
			printf("error\n");
			break;
	}

	free(cookies[0]); // free dinamyc memory
	free(cookies);

	// close the fd
	close_connection(fd);
}

void Interact::add_book() {
	string title, author, genre, publisher, page_count; // read book info
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
	if (page_nr <= 0) {
		printf("Give a proper number\n");
		return;
	}

	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);

	json book = { // create json for book
		{ "title", title },
		{ "author", author },
		{ "genre", genre },
		{ "publisher", publisher },
		{ "page_count", page_nr }
	};

	string payload = book.dump();
	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_post_request(HOST, ADD_BOOK, "application/json", true, token.c_str(), payload.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	int code = getCode(response);

	switch (code) { // check for errors
		case CODE_SUC1:
			printf("Success\n");
			break;

		default:
			printf("error\n");
			break;
	}

	// close the fd
	close_connection(fd);
}

void Interact::get_book() {
	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	int id; // read id
	printf("id = ");
	scanf("%d", &id);

	string book_path = string(ADD_BOOK) + string("/") + to_string(id); // getting the path of the book to the id

	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_get_request(HOST, book_path.c_str(), NULL, true, token.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ]; // copy the response
	strcpy(copy, response);

	int code = getCode(response);

	switch (code) { // check for errors
		case CODE_SUC1:
			printf("%s\n", basic_extract_json_response(copy)); // print the book information
			break;

		default:
			printf("error\n");
			break;
	}

	// close the fd
	close_connection(fd);
}

void Interact::get_books() {
	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_get_request(HOST, ADD_BOOK, NULL, true, token.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char copy[BUFSIZ]; // copy the response
	strcpy(copy, response);

	int code = getCode(response);
	char *bookList;
	switch (code) { // check for errors
		case CODE_SUC1:
			bookList = basic_extract_json_response(copy); // get the list of all books
			if (bookList == NULL) { // check we actually have books
				printf("No books\n");
				break;
			}
			printf("[%s\n", bookList);
			break;

		default:
			printf("error\n");
			break;
	}

	// close the fd
	close_connection(fd);
}

void Interact::delete_book() {
	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);
	int id; // read id
	printf("id = ");
	scanf("%d", &id);

	string book_path = string(ADD_BOOK) + string("/") + to_string(id); // getting the path of the book to the id
	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_delete_request(HOST, book_path.c_str(), NULL, true, token.c_str(), NULL, 0);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);

	int code = getCode(response);

	switch (code) { // check for errors
		case CODE_SUC1:
			printf("Deleted\n");
			break;

		default:
			printf("error\n");
			break;
	}

	// close the fd
	close_connection(fd);
}

void Interact::logout() {
	// open the fd
	fd = open_connection((char *)HOST, PORT, AF_INET, SOCK_STREAM, 0);

	char **cookies = (char**)malloc(1 * sizeof(char*));
	cookies[0] = (char *)malloc(BUFSIZ);
	strcpy(cookies[0], cookie.c_str()); // copy the current cookie into a char ** for skel

	// creating the specific message, sending it to server then getting the repsonse
	char *message = compute_get_request(HOST, LOGOUT, NULL, false, NULL, cookies, 1);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);

	int code = getCode(response);
	switch (code) { // check for errors
		case CODE_SUC1:
			printf("Succes get out\n");
			// empty token and cookie (logout and exit library)
			token.clear();
			cookie.clear();
			break;

		default:
			printf("error\n");
			break;
	}

	free(cookies[0]); // free dinamyc memory
	free(cookies);

	// close the fd
	close_connection(fd);
}

void Interact::get_new_cookie(char *buf) {
	char *p = strstr(buf, "Set-Cookie: "); // find Set-cookie
	p = strtok(p, " :;");
	p = strtok(NULL, " ;");
	cookie = p; // get what is after the :
}


int Interact::getCode(char *buf) { // get the error code from the response
	char *p = strtok(buf, " "); // get the number of the end of this buffer
	while(p != NULL) {
		p = strtok(NULL, " ");
		if ((*p) >= '1' && (*p) <= '9' ) // check if it s a number
			return atoi(p); // convert string to int
	}
	return -1; // if it's not a number we get an error
}