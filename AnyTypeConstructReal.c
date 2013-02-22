#include "AnyType.h"

AnyTypePNTR Construct_RealAnyType0(float value, char* type) {
  AnyTypePNTR this = Construct_AnyType();
  if(this != NULL){
    this->value.real_value = value;
    this->type = type;
  }	
  return this;
}
