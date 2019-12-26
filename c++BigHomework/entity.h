#include "stdafx.h"
#pragma once
class User
{
public:
	User();
	User(std::string  username, std::string  phone, int depId);
	int uId;
	std::string username;
	std::string phone;
	int depId;
	std::string role;
	std::string password;

};

class Department
{
public:

	int depId;
	std::string depName;
	
};




