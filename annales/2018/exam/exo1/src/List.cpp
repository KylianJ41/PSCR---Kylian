// ERREUR : manque le include
#include "List.h"

namespace pr
{

	// ******************* Chainon
	Chainon::Chainon(const std::string &data, Chainon *next) : data(data), next(next) {};

	size_t Chainon::length()
	{
		size_t len = 1;
		if (next != nullptr)
		{
			len += next->length();
		}
		// Erreur : doit return la var len
		// return length();
		return len;
	}

	// ERREUR : manque le const
	// void Chainon::print(std::ostream &os)
	void Chainon::print(std::ostream &os) const
	{
		os << data;
		if (next != nullptr)
		{
			os << ", ";
			next->print(os);
		}
		// ERREUR : appel systématiquement sur next meme si NULL
		// next->print(os);
	}

	// ******************  List
	const std::string &List::operator[](size_t index) const
	{
		auto it = begin();
		for (size_t i = 0; i < index; i++)
		{
			++it;
		}
		return *it;
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

	// ERREUR : redifinition
	/*
	void List::push_front(const std::string &val)
	{
		tete = new Chainon(val, tete);
	}
	*/

	// ERREUR : manque le List::
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

	std::ostream &operator<<(std::ostream &os, const pr::List &vec)
	{
		os << "[";
		if (vec.tete != nullptr)
		{
			vec.tete->print(os);
		}
		os << "]";
		return os;
	}

} // namespace pr

// ERREUR : Hors du namespace
/*
std::ostream &operator<<(std::ostream &os, const pr::List &vec)
{
	os << "[";
	if (vec.tete != nullptr)
	{
		vec.tete->print(os);
	}
	os << "]";
	return os;
}
*/