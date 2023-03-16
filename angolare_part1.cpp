#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "measures.hpp"

int main()
{
    // Configurazioni
    std::vector<std::array<Acquisition, 2>> v;
    Measure diameter{};
    std::cout << "Diametro del perno e incertezza = ";
    std::cin >> diameter;
    for (int i{0}; i < 2; ++i)
    { // acquisisce i dati per le due configurazioni
        // nota: i dati aquisiti più volte devono essere pssati già mediati
        std::cout<<"\n\n---------------\nACQUISIZIONE CONFIGURAZIONE MASSA "<< i+1<<"\n";
        
        Measure m{};
        std::cout << "Massa e incertezza " << i + 1<<" =";
        std::cin >> m;
        std::array<Acquisition, 2> test;
        for (int j{0}; j < 2; ++j)
        {   Measure a{};
            std::cout << "\nAccelerazione angolare conf. "<< ((j==0)?"iniziale":"finale")<<" =";
            std::cin >> a;

            Acquisition e{m, a, diameter};
            test[j] = e;
        }
        v.push_back(test);
    }
    int k{1};
    std::for_each(v.begin(),v.end(),[&k](std::array<Acquisition,2> setup){//per ogni coppia di acquisizione
        //calcola I e il rapporto
        auto I_i{setup[0].inertia()};
        auto I_f{setup[1].inertia()};
        Measure r{I_i/I_f};
        std::cout<<"Rapporto massa "<< k<<": "<<r<<"\n";
        ++k;
    });
}
