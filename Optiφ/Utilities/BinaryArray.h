#include <stdlib.h>
class BinaryArray {

public:
    // Methods

  bool isValid();
  int getLength();
  int* getData();

  // Operators
  int&          operator()   (int index) const;
  BinaryArray     operator +   (const BinaryArray & arr) const;
  void          operator +=  (const BinaryArray & arr) ;
  void          operator =   (const BinaryArray & arr);
  bool          operator ==  (const BinaryArray & p) const;
  
   
  static BinaryArray inValidBinaryArray;


  

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