#include "helpers.hpp"
#include "requests.hpp"
#include "buffer.hpp"
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

#define HOST "34.254.242.81"
#define PORT 8080

#define REGISTER "/api/v1/tema/auth/register"
#define LOGIN "/api/v1/tema/auth/login"
#define ACCESS "/api/v1/tema/library/access"
#define ADD_BOOK "/api/v1/tema/library/books"
#define LOGOUT "/api/v1/tema/auth/logout"

#define CODE_SUC1 200
#define CODE_SUC2 201
#define CODE_BAD_REQUEST 400
#define CODE_TOO_MANY 429

class Interact{
public:
	bool isCookieActive();
	bool hasToken();
	void reg_user();
	void login();
	void get_new_cookie(char *buf);
	void enter_library();
	void add_book();
	void get_books();
	void get_book();
	void delete_book();
	void logout();

	int getCode(char *buf);
	void closing();
private:
	int fd;
	string token;
	string cookie;
};