#include <time.h> // used for program running time calculations
#include"enigma.h"

// Chris Beyer

// -e encrypts -d decrypts 

int main(int argc, char **argv) 
{
  time_t actStart,end; // Actual time 
	actStart = time(NULL);
	clock_t start = clock(); // CPU Time

   FILE *infile; // file your reading in 
   FILE *outfile; //output file

   int c; // character being read in
   std::string password; // word or phrase to encrypt or decrypt the data with
   bool encrypt = false; // has to be set to true by program

// sees how many arguments are being supplied
    if (argc != 5) { // means there arnt 1 + 4 arguments supplied in the command line
        fprintf(stderr, "Usage: %s -e or -d password infile.txt outfile.txt\n", argv[0]);
        return 0; // exits the program
    }

// Fopen commands
    if (!(infile = fopen(argv[3], "rt"))) { // read textfile
        perror(argv[3]); // sends error msg if file doesnt exist
        return 0; //exits program
    }

    if (!(outfile = fopen(argv[4], "wb+x"))) { // writes but doesnt overwrite existing files 
        perror(argv[4]);
        return 0; //exits program
    }

		switch( argv[1][0] ) // argv[1][0] checks the first char in the second command argument
       {
       case '-':
          switch( argv[1][1] ) // argv[1][1] checks the second char in the second command
           {
            case 'e':
            // set encrypt to true to encrypt data
            printf("Encrypting data...\n");
            encrypt = true;
            break;
            case 'd':
            // set encrypt to false to decrypt data
            printf("Decrypting data...\n");
            break;
            default:
            printf("Invalid parameter: %s\n", argv[1]);
            printf("-e Encrypts and -d Decrypts");
            return 0; // exit program if invalid parameters are entered
            break;
           }
       break;
       default: // if first character is not '-'
        printf("Invalid parameter: %s\n", argv[1]);
        printf("-e Encrypts and -d Decrypts");
        return 0;
       break;
		}

    password = argv[2]; // puts the password in the password varable



//---------------Enigma Object--------------------------

	enigma machine(password); // create object for encrypting and decrypting

//------------------------------------------------------------
// Line counter
 int totalCharacters = 0;
 int currentCharacter = 0;
    while ((c = fgetc(infile)) != EOF){
        if (c==10){
            ++totalCharacters;
        }
    }
  rewind(infile);

//-------------------------------------------------------------
  while ((c = fgetc(infile)) != EOF) { // read in characters as long as they are available       
    if(c==10){ // if character is a line break
   // Prints it straight out to the file without any modification   
      fputc(c, outfile);
    }else{ // otherwise send it down here, this is where all the main encryption will happen
      c = machine.process(c,encrypt); // encrypts every character except for \n      
      fputc(c, outfile); // output to the file once were done processing
      currentCharacter++; // sets what the current character is on
    }
  }//end of while loop
    
  fclose(infile); // close infile when finished
  fclose(outfile); // close outfile when finished

	clock_t finished = clock(); // finished time
  end = time(NULL);
  printf("All Finished!\n");
	printf("%g CPU Seconds\n",(double) (finished-start)/CLOCKS_PER_SEC); // prints program cpu time
  printf("Actual Running Time %g Seconds\n", difftime(end, actStart)); // prints actual running time
		if(encrypt){
  		printf("Number of lines encrypted %i\n", totalCharacters);
		}else{
  		printf("Number of lines decrypted %i\n", totalCharacters);
		}
  return 0;
}

