#pragma once
#include <iostream>
#include <string>
#include "graphics.h"

namespace TYPENAME
{
	enum { INT, FLOAT, DOUBLE, CHAR };
	
}


typedef struct inspector_elem
{
	int type = 0;
	std::string name = "unnamed";
	void *data = nullptr;
} inspector_elem;

class Inspector
{
	public:
		Inspector();
		void set_name(std::string name);
		void add_element(int type, std::string name, void *data);
		void clear();
		void display();

	private:
		std::string _name = "unnamed";
		std::vector<inspector_elem> _elem;
};
