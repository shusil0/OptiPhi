#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "BinaryArray.h"

using namespace std;



int inValidEntry = -1; //In that is to be returned if an invalid entry is accessed
BinaryArray BinaryArray::inValidBinaryArray(new int[0], -1); //Invald data array, returned when functions that return a data array fail

//Returns length of data array
int BinaryArray::getLength(){
    return _length;
}
//Returns a pointer to the data array, this means multiple objects will point to the same array, this is expected and normal
int* BinaryArray::getData(){
    return _data;
}
//Checks to see if array is valid
bool BinaryArray::isValid(){
    return _length > 0;
}

/*
*Operator:()
*this takes a data array, and returns a requested value

Input: Index of data that needs to be accessed
Output:
*0/1 if the input is in range of the array
*-1 if the requested element is out of range

*/
int & BinaryArray::operator()(int index) const {
  if (index >= _length || index < 0) {
    return inValidEntry;
  }
  return _data[index];
}
/*
*Operator:+
*this takes a data array, and appends it to its self

Input: BinaryArray to add
Output: BinaryArray that has the parameter data Array appended

Ex: [0,1,1,1,1,1] + [0,0,0,0,0,0,0] = [0,1,1,1,1,1,0,0,0,0,0,0,0]

*/
BinaryArray BinaryArray::operator + (const BinaryArray & arr) const {
  BinaryArray temp( * this);//Create 
  temp._length += arr._length;
  delete[] temp._data;
  temp._data = new(std::nothrow) int[temp._length];
  if (temp._data == NULL) {
    return inValidBinaryArray;
  }
  for (int i = 0; i < _length; i++) {
    temp._data[i] = _data[i];
  }
  for (int i = _length; i < temp._length; i++) {
    temp._data[i] = arr._data[i - _length];
  }
  return temp;
}
void BinaryArray::operator += (const BinaryArray & arr)  {
    *this = *this + arr;
}
/*
*Operator:=
*this is the assignment operator, assigns the parameter passes in to the "this" object
Input: Data array to assign
Output: none

*/
void BinaryArray::operator = (const BinaryArray & arr) {
    _data = new(std::nothrow) int[arr._length]; 
    if(_data == NULL){
        _length = -1;
        return;
    }
   _length = arr._length;
   for (int i = 0; i < _length; i++) {
     _data[i] = arr._data[i];
   }
}
/*
*Operator:==
*this operator checks to see if two Binary Arrays are the same  
Input: Data array to check
Output: boolean denoting wether or not to binary arrays are the same 

*/
bool BinaryArray::operator == (const BinaryArray & arr) const {
  if (_length != arr._length)
    return 0;
  for (int i = 0; i < _length; i++) {
    if (arr(i) != this -> operator()(i))
      return 0;
  }
  return 1;

}
// Constructors/Destructor
BinaryArray::BinaryArray() {
  _length = 0;
  _data = new(std::nothrow) int[0];
}
BinaryArray::BinaryArray(const BinaryArray &arr) {
    _data = new(std::nothrow) int[arr._length]; 
    if(_data == NULL){
        _length = -1;
        return;
    }
   _length = arr._length;
   for (int i = 0; i < _length; i++) {
     _data[i] = arr._data[i];
   }
}
BinaryArray::BinaryArray(const int* data,const int length) {
    _length = length;
    _data = new(std::nothrow) int[length]; 
    for(int i = 0; i < length; i++){
        _data[i] = !!data[i];
        cout<<"i: "<<i<<", "<<_data[i]<<endl;
    }
}
BinaryArray::~BinaryArray(){
    delete[] _data;
    _data = NULL;
    _length = 0;
}

std::ostream& operator<<(std::ostream& os, BinaryArray& a) {
  if (!a.isValid()) {
    os << "Not A Data Array";
    return os;
  }

  os << "[";
  for (int i = 0; i < a.getLength(); ++i) {
    os << a(i);
    if (i < (a.getLength() - 1))
      os << ", ";
  }
  os << "]";
  return os;
}