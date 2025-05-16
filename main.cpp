#include <iostream>
#include <string>

#ifndef SALES_DATA_H
#define SALES_DATA_H

// unchanged from chapter 14 except for added friend declaration for hash
class Sales_data {
    friend class std::hash<Sales_data>;
    friend std::ostream &operator<<(std::ostream&, const Sales_data&);
    friend std::istream &operator>>(std::istream&, Sales_data&);
    friend bool operator==(const Sales_data &, const Sales_data &);
    friend std::ostream &print(std::ostream&, const Sales_data&);
    friend std::istream &read(std::istream&, Sales_data&);
public:
    // constructors
    Sales_data() = default;
    Sales_data(const std::string &s): bookNo(s) { }
    Sales_data(const std::string &s, unsigned n, double p):
        bookNo(s), units_sold(n), revenue(p * n) { }
    Sales_data(std::istream &);

    std::string isbn() const { return bookNo; }
    Sales_data& operator+=(const Sales_data&);

private:
    double avg_price() const;

    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};

namespace std {
template <>
struct hash<Sales_data> {
    typedef size_t result_type;
    typedef Sales_data argument_type;
    size_t operator()(const Sales_data& s) const {
        return hash<std::string>()(s.isbn()) ^
               hash<unsigned>()(s.units_sold) ^
               hash<double>()(s.revenue);
    }
};
}

inline bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs) {
    return lhs.isbn() < rhs.isbn();
}

inline bool operator==(const Sales_data &lhs, const Sales_data &rhs) {
    return lhs.isbn() == rhs.isbn() &&
           lhs.units_sold == rhs.units_sold &&
           lhs.revenue == rhs.revenue;
}

inline bool operator!=(const Sales_data &lhs, const Sales_data &rhs) {
    return !(lhs == rhs);
}

Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
Sales_data operator+(const Sales_data&, const Sales_data&);
std::ostream &operator<<(std::ostream&, const Sales_data&);
std::istream &operator>>(std::istream&, Sales_data&);

#endif // SALES_DATA_H

// === Function Definitions ===

Sales_data::Sales_data(std::istream &is) {
    read(is, *this);
}

Sales_data& Sales_data::operator+=(const Sales_data &rhs) {
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}

double Sales_data::avg_price() const {
    return units_sold ? revenue / units_sold : 0;
}

std::istream &read(std::istream &is, Sales_data &item) {
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}

std::ostream &print(std::ostream &os, const Sales_data &item) {
    os << item.isbn() << " " << item.units_sold << " "
       << item.revenue << " " << item.avg_price();
    return os;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs) {
    Sales_data sum = lhs;
    sum += rhs;
    return sum;
}

Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs) {
    Sales_data result = lhs;
    result += rhs;
    return result;
}

std::ostream &operator<<(std::ostream &os, const Sales_data &item) {
    return print(os, item);
}

std::istream &operator>>(std::istream &is, Sales_data &item) {
    return read(is, item);
}

// === Main Program ===

int main()

{

 Sales_data item1, item2;

 std::cin >> item1 >> item2;

 // first check that item1 and item2 represent the same book

 if (item1.isbn() == item2.isbn()) {

   std::cout << item1 + item2 << std::endl;

   return 0; // indicate success

 } else {

   std::cerr << "Data must refer to same ISBN"

   << std::endl;

   return -1; // indicate failure

 }

}    
