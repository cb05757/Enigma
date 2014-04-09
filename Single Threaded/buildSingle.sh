#1/bin/bash

# Compiles Single Threaded Enigma Program
if [ "${1}" != "" ] 
	then  
    BUILD=`g++ enigmaMainS.cpp enigmaClass.cpp -o ${1}`
    echo $BUILD
			else
    	 BUILD=`g++ enigmaMainS.cpp enigmaClass.cpp -o enigmaSingle`
   		 echo $BUILD
fi


