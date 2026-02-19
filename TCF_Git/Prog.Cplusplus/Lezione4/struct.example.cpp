#include <iostream>

struct Product {

    int id;         // product ID

    float price;    // price in dollars

    char category;  // category code (e.g., 'A', 'B', 'C')
};

int main() {
    
    // Declare and initialize a product
    
    Product p1;
    
    p1.id = 101;
    
    p1.price = 29.99f;
    
    p1.category = 'A';

    // Print product info
    
    std::cout << "Product ID: " << p1.id << std::endl;
    
    std::cout << "Price: $" << p1.price << std::endl;
    
    std::cout << "Category: " << p1.category << std::endl;

    Product p2 = {102, 19.98, 'C'};
    
    // Print product info
    
    std::cout << "Product ID: " << p2.id << std::endl;
    
    std::cout << "Price: $" << p2.price << std::endl;
    
    std::cout << "Category: " << p2.category << std::endl;

    return 0;
}