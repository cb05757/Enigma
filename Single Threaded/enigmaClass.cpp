#include"enigma.h"

	int enigma::process (int charIn,bool encrypt) {
		int charOut = 126; // sets as a ~ if there is a malfunction
		int rotorPosition = password[currentPasswordIndex];
		int shift = -1; // set to -1 because there cant be an array with a position of -1
		int i;
      for(i=0;i<94;i++){ // using the size of the letters array
    		if(letters[i] == password[currentPasswordIndex]){ // uses linear search
			  	shift = i; 
        	break;
      	}
      }

			if(shift==-1){//error code			
				printf("Error: Invalid character was used in the password\n");
    		//end program
			}

     	if(encrypt == true){
        //excutes if true
/* The letter were encrypting with will be password[currentPasswordIndex] , we treat password like an array
   then we use the letters array as our "rotor" and the above will be our rotor setting to
   use with password and currentPasswordIndex and passwordlength
*/
	//we then take our password letter and search for it in our letters array and when we find it, the index position + 1 is what we shift with

				if((charIn + shift) < 127){ // keep all values in the range of 33 to 126
					charOut = charIn + shift;
				}else{  // if it is greater than
					charOut = (charIn + shift)-94; 		
  			}

				if(charOut >126 || charOut <32){ // if char is not in the range then print and set it = to ~
					printf("Error: Invalid output character was replaced\n");
					printf("charIn %i\n",charIn);
					printf("shift %i\n",shift);
					printf("charOut %i\n",charOut);
					printf("\n");
					charOut = 126;// if the character is out of bounds then print ~ in its place
				}
				rotateRotor(1); // Rotates the rotor once
  			}else{ // decrypt

					if((charIn - shift) > 31){ // keep all values in the range of 33 to 126
						charOut = charIn - shift;
					}else{
						charOut = (charIn - shift) + 94;
					}
				rotateRotor(1);   
  			}
		swapRotor();
		return charOut; // output the encrypted char
	}

	void enigma::swapRotor(){
		if(currentPasswordIndex<passwordLength-1){
			currentPasswordIndex++;
		}else{
			currentPasswordIndex = 0;
		}
	}

	void enigma::rotateRotor(int rotations){ 
		int max = 126;
 		int min = 33;
		if(rotations > 0){ 
			password[currentPasswordIndex] = (password[currentPasswordIndex]+rotations) % (max+1); 
      	if(password[currentPasswordIndex]<min){
        	password[currentPasswordIndex] += min;
      	}
		}else{// if less than 33 aka the rotor is turned in the negative direction
			// not used in the current configuration
		}
	}


