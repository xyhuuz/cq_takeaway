#pragma once
#include <iostream>
#include <map>
#include "afxwin.h"
#include "Goods.h"

class User
{
private:
	
public:
	CString username;
	CString psw;
	BOOL usertype;
	CString phone;
	CString idcode;
	std::map<CString, Goods*> goods;


	User(CString username, CString psw, int usertype, CString phone, CString idcode);
	User(CString username, CString psw, int usertype);
	User();
	~User();

	bool find (CString username, CString psw,BOOL usertype);
	bool find (CString username);
	bool find(CString username, CString idcode);
};

extern User* user;



/*
CREATE DATABASE wmex;
USE wmex;

CREATE TABLE users (
	id INT AUTO_INCREMENT PRIMARY KEY,
	username VARCHAR(50) NOT NULL UNIQUE,
	password VARCHAR(50) NOT NULL,
	usertype TINYINT NOT NULL,
	phone VARCHAR(20),
	idcode VARCHAR(20)
);
*/