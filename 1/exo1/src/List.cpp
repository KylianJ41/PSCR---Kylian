// FAUTE : manque le include de List.h
#include "List.h"

namespace pr
{

	// ******************* Chainon
	Chainon::Chainon(const std::string &data, Chainon *next) : data(data), next(next) {};

	// FAUTE : recursion infinie
	size_t Chainon::length()
	{
		size_t len = 1;
		if (next != nullptr)
		{
			len += next->length();
		}
		// return length();
		return len;
	}

	// FAUTE : il faut ajouter const
	// void Chainon::print(std::ostream &os)
	void Chainon::print(std::ostream &os) const
	{
		os << data;
		if (next != nullptr)
		{
			os << ", ";
			next->print(os); // new line to fix issue below
		}
		// FAUTE : accès potentiel à un pointeur null
		// next->print(os);
	}

	// ******************  List
	const std::string &List::operator[](size_t index) const
	{
		Chainon *it = tete;
		for (size_t i = 0; i < index; i++)
		{
			it = it->next;
		}
		return it->data;
	}

	void List::push_back(const std::string &val)
	{
		if (tete == nullptr)
		{
			tete = new Chainon(val);
		}
		else
		{
			Chainon *fin = tete;
			while (fin->next)
			{
				fin = fin->next;
			}
			fin->next = new Chainon(val);
		}
	}

	void List::push_front(const std::string &val)
	{
		tete = new Chainon(val, tete);
	}

	// FAUTE : def en dehors de la classe
	// bool empty()
	bool List::empty()
	{
		return tete == nullptr;
	}

	size_t List::size() const
	{
		if (tete == nullptr)
		{
			return 0;
		}
		else
		{
			return tete->length();
		}
	}

} // namespace pr

// FAUTE : namespace manquant pour l'opérateur
// std::ostream &operator<<(std::ostream &os, const pr::List &vec)
std::ostream &pr::operator<<(std::ostream &os, const pr::List &vec)
{
	os << "[";
	if (vec.tete != nullptr)
	{
		vec.tete->print(os);
	}
	os << "]";
	return os;
}
