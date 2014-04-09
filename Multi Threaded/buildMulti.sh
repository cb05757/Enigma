#1/bin/bash

# Compiles Enigma Multi Threaded Program
if [ "${1}" != "" ] 
	then  
    BUILD=`g++ -lpthread enigmaMainM.cpp enigmaClass.cpp -o ${1}`
    echo $BUILD
			else
    	 BUILD=`g++ -lpthread enigmaMainM.cpp enigmaClass.cpp -o enigmaMulti`
   		 echo $BUILD
fi


