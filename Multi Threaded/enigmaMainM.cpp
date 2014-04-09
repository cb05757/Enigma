
#include <time.h> // used for program running time calculations
#include <pthread.h>
#include"enigma.h"
#include <assert.h>

// thread 1 reads file
// thread 2 encrypts data 
// thread 3 output file

//Chris Beyer
//------------------------------Global Variables ---------
   FILE *infile; // file your reading in 
   FILE *fileout; //output file
   
	 // Variables and mutexes for exchange of data between threads
   int inCwork;
	 pthread_mutex_t inCwork_mutex;

   bool isEmpty1 = true;
	 pthread_mutex_t isEmpty1_mutex;
	 pthread_cond_t isEmpty1_cond;

   int workCout;
	 pthread_mutex_t workCout_mutex;

	 bool isEmpty2 = true;
	 pthread_mutex_t isEmpty2_mutex;
   pthread_cond_t isEmpty2_cond;

 	 int totalCharacters = 0; 
   int currentCharacter = 0;  

   std::string password; // word or phrase to encrypt or decrypt the data with
   bool encryptstate = false; // has to be set to true by program


//-----------------------------------------------------------

void * funcIn(void * arg) { // when finished set inCwork to -100 before closing
	//printf("thread 1 started\n");
	int c;
// Line counter
    while ((c = fgetc(infile)) != EOF){
        if (c==10){
            ++totalCharacters; 
        }
    }
  rewind(infile);
//--------------------------------------

while ((c = fgetc(infile)) != EOF) { // pulls in characters from the textfile

	//printf("Thread 1 in: %c\n",c); // for testing how the threads interact
	//printf("Thread 1 while loop started\n");

	pthread_mutex_lock(&isEmpty1_mutex);
		if(!isEmpty1){ // if the variable not is empty then wait
			//printf("Thread 1 wait started\n");
			pthread_cond_wait(&isEmpty1_cond, &isEmpty1_mutex); // have to make a signal in the others
		}
	pthread_mutex_unlock(&isEmpty1_mutex); 

	pthread_mutex_lock(&inCwork_mutex);
	inCwork = c;
	pthread_mutex_unlock(&inCwork_mutex);
	//printf("Thread 1 out: %c\n",c); // for testing how the threads interact
	pthread_mutex_lock(&isEmpty1_mutex); 
	isEmpty1 = false;
	pthread_cond_signal(&isEmpty1_cond); 
			//printf("Thread 1 wake Thread 2\n");
	pthread_mutex_unlock(&isEmpty1_mutex);
	}
// for when there is no more input
		for(;;){
			pthread_mutex_lock(&inCwork_mutex);
			pthread_mutex_lock(&isEmpty1_mutex);
				if(isEmpty1){ // if empty
					//printf("Close Sent from Thread 1\n"); 
					inCwork = -100; // signals the next thread that no more data is coming
					pthread_mutex_unlock(&inCwork_mutex);
  				isEmpty1 = false; 
					pthread_cond_signal(&isEmpty1_cond);
					pthread_mutex_unlock(&isEmpty1_mutex);

        	pthread_exit(NULL);
         }// if end
			pthread_cond_signal(&isEmpty1_cond); 
			pthread_mutex_unlock(&inCwork_mutex);
			pthread_mutex_unlock(&isEmpty1_mutex);
		}// for end 
}

//-----------------------------------------------------------------------------------------------------------------------------------

int inCworkCheck() 
{
	pthread_mutex_t result_mutex;
  pthread_mutex_init(&result_mutex, NULL);
  int result;
	pthread_mutex_lock(&result_mutex); 
  result = inCwork;
  pthread_mutex_unlock(&result_mutex);
  pthread_mutex_destroy(&result_mutex);
  return result;
}
//-------------------------------------------------------------------------------------------------------

void * funcWork(void * arg) { //pass in password and encrypt as a struct or make them global vars

	enigma machine(password); // create object for encrypting and decrypting needs a password 

		while ( inCworkCheck() !=-100){ // is set to -100 when the read in thread is finished
			//printf("Thread 2 while loop started\n");
			int c; // make sure this is in the right location
			pthread_mutex_lock(&isEmpty1_mutex);

				if(isEmpty1){
					//printf("Thread 2 wait 1 started\n");
					pthread_cond_wait(&isEmpty1_cond, &isEmpty1_mutex); 
				}
			pthread_mutex_unlock(&isEmpty1_mutex);
			pthread_mutex_lock(&inCwork_mutex);
			c = inCwork;
			pthread_mutex_unlock(&inCwork_mutex);
			//printf("Thread 2 in: %i\n",c); // for testing how the threads interact
			pthread_mutex_lock(&isEmpty1_mutex);
    	isEmpty1 = true; // because we just read in the value
			pthread_cond_signal(&isEmpty1_cond); // not in the right location causing problems
			pthread_mutex_unlock(&isEmpty1_mutex);
      // signal used to be here
			//printf("Thread 2 wake Thread 1\n");

				if(c==10 or c== -100){
				// pass on the variable to the next thread without touching it

  			}else{ 
       		c = machine.process(c,encryptstate);     
  			}
	// output data
			pthread_mutex_lock(&isEmpty2_mutex);
				if(!isEmpty2){ // if the variable not is empty then wait
					//printf("Thread 2 wait 2 started\n");
					pthread_cond_wait(&isEmpty2_cond, &isEmpty2_mutex); 
				}
			pthread_mutex_unlock(&isEmpty2_mutex);
			pthread_mutex_lock(&workCout_mutex);
			workCout = c;
			pthread_mutex_unlock(&workCout_mutex);
			//printf("Thread 2 out: %c\n",c); // for testing how the threads interact
			pthread_mutex_lock(&isEmpty2_mutex);
			isEmpty2 = false;
			pthread_cond_signal(&isEmpty2_cond);
			//printf("Thread 2 wake Thread 3\n");
			pthread_mutex_unlock(&isEmpty2_mutex);
		}
	pthread_mutex_lock(&workCout_mutex);
	workCout = -100; // pass on so that the other threads know to shut down
	pthread_mutex_unlock(&workCout_mutex);
	//printf("Thread 2 out: -100\n"); // for testing how the threads interact
	pthread_mutex_lock(&isEmpty2_mutex);
	isEmpty2 = false;
	pthread_cond_signal(&isEmpty2_cond);
	pthread_mutex_unlock(&isEmpty2_mutex);


  pthread_exit(NULL);
}

//-----------------------------------------------------------------------------------------------------------------------------------

int workCoutCheck()
{
	pthread_mutex_t result_mutex;
  pthread_mutex_init(&result_mutex, NULL);
  int result;
  pthread_mutex_lock(&result_mutex); 
  result = workCout;
  pthread_mutex_unlock(&result_mutex);
  pthread_mutex_destroy(&result_mutex);
  return result;
}
//--------------------------------------

void * funcOut(void * arg) { 

	while ( workCoutCheck() !=-100){ // is set to -100 when the work thread is finished
		//printf("Thread 3 while loop started\n");
		int c; 
		pthread_mutex_lock(&isEmpty2_mutex);

			if(isEmpty2){ // if the variable not is empty then wait
				//printf("Thread 3 wait started\n");
				pthread_cond_wait(&isEmpty2_cond, &isEmpty2_mutex); // maybe wont work
			}
		pthread_mutex_unlock(&isEmpty2_mutex);
		pthread_mutex_lock(&workCout_mutex);
		c = workCout;
		pthread_mutex_unlock(&workCout_mutex);
		//printf("Thread 3 in: %c\n",c); // for testing how the threads interact
		pthread_mutex_lock(&isEmpty2_mutex);
		isEmpty2 = true;
		pthread_cond_signal(&isEmpty2_cond); // new test for function
		//printf("Thread 3 wake Thread 2\n");
		pthread_mutex_unlock(&isEmpty2_mutex);
			if(c!=-100){ // see if really needed
      	fputc(c, fileout); 
        currentCharacter++; // sets what the current character is on 
				//printf("Thread 3 out: %c\n",c); // for testing how the threads interact
			}

	}
  pthread_exit(NULL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


int main(int argc, char **argv) 
{
// Timers
  time_t actStart,end; // Actual time 
	actStart = time(NULL);
	clock_t start = clock(); // CPU Time

// sees how many arguments are being supplied
    if (argc != 5) { // means there arnt 1 + 4 arguments supplied in the command line
        fprintf(stderr, "Usage: %s -e or -d password infile.txt outfile.txt\n", argv[0]);
        return 0; // exits the program
    }

// Fopen commands
    if (!(infile = fopen(argv[3], "rt"))) { // r or rt? was 1
        perror(argv[3]); // sends error msg if file doesnt exist
        return 0; //exits program
    }

    if (!(fileout = fopen(argv[4], "wb+x"))) { // writes but doesnt overwrite existing files 
        perror(argv[4]);
        return 0; //exits program
    }
	//printf("Files Opened\n");

		switch( argv[1][0] ) // argv[1][0] checks the first char in the second command argument
       {
       case '-':
          switch( argv[1][1] ) // argv[1][1] checks the second char in the second command
           {
            case 'e':
            // set encrypt to true to encrypt data
            printf("Encrypting data...\n");
            encryptstate = true;  
            break;
            case 'd':
            // set encrypt to false to decrypt data
            printf("Decrypting data...\n");
            break;
            default:
            printf("Invalid parameter: %s\n", argv[1]);
            printf("-e Encrypts and -d Decrypts");
            return 0; // exit program if invalid
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
	//printf("password is stored\n");

//-------------------------------------------------------Thread Operations Begin------------------------------------------------------------------------------------------------------------
	pthread_t tIn, tWork, tOut; // threads 
 // Initialize mutex and condition variable objects
  pthread_mutex_init(&isEmpty1_mutex, NULL);
  pthread_mutex_init(&isEmpty2_mutex, NULL);
  pthread_mutex_init(&inCwork_mutex, NULL);
  pthread_mutex_init(&workCout_mutex, NULL);
 // Condition Variables
  pthread_cond_init (&isEmpty1_cond, NULL);
  pthread_cond_init (&isEmpty2_cond, NULL);
 // Create threads
	assert(pthread_create(&tIn, NULL, funcIn, NULL)== 0);
	assert(pthread_create(&tWork, NULL, funcWork, NULL)== 0);
	assert(pthread_create(&tOut, NULL, funcOut, NULL)== 0);
 // Join threads
	assert(pthread_join(tIn, NULL)== 0);
	assert(pthread_join(tWork, NULL)== 0);
	assert(pthread_join(tOut, NULL)== 0);


//----------------------------------------------------------------------------------------------
 // Close the files used
  fclose(infile); // close infile when finished
  fclose(fileout); // close outfile when finished
 // Destroy the mutex locks and condition 
  pthread_mutex_destroy(&isEmpty1_mutex);
  pthread_cond_destroy(&isEmpty1_cond);
  pthread_mutex_destroy(&isEmpty2_mutex);
  pthread_cond_destroy(&isEmpty2_cond);
  pthread_mutex_destroy(&inCwork_mutex);
  pthread_mutex_destroy(&workCout_mutex);

	clock_t finished = clock(); // finished time
  end = time(NULL);
  printf("All Finished!\n");
	printf("%g CPU Seconds\n",(double) (finished-start)/CLOCKS_PER_SEC); // prints program cpu time
  printf("Actual running time %g seconds\n", difftime(end, actStart));
		if(encrypt){
  		printf("Number of lines encrypted %i\n", totalCharacters);
		}else{
  		printf("Number of lines decrypted %i\n", totalCharacters);
		}
  return 0;
}


