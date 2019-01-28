#include "debug.h"
Inspector::Inspector()
{
	
}

void Inspector::set_name(std::string name)
{
	_name = name;
}

void Inspector::display()
{
	Draw::Filled_Rectangle(Vector2(5, 5), Vector2(300, 60 + 16 * _elem.size()), rgba_color(1, 1, 1, 0.1));
	Draw::Rectangle(Vector2(5, 5), Vector2(300, 60 + 16 * _elem.size()), rgb_color(1, 1, 1));
	Draw::Debug_Text(Vector2(10, 10), "Inspecting : " + _name, 14, rgb_color(1, 1, 1));
	for (int i = 0; i < _elem.size(); i++)
	{
		
		switch (_elem[i].type)
		{
			case TYPENAME::INT:
				Draw::Debug_Text(Vector2(10, 50 + 16 * i), "int", 14, rgb_color(0.3, 1, 1));
				Draw::Debug_Text(Vector2(70, 50 + 16 * i), _elem[i].name, 14, rgb_color(1, 0.6, 0.8));
				Draw::Debug_Text(Vector2(200, 50 + 16 * i), std::to_string(*((int*)_elem[i].data)), 14, rgb_color(1, 1, 1));
				break;
			case TYPENAME::DOUBLE:
				Draw::Debug_Text(Vector2(10, 50 + 16 * i), "double", 14, rgb_color(0.3, 1, 1));
				Draw::Debug_Text(Vector2(70, 50 + 16 * i), _elem[i].name, 14, rgb_color(1, 0.6, 0.8));
				Draw::Debug_Text(Vector2(200, 50 + 16 * i), std::to_string(*((double*)_elem[i].data)), 14, rgb_color(1, 1, 1));
				break;
		}
	}
	
}

void Inspector::add_element(int type, std::string name, void *data)
{
	inspector_elem elem;
	elem.data = data; 
	elem.type = type;
	elem.name = name;

	_elem.push_back(elem);
}

void Inspector::clear()
{
	_elem.clear();
}

