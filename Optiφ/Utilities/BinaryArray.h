#include <stdlib.h>
#include "Utilities.cpp"

class BinaryArray {

public:

  static BinaryArray inValidBinaryArray;
  static BinaryArray newCharArray;
  // Methods
  bool isValid();
  int getLength();
  //int* getData();
  void addNewChar();
  int numOnes();
  int numZeros();

  // Operators
  int&          operator()   (int index) const;
  BinaryArray     operator +   (const BinaryArray & arr) const;
  void          operator +=  (const BinaryArray & arr) ;
  BinaryArray     operator +   (const bool b) const;
  void          operator +=  (const bool b);
  void          operator =   (const BinaryArray & arr);
  bool          operator ==  (const BinaryArray & p) const;
  
   



  

  // Constructors/Destructor
  BinaryArray(const int* data,const int length);
  BinaryArray(const BinaryArray & m);
  BinaryArray();
  // Initialized matrix
  ~BinaryArray();

private:
    int _length;
    int* _data;
    friend std::ostream& operator<<(std::ostream& os, const BinaryArray& m);

    

};