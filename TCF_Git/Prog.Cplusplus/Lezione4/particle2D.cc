#include <iostream>

#include "particle2D.h"

particle2D::particle2D (float x0, float y0, float vx0, float vy0, float m):

  m_x (x0), //sto dando al data-member x il valore x0 ("m_" per indicare che si tratta di un data member)
  
  m_y (y0),
  
  m_vx (vx0),
  
  m_vy (vy0),
  
  m_mass (m)

  {
  
  }

  particle2D & particle2D::operator= (const particle2D & orig)
  {
    m_x = orig.m_x ;
    
    m_y = orig.m_y ;
    
    m_vx = orig.m_vx ;
    
    m_vy = orig.m_vy ;
    
    m_mass = orig.m_mass ;
    
    return *this;
  }

  void particle2D::update(double timedelay)
  {
    m_x += m_vx * timedelay;
    m_y += m_vy * timedelay;
  }

  particle2D particle2D::operator+ (const particle2D & merger)
  {
    particle2D sum(m_x,m_y,m_vx,m_vy,m_mass);
    
    sum.m_mass += merger.m_mass ;
    
    sum.m_vx = ( m_mass * m_vx + merger.m_mass * merger.m_vx ) / sum.m_mass ;
    
    sum.m_vy = ( m_mass * m_vy + merger.m_mass * merger.m_vy ) / sum.m_mass ;
    
    return sum;
  }

  particle2D::~particle2D()
  {
  }

  void particle2D::printState()
  {
    std::cout << "\nPosition: (" << m_x << ", " << m_y << ")\n";
    
    std::cout << "Velocity: (" << m_vx << ", " << m_vy << ")\n";
    
    std::cout << "Mass: " << m_mass << " kg\n";
  }