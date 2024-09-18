#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main()
{

	std::string abc = "abc";
	char *str = new char[3];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	// size_t i = 0;

	if (!strcmp(str, abc.c_str()))
	{
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front("test");

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	// FAUTE : comparaison entre signed (int) et unsigned (size_t)
	// for (i= list.size() - 1 ; i >= 0 ; i--) {
	for (int i = static_cast<int>(list.size()) - 1; i >= 0; i--)
	{
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// liberer les char de la chaine
	// FAUTE : suppression incorrecte de chaque char
	// for (char *cp = str ; *cp ; cp++) {
	//	delete cp;
	//}

	// et la chaine elle meme
	// FAUTE : il faut delete [] str car c'est un tableau
	// delete str;
	delete[] str;
}
