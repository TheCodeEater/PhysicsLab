#include <iostream>
#include <numeric>
#include <vector>
#include <array>

struct Statistics {
  double mean{};
  double sigma{};
  double mean_err{};
  double median{};
};

class Sample {
 private:
  std::vector<double> entries_{};

 public:
  explicit Sample(std::vector<double>& list): entries_(list){}

  template<unsigned long n>
  explicit Sample(std::array<double,n>& list){
    entries_.insert(entries_.begin(),list.begin(),list.end());
  }

  void add(double x) { entries_.push_back(x); }
  auto size() const { return entries_.size(); }
  bool remove(double val) {
    auto it = std::find(entries_.begin(), entries_.end(), val);
    if (it != entries_.end()) {
      entries_.erase(it);
      return true;
    } else {
      return false;
    }
  }

  Statistics statistics() const {
    int N_ = entries_.size();
    if (N_ == 0) {
      throw std::runtime_error(
          "Cannot calculate parameters from empty sample!");
    }
    // calcola i parametri sum e sum_2
    double sum_{};
    double sum2_{};
    std::for_each(entries_.begin(), entries_.end(),
                  [&sum_, &sum2_](double val) {  // calcola le somme
                    sum_ += val;
                    sum2_ += val * val;
                  });

    const double mean{sum_ / N_};
    const double sigma{sqrt(
        N_ / (N_ - 1) *
        (sum2_ / N_ - mean * mean))};  // x quadro medio meno x medio quadro
    const double mean_err = {sigma / sqrt(N_)};

    // sort vector
    auto entries{entries_};
    std::sort(entries.begin(), entries.end());
    const double median{N_%2==0? (entries.at(N_ / 2 - 1) + entries.at(N_ / 2)) / 2. :entries.at((N_ + 1) / 2 - 1)};

    std::cout << N_ / 2 << "\n" << N_ << "\n";



    return {mean, sigma, mean_err, median};
  }

  const auto& entries() const{
    return entries_;
  }

  Sample& operator+=(const Sample & rhs){
    entries_.insert(entries_.end(),rhs.entries().begin(),rhs.entries().end());
    return *this;
  }
};

Sample operator+(Sample const& l, Sample const& r){
    Sample s{l};//copia di sample
    s+=r;
    return s;
}

auto operator<<(std::ostream& os, Statistics s){
  os<<"\n-----OUTPUT DATI STATISTICI-----\n";
  os<<"Media: "<<s.mean<<"\n";
  os<<"Sigma campionaria: "<<s.sigma<<"\n";
  os<<"Sigma media: "<<s.mean_err<<"\n";
  os<<"\n-----FINE OUTPUT DATI-----\n";
}