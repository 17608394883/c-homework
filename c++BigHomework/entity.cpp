#include "stdafx.h"

User::User(){}
User::User(std::string  username, std::string  phone, int depId)
{
	this->username = username;
	this->phone = phone;
	this->depId = depId;
}