#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "measures.hpp"
#include <numeric> 

TEST_CASE("Testing uncertainty calculation"){
    Measure a{5,0.2};
    Measure b{2.5,0.1};
    auto res=a*b;
    Measure c{12.5,1};
    std::cout<<"RES: "<<res<<"\n";
    std::cout<<"C: "<<c<<"\n";
    SUBCASE("Sum"){
        CHECK(res.getValue()-c.getValue()==doctest::Approx(0));
        CHECK(res.getDelta()-c.getDelta()==doctest::Approx(0));
    }
}