#include <iostream>
#include <type_traits>

class Measure
{
    double val_{};
    double delta_{};

public:
    Measure() : val_{}, delta_{} {}
    explicit Measure(double value) : val_{value}, delta_{} {}
    Measure(double value, double uncertainty) : val_{value}, delta_{uncertainty} {}
    void setValue(double val)
    {
        val_ = val;
    }
    void setDelta(double d)
    {
        delta_ = d;
    }
    double getValue() const { return val_; }
    double getDelta() const { return delta_; }
    // operatori @=
    auto operator+=(Measure const &b)
    {
        val_ += b.getValue();
        delta_ += b.getValue();
    }
    auto operator-=(Measure const &b)
    {
        val_ -= b.getValue();
        delta_ += b.getValue();
    }
    auto operator*=(Measure const &b)
    {
        double prod{val_ * b.getValue()};                              // calcola dil prodotto
        delta_ = prod * (delta_ / val_ + b.getDelta() / b.getValue()); // calcola errore relativo, poi assoluto
        // assegno il valore dopo poiché mi serve conservare il valore iniziale per calcolare incertezza
        val_ = prod;
    }
    auto operator/=(Measure const &b)
    {
        double quot{val_ / b.getValue()};
        delta_ = quot * (delta_ / val_ + b.getDelta() / b.getValue());
        val_ = quot;
    }
    template <typename T>
    auto operator+=(T &k)
    {
        static_assert(std::is_arithmetic<T>());
        val_ += k;
    }
    template <typename T>
    auto operator-=(T &k)
    {
        static_assert(std::is_arithmetic<T>());
        val_ -= k;
    }
    template <typename T>
    auto operator*=(T &k)
    {
        static_assert(std::is_arithmetic<T>());
        val_ *= k;
        delta_ *= k;
    }
    template <typename T>
    auto operator/=(T &k)
    {
        static_assert(std::is_arithmetic<T>());
        val_ /= k;
        delta_ /= k;
    }
};

const Measure g{9.81, 0.01};

// operatori misura
auto operator+(Measure const &a, Measure const &b)
{
    Measure res{a};
    res += b;
    return a;
}
auto operator-(Measure const &a, Measure const &b)
{
    Measure res{a};
    res -= b;
    return a;
}
auto operator*(Measure const &a, Measure const &b)
{
    Measure res{a};
    res *= b;
    return res;
}
auto operator/(Measure const &a, Measure const &b)
{
    Measure res{a};
    res /= b;
    return res;
}
//OPERATORI CON LE COSTANTI
template <typename T>
auto operator+(Measure const& a, T k){
    static_assert(std::is_arithmetic<T>());
    Measure res{a};
    res+=k;
    return res;
}
template <typename T>
auto operator-(Measure const& a, T k){
    static_assert(std::is_arithmetic<T>());
    Measure res{a};
    res-=k;
    return res;
}
template <typename T>
auto operator*(Measure const& a, T k){
    static_assert(std::is_arithmetic<T>());
    Measure res{a};
    res*=k;
    return res;
}
template <typename T>
auto operator/(Measure const& a, T k){
    static_assert(std::is_arithmetic<T>());
    Measure res{a};
    res/=k;
    return res;
}
//OPERATORI CON LE COSTANTI - COMMUTATIVA
template <typename T>
auto operator+(T k, Measure const& b){
    static_assert(std::is_arithmetic<T>());
    Measure res{b};
    res+=k;
    return res;
}
template <typename T>
auto operator-(T k,Measure const& b){
    static_assert(std::is_arithmetic<T>());
    Measure res{b};
    res-=k;
    return res;
}
template<typename T>
auto operator*(T k,Measure const& b){
    static_assert(std::is_arithmetic<T>());
    Measure res{b};
    res*=k;
    return res;
}
template<typename T>
auto operator/(T k,Measure const& b){
    static_assert(std::is_arithmetic<T>());
    return Measure{k/b.getValue(),k/(b.getValue()*b.getValue())*b.getDelta()};
}


std::istream &operator>>(std::istream &is, Measure &m)
{
    double meas{};
    double delt{};
    is >> meas;
    is >> delt;
    m.setValue(meas);
    m.setDelta(delt);
    return is;
}
std::ostream &operator<<(std::ostream &os, Measure &m)
{
    os << m.getValue();
    os << "+-";
    os << m.getDelta();
    return os;
}

class Acquisition
{               // acquisizione dati prima parte
    Measure m_; // massa appesa
    Measure a_; // accelerazione angolare
    Measure d_; // diametro perno
public:
    Acquisition() = default;
    Acquisition(Measure m, Measure a, Measure d) : m_{m}, a_{a}, d_{d} {}
    Measure inertia()
    {
        return m_ * d_ * (2 * g - a_ * d_) / (4 * a_);
    }
};