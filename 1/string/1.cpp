#include <cstddef>
#include <iostream>
#include <cstring>
#include "pr_string_utils.h"
#include "string.h"

// q1 / q3
// ver 1
/*
size_t length(const char *str)
{
    size_t len = 0;
    for (; str[len]; len++)
        ;
    return len;
}
*/
size_t pr::length(const char *str)
{
    const char *end = str;
    while (*end)
        ++end;

    return end - str;
}
// q4
/*
char *newcopy(const char *str)
{
    char *new_str = new char[length(str) + 1];
    for (size_t i = 0; str[i]; i++)
        new_str[i] = str[i];

    return new_str;
}
*/
// q5 : malloc returns void* so we need to cast it to char*
// malloc returns a pointer to the allocated memory, or NULL if the request fails.
// new is type-safe and returns the correct type
// new returns a pointer to the allocated memory, or throws an exception if the request fails.

// q6
char *pr::newcopy(const char *str)
{
    size_t len = length(str);
    char *new_str = new char[len + 1];
    memcpy(new_str, str, len + 1);
    return new_str;
}

// q8
/*
Compile the main program file (exo1.cpp) to object file:
g++ -c -std=c++11 -Wall -g exo1.cpp -o exo1.o
Link the object file to create the executable:
g++ exo1.o -o exo1
Run the executable:
./exo1

Or in a single step :
g++ -std=c++11 -Wall -g exo1.cpp -o exo1
*/

// q11
/*
dans getalphabet une char array tab est remplie
Puis un objet de type String est créé pointant vers tab puis retourné

alors abc va hold un pointeur vers la même adresse memoire que l'objet String
Or tab, a la sortie de getalphabet est destroyed.
On se retrouve alors avec un dangling pointer
On aura donc un undefined behavior

*/

// q13
/*
le problème survient en ligne 9 : nous n'avons pas implémenté la surcharge
de l'opérateur = , cette ligne va donc faire un shallow copy,
copiant simplement le pointeur de abc vers def

le string "def" va causer une memory leak.

le cout va print "abc" deux fois.
*/

int main()
{

    // q7
    /*
    const char *str = "Hello, World!";
    const char *copy_str = pr::newcopy(str);

    std::cout << "original : " << str << std::endl;
    std::cout << "copy : " << copy_str << std::endl;

    delete[] copy_str;
    */

    pr::String str("Hello, World!");
    std::cout << str << std::endl;

    // create a regular string
    const char *abc = "yay";
    // create a String object
    str = abc;

    std::cout << str << std::endl;

    if (pr::compare("abc", "abc") == 0)
        std::cout << "equal" << std::endl;
    else
        std::cout << "not equal" << std::endl;

    if (pr::compare("abc", "def") == 0)
    {
        std::cout << "equal" << std::endl;
    }
    else
    {
        std::cout << "not equal" << std::endl;
    }

    if (abc < "def")
    {
        std::cout << "abc < def" << std::endl;
    }
    else
    {
        std::cout << "abc >= def" << std::endl;
    }

    return 0;
}