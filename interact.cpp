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
	printf("user=");
	getline(cin, user, '\n');
	printf("password=");
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
	printf("user=");
	getline(cin, user, '\n');
	printf("password=");
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
	char *copy;
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

	char **cookies;
	strcpy(cookies[0], cookie.c_str());

	char *message = compute_get_request((char *)HOST, ACCESS, NULL, false, NULL, cookies, 1);
	send_to_server(fd, message);
	char *response = receive_from_server(fd);
	char *copy;
	strcpy(copy, response);
	int code = getCode(response);

	json jsonVal = json::parse(basic_extract_json_response(copy));

	switch (code) {
		case CODE_SUC1:
			token = "Bearer " + jsonVal["token"].get<string>();
			printf("Success\n");
			break;

		default:
			printf("error\n");
			break;
	}

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