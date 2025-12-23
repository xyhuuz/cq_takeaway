#pragma once
#include "afxwin.h"

class Goods
{
private:

public:
	CString name;
	CString belongs;
	CString des;
	int amount;
	double price;

	Goods(CString name, CString belongs, CString des, int amount, double price);
};

/*
CREATE TABLE IF NOT EXISTS goods (
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(255) NOT NULL,
	belongs VARCHAR(255),
	des TEXT,
	amount INT,
	price DECIMAL(10, 2)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
*/