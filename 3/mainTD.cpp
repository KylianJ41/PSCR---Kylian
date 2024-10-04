#include <iostream>
#include <algorithm>
#include <string>
#include "Vector.h"
#include "List.h"

int main()
{
    pr::Vector<int> cont;

    cont.push_back(10);
    cont.push_back(20);
    cont.push_back(30);
    cont.push_back(40);
    cont.push_back(50);
    /*
    std::cout << "Elements du vecteur (boucle range-for) : ";
    for (const int &i : cont)
        std::cout << i << " ";

    std::cout << std::endl;*/

    pr::List<int> myList;

    myList.push_back(10);
    myList.push_back(20);
    myList.push_back(30);
    myList.push_back(40);
    myList.push_back(50);
    /*
    std::cout << "Elements de la liste (boucle range-for) : ";
    for (const int &i : myList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "boucle for avec ecriture (non const) : ";
    for (int &i : myList)
    {
        i *= 2;
        std::cout << i << " ";
    }
    std::cout << std::endl;
    */
    /*

     int target = 30;
     pr::List<int>::Iterator it_list = pr::find(myList.begin(), myList.end(), target);

     if (it_list != myList.end())
     {
         std::cout << "Element trouve : " << *it << std::endl;
     }
     else
     {
         std::cout << "Element non trouve" << std::endl;
     }

     pr::Vector<int>::Iterator it_vec = pr::find(cont.begin(), cont.end(), target);
     */
    pr::Vector<std::string> str_vec;

    str_vec.push_back("apple");
    str_vec.push_back("banana");
    str_vec.push_back("dog");
    str_vec.push_back("date");
    /*
    auto isLenThree = [](const std::string &s) -> bool
    {
        return s.length() == 3;
    };

    pr::Vector<std::string>::iterator it = std::find_if(str_vec.begin(), str_vec.end(), isLenThree);

    if (it != str_vec.end())
    {
        std::cout << "Element trouve : " << *it << std::endl;
    }
    else
    {
        std::cout << "Element non trouve" << std::endl;
    }
    */

    size_t n;
    std::cout << "Entrez la longueur des chaînes à rechecher : ";
    std::cin >> n;

    auto isLenN = [n](const std::string &s) -> bool
    {
        return s.length() == n;
    };

    pr::Vector<std::string>::const_iterator it = std::find_if(str_vec.begin(), str_vec.end(), isLenN);

    if (it != str_vec.end())
    {
        std::cout << "Element trouve : " << *it << std::endl;
    }
    else
    {
        std::cout << "Element non trouve" << std::endl;
    }

    return 0;
}