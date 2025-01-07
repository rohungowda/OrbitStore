#include <iostream>
#include <fstream>


/*
Data Type	    Size 	    Range
short	        2 bytes	    -32,768 to 32,767
unsigned short	2 bytes	    0 to 65,535
int	            4 bytes	    -2,147,483,648 to 2,147,483,647
unsigned int	4 bytes	    0 to 4,294,967,295
 */

// std::streampos pos gives a poisition in files, bytes, that is resistant to platform change

//seekg -  // ( bytes offset, [ios::beg, ios::end, ios:curr] ) - from where
// can acutaly go backwards with -1,-2,-3,-4 ...

//seekp - write to a position in to a file (offset, ptr)
// put - data , moves the pointer by 1 and inserts the data in
// tellg() current pointern location

void writeArray(std::fstream& fout, int a[], int size){
    // this a[] is the pointer to the first element in the array. Could be written as int* a. where a is an array
    fout.write(reinterpret_cast<char*>(a), size * sizeof(size));
}

void readArray(std::fstream& fout, std::streampos offset, int size){

    // number of bytes offset, pointer from where
    // tellg(): This returns the current position of the read pointer in the file.
    fout.seekg(offset, std::ios::beg);
    int results[size];
    fout.read(reinterpret_cast<char*>(results), sizeof(results));

    for(int i = 0; i < 7; i++){
        std::cout << results[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    
    std::fstream fout;

    // when the std::ios::trunc flag is true each rewrite ereases the data in the file
    // std::ios::out says data can only be written to and not read from
    // to append data -> std::ios::app
    // std::ios::ate: -> Moves the file pointer to the end of the file when the file is opened, but allows reading or writing anywhere in the file afterward.
    //char*: A pointer that points to the starting memory address of a sequence of bytes. It doesn't care about how many bytes the data being pointed to actually occupies; it just treats the memory as a sequence of bytes.
    fout.open("test.dat", std::ios::out | std::ios::binary);

    
    // 16 bits (2 bytes), and positive integers -> [0,2^(16) - 1] -> [0, 65,535]
    unsigned short two_bytes = 125;
    int value[] = {5,6,7,8,2,4,6};
    int SIZE = 7;
    // number of bytes = 2 + (7 * 4) = 30 bytes in file

    if (fout){
        // memory address of the variable but needs to be a characther memory address, number of bytes being written to file
        // the reinteprit_cast convert the short address in memory to a charactehr pointer in memory
        fout.write(reinterpret_cast<char*>(&two_bytes), sizeof(unsigned short));

        // arrays are pointers in memory to addresses where the data is stored sequentially
        writeArray(fout,value,SIZE);

        fout.close();
    }else{
        std::cout << "Error opening File! \n";
    }

    // only reads data from a file
    fout.open("test.dat", std::ios::in | std::ios::binary);

    if (fout){
        // address to send data in memory, size of data being read
        unsigned short two_bytes_output;
        int read_value[SIZE];

        fout.read(reinterpret_cast<char*>(&two_bytes_output), sizeof(unsigned short));
        
        // move two bytes from the beggining

        std::cout << "Identifier: " << two_bytes_output << std::endl << "Value: " << std::endl;

        readArray(fout, sizeof(unsigned short), SIZE);

        fout.close();


    }else{
        std::cout << "Error opening File! \n";
    }

    return 0;
}
