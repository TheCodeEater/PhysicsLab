#include "measures.hpp"
#include "statistics.hpp"
#include <fstream>
#include <string>
#include <array>
#include <algorithm>

const int n_meas{10};

int main(){
    std::string filename;
    std::cout<<"Inserire nome del file (con 10 misure): ";
    std::cin>>filename;
    std::ifstream f(filename);
    //carica di dati in un vettore. Nel file, ogni riga è una coppia
    std::array<double,n_meas> raw_data;
    
    //per semplicita, for da 10 misure
    std::generate(raw_data.begin(),raw_data.end(),[&f](){
        double w_i{};
        double w_f{};
        f>>w_i>>w_f;
        return w_f/w_i;
    });
    f.close();

    //crea campione
    Sample s(raw_data);
    //
    auto data=s.statistics();
    //adattamento a inc massima
    std::cout<<data;
    std::cout<<"Incertezza assoluta media (fattore copertura 3): "<<data.mean_err*3.<<"\n";
    return 0;
}