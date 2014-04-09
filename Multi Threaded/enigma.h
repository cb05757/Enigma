#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <iostream>
#ifndef ENIGMA_H
#define ENIGMA_H

class enigma { 
 private:
    int letters[94]; // holds all useable ASCII letters, create and populate on inialization
    std::string password; // set the machines encryption/decryption password
    int currentPasswordIndex; // current password letter were encrypting with 
    int passwordLength; // length of the password string
    void swapRotor(void); // swaps the rotor
    void rotateRotor(int); // rotates the rotor
    
  public:
    enigma (std::string newPassword) {
      password = newPassword; // sets the "machines" password
      currentPasswordIndex = 0; //sets to the first letter in the "array" aka string
      passwordLength = password.length(); // sets to the length of your password
      int i;//first letter in letters array
      int j;
      	for(i = 33,j=0; i< 127; i++,j++){  //populate the array letters with 94 values
         letters[j] = i; // letters 33-126     
      	}
    }

    int process(int,bool); // int charIn,bool encrypt

// Deconstructor
    ~enigma () { // deletes the object and all data fields
      password.clear(); // emptys the password string
    } 
		
};

#endif
