#include "pch.h"
#include "User.h"

User::User(CString username, CString psw, int usertype, CString phone, CString idcode) {
	this->username = username;
	this->psw = psw;
	this->usertype = usertype;
	this->phone = phone;
	this->idcode = idcode;
}

User::User(CString username, CString psw, int usertype) {
	this->username = username;
	this->psw = psw;
	this->usertype = usertype;
}

User::User(){}

User::~User(){}

bool User::find(CString username1, CString psw1, BOOL usertype1)
{
	if (this->username == username1 && this->psw == psw1 && this->usertype == usertype1)
		return true;
	return false;
}

bool User::find(CString username)
{
	if (this->username == username)
		return true;
	return false;
}

bool User::find(CString username, CString idcode)
{
	if (this->username == username && this->idcode == idcode)
		return true;
	return false;
}
