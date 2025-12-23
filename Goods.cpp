#include "pch.h"
#include "Goods.h"

Goods::Goods(CString name, CString belongs, CString des, int amount, double price) {
	this->name = name;
	this->belongs = belongs;
	this->des = des;
	this->amount = amount;
	this->price = price;
}
