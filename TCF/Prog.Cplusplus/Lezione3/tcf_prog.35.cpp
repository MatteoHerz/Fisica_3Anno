#include <iostream>

int main(){

	//Utilizzo degli ALIAS
	//Si tratta di una referenza ad una variabile tramite lo stesso indirizzo di memoria.
	//Mi permette di non dichiarare 2 varibili separate per uno stesso valore. 
	//Differenze con puntatori: non esistono null ALIAS (non può puntare a nulla); l'ALIAS una volta inizializzato ad una varibiale non può essere cambiato ad un'altra variabile; 
	
	int pippo = 12;

	int &pasticcio = pippo;

	std::cout << pippo << " equal to " << pasticcio << std::endl;

	pippo = 15;

	std::cout << pippo << " equal to " << pasticcio << std::endl;

}
