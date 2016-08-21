#include "complex_test.h"
#include <complex.h>
#include <utility>
#include <algorithm>

CPPUNIT_TEST_SUITE_REGISTRATION( ComplexTest );

using blieng::Complex;

void ComplexTest::basic()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(5, 4);

    CPPUNIT_ASSERT(c1 != c2);
    CPPUNIT_ASSERT(c3 == c2);

    CPPUNIT_ASSERT(c1 == Complex(1,1));
    CPPUNIT_ASSERT(c1 != Complex(1,2));
}

void ComplexTest::plus()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    Complex res = c1.plus(c2);
    CPPUNIT_ASSERT(res == Complex(6, 5));

    CPPUNIT_ASSERT(c3.plus(c2) == Complex(15, 11));
    CPPUNIT_ASSERT(c3+c2 == Complex(15, 11));

    CPPUNIT_ASSERT(c2 + 2 == Complex(7, 4));
}

void ComplexTest::minus()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    CPPUNIT_ASSERT(c2.minus(c1) == Complex(4, 3));
    CPPUNIT_ASSERT(c1.minus(c2) == Complex(-4, -3));

    CPPUNIT_ASSERT(c3.minus(c2) == Complex(5, 3));
    CPPUNIT_ASSERT(c3 - c2 == Complex(5, 3));
    CPPUNIT_ASSERT(c2 - 2 == Complex(3, 4));

}

void ComplexTest::mul()
{
    Complex c1(2.5, 2);
    Complex c2(5, 4);
    Complex c3(10, 7);

    CPPUNIT_ASSERT(c2.mul(c1) == Complex(4.5, 20));
    CPPUNIT_ASSERT(c1.mul(c2) == Complex(4.5, 20));

    CPPUNIT_ASSERT(c3.mul(c2) == Complex(22, 75));
    CPPUNIT_ASSERT(c3 * c2 == Complex(22, 75));
    CPPUNIT_ASSERT(c2 * 2 == Complex(10, 8));
}

void ComplexTest::conjugate()
{
    Complex c1(2.5, 2);
    Complex c2(5, 4);
    Complex c3(10, 7);
    Complex c4(1, -2);

    CPPUNIT_ASSERT(c1.conjugate() == Complex(2.5, -2));
    CPPUNIT_ASSERT(c2.conjugate() == Complex(5, -4));
    CPPUNIT_ASSERT(c3.conjugate() == Complex(10, -7));
    CPPUNIT_ASSERT(c4.conjugate() == Complex(1, 2));
}

void ComplexTest::assign()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    Complex b;
    b = c1;
    CPPUNIT_ASSERT(b == c1);
    CPPUNIT_ASSERT(b != c2);
    b = c2;
    CPPUNIT_ASSERT(b != c1);
    CPPUNIT_ASSERT(b == c2);
}

//blieng::Complex cmplxI(blieng::Complex s1, blieng::Complex s2, blieng::Complex a, blieng::Complex b)
template<typename T> std::vector<blieng::Complex> tcmplxI(std::pair<blieng::Complex,blieng::Complex> s, T a, blieng::Complex b=blieng::Complex(0,1))
{
    std::vector<blieng::Complex> res;
    blieng::Complex c = s.first;
    blieng::Complex d = s.second;

    d = d - c;
    c = c - a;
    /*
    std::cout << "III1 " << d.to_string() << "\n";
    std::cout << "III1 " << c.to_string() << "\n";
    */

    double e = (d * b.conjugate()).imag();
    double e2 = e * e; //(d * b.conjugate()).imag();
    /*
    std::cout << "III2 " << e << "\n";
    std::cout << "III2 " << e2 << "\n";
    */
    if (! (e * (0 <= e2 && e2 <= e*e))) {
        return res;
    }

    double t2 = (d * c.conjugate()).imag();
    blieng::Complex t3 = b / e;
    blieng::Complex t4 = t3 * t2;

    /*
    std::cout << "III3 " << a.to_string() << "\n";
    std::cout << "III3 " << t2 << "\n";
    std::cout << "III3 " << t3.to_string() << "\n";
    std::cout << "III3 " << t4.to_string() << "\n";
    */
    double v = e * (b * c.conjugate()).imag();
    ///e * (0 <= (b*c.conjugate()).imag * e <= e*e)

    if (e * (0 <= v && v <= e2)) {
        res.push_back(t4 + a);
    }
    return res;
    //return e * (0 <= e2 && e2 <= e*e) && (a + t4);
}

/*
std::vector<std::pair<blieng::Complex,blieng::Complex>> expandComplex(std::vector<blieng::Complex> inp)
{
    std::vector<std::pair<blieng::Complex,blieng::Complex>>  res;
    for (unsigned int i = 0; i < inp.size(); ++i) {
        blieng::Complex a = inp[i];
        blieng::Complex b;
        if (i == inp.size() - 1) {
            b = inp[0];
        } else {
            b = inp[i + 1];
        }
        res.push_back(std::pair<blieng::Complex,blieng::Complex>(a, b));
    }
    return res;
}
*/

template<typename T> std::vector<std::pair<T,T>> expandTypes(std::vector<T> inp)
{
    std::vector<std::pair<T,T>>  res;
    for (unsigned int i = 0; i < inp.size(); ++i) {
        T a = inp[i];
        T b;
        if (i == inp.size() - 1) {
            b = inp[0];
        } else {
            b = inp[i + 1];
        }
        res.push_back(std::pair<T,T>(a, b));
    }
    return res;
}

void ComplexTest::calc()
{
    //(5,1), (1,5)), (1,1), ((4,4)
    //std::cout << "III " << tcmplxI(std::pair<blieng::Complex,blieng::Complex>(Complex(5,1), Complex(1,5)), Complex(1,1), Complex(4,4)).to_string() << "\n";

    std::vector<blieng::Complex> inp;
    //[1+2j,3+4j,5+6j,8+4j,8+2j
    inp.push_back(blieng::Complex(1,2));
    inp.push_back(blieng::Complex(3,4));
    inp.push_back(blieng::Complex(5,6));
    inp.push_back(blieng::Complex(8,4));
    inp.push_back(blieng::Complex(8,2));

    /*
    for (auto i : tcmplxI<double>(std::pair<blieng::Complex,blieng::Complex>(blieng::Complex(5,6),blieng::Complex(8,4)),1)) {
        std::cout << " ** II " <<i.to_string() << "\n";
    }
    return;
    */
    //std::vector<std::pair<blieng::Complex,blieng::Complex>> P = expandComplex(inp);
    //std::vector<std::pair<blieng::Complex,blieng::Complex>> P = expandTypes<blieng::Complex>(inp);
    auto P = expandTypes<blieng::Complex>(inp);
    std::vector<double> i_real;
    for (auto ab : P) {
        blieng::Complex a = ab.first;
        blieng::Complex b = ab.second;
        for (auto e : P) {
            for (auto i : tcmplxI<blieng::Complex>(e, a, b-a)) {
                i_real.push_back(i.real());
            }
            //i_real.push_back(cmplxI<blieng::Complex>(e, a, b-a).real());
        }
    }
    std::sort(i_real.begin(), i_real.end());

    auto lr = expandTypes<double>(i_real);
    lr.pop_back();
    /*
    for (auto nn : lr) {
        std::cout << "IRR " << nn.first << " " << nn.second << "\n";
    }
    */

    std::vector<std::pair<double,double>> bt;
    double ar = 0;
    for (auto tlr : lr) {
        std::vector<double> ij_imag;
        double l = tlr.first;
        double r = tlr.second;
        //std::cout << "LR " << l << " " << r <<"\n";
        for (auto e : P) {
            //std::cout << " E " << e.first.to_string() << " , " << e.second.to_string() <<"\n";
            for (auto i : tcmplxI<double>(e, l)) {
                //std::cout << " i " << i.to_string() <<"\n";
                for (auto j : tcmplxI<double>(e, r)) {
                    //std::cout << " j " << j.to_string() <<"\n";
                    //std::cout << "   *** " << i.to_string() << " " << j.to_string() << "\n";
                    ij_imag.push_back( ((i + j).conjugate()).imag() );
                }
            }
        }
        std::sort(ij_imag.begin(), ij_imag.end());
        auto ij_pairs = expandTypes<double>(ij_imag);
        bool tock = true;
        for (auto i : ij_pairs) {
            if (tock) {
                //bt.push_back(i);
                double b = i.first;
                double t = i.second;
                ar += (t-b) * (r - l) /2;
            }
            tock ^= true;
        }
    }

    /*
    std::sort(ij_imag.begin(), ij_imag.end());
    auto ij_pairs = expandTypes<double>(ij_imag);

    std::vector<std::pair<double,double>> bt;
    bool tock = true;
    for (auto i : ij_pairs) {
        if (tock) {
            bt.push_back(i);
        }
        tock ^= true;
    }
    */
    /*
    std::vector<std::pair<double,double>> bt;
    bool p = false;
    double m = 0;
    for (auto i : ij_imag) {
        if (p) {
            if (i < m)
            bt.push_back(std::pair<double,double>(i,m));
            else
            bt.push_back(std::pair<double,double>(m,i));
        } else  m = i;
        p ^= true;
    }
    */

    /*
    std::vector<double> s;
    for (auto tbt : bt) {
        double b = tbt.first;
        double t = tbt.second;
        std::cout << " TB " << t << " " << b<< " " << (t-b) << "\n";
        s.push_back(t-b);
    }
    std::reverse(s.begin(), s.end());
    std::reverse(bt.begin(), bt.end());
    for (auto tlr : lr) {
        double l = tlr.first;
        double r = tlr.second;
        auto tbt = bt.back();
        bt.pop_back();
        double b = tbt.first;
        double t = tbt.second;
        //double a = s.back();
        //s.pop_back();
        ar += (t-b) * (r - l) /2;
        std::cout << " RL " << t << " " << b << " " << r << " " << l << "\n";
        //std::cout << "*V " << ar << " " << (t-b) << " " << (r-l) << " " << (t-b) * (r - l) /2<< "\n";
    }
    */
    std::cout << "ARR " << ar<<"\n";

}
