#pragma once

#include <vector>
#include <string>

namespace pr
{

	class concat
	{
		// TODO : attributs stockant ref ou pointeurs vers les constituants v1,v2
		std::vector<std::string> &v1;
		std::vector<std::string> &v2;

	public:
		concat(std::vector<std::string> &v1, std::vector<std::string> &v2) : v1(v1), v2(v2);

		class iterator
		{
			// Type d'itérateur fourni par STL vector, se comporte comme un pointeur
			// vers l'élément actuellement parcouru dans v1 ou v2
			std::vector<std::string>::iterator ite;
			concat &context;

		public:
			// TODO : signature du constructeur
			iterator(const std::vector<std::string>::iterator &ite, concat &context) : ite(ite), context(context) {};
			// TODO : contrat itérateur
			std::string &operator*()
			{
				return *ite;
			}
			iterator &operator++()
			{
				++ite;
				if (ite == context.v1.end())
					ite = context.v2.begin();
			}
			bool operator!=(const iterator &other) const
			{
				return ite != other.ite;
			}
		};

		iterator begin()
		{
			return iterator(v1.begin(), *this);
		}
		iterator end()
		{
			return iterator(v2.end(), *this);
		}
	};

}
