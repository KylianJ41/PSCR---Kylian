#include <iostream>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    // make an array with numbers from 0 to 9
    int tab[10];
    for (int i = 0; i < 10; i++)
    {
        tab[i] = i;
    }
    
    for (size_t i = 9; i >= 0; i--)
    {
        if (tab[i] - tab[i - 1] != 1)
        {
            std::cout << "probleme !";
        }
    }
    
    return 0;
}
