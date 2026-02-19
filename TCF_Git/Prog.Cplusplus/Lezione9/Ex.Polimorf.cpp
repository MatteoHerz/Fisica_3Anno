#include <iostream>
#include <string>
using namespace std;

class Particle{
    public :
        string m_name; 
        double m_mass;
        
        Particle(string n, double mass): m_name(n), m_mass(mass){}
        
        virtual ~Particle() {}; 

        virtual void printCharge () = 0;

        virtual void printState() const { cout<<"\n-- PARTICLE STATE --\nParticle: "<< m_name 
                                              <<"  Particle mass: "<< m_mass <<endl; }
};

class Lepton : public Particle {
    public:
        double spin = 1./2.;

        Lepton(string n, double mass) : Particle(n,mass) {}
        ~Lepton() {}; 

        void printState() const override {
            cout<<"\n-- LEPTON STATE --\nLepton type: "<< m_name 
                <<"  Lepton mass: "<< m_mass
                <<"  Lepton spin: "<< spin<<endl;
        }
};

class Electron : public Lepton {
    public:
        Electron() : Lepton("Electron", 9.11e-31) {}
        ~Electron() {}; 

        void printState() const override {
            cout<<"\n-- ELECTRON STATE --\nParticle group: Leptons\nElectron mass: "<< m_mass
                <<"Kg\nElectron spin: "<<spin<<endl;
        }

        void printCharge() override {
            cout << "Electron charge: -1\n"<<endl; 
        };
};

class Muon : public Lepton {
    public:
        Muon() : Lepton("Muon", 1.88e-18) {}
        ~Muon() {}; 

        void printState() const override {
            cout<<"\n-- MUON STATE --\nParticle group: Leptons\nMuon mass: "<< m_mass
                <<"Kg\nMuon spin: "
                <<spin<<endl;
        }

        void printCharge() override {
            cout << "Muon charge: -1\n"<<endl; 
        }; 
};

int main(){

    Particle *part1 = new Electron();

    Particle *part2 = new Muon();    

    part1->printState();

    part1->printCharge();

    part2->printState();    

    part2->printCharge();

    delete part1; 
    delete part2; 
    return 0; 
}