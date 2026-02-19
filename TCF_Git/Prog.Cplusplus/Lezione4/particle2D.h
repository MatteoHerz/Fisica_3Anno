#include <iostream>

class particle2D {

    public:
        
        particle2D (float x0, float y0, float vx0, float vy0, float m); // Constructor

        ~particle2D (); // destructor

        particle2D& operator=(const particle2D& orig); //overloading operator

        particle2D operator+(const particle2D& merger); //sum operator

        void printState(); // Method to print state

        void update(double timedelay); //updates position after dt

    private:

        float m_x, m_y;       // Position

        float m_vx, m_vy;     // Velocity

        float m_mass;       // Mass
};