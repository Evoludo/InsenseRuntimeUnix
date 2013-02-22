#include "AnyType.h"

AnyTypePNTR Construct_BoolAnyType0(bool value, char* type) {
  AnyTypePNTR this = Construct_AnyType();
  if(this != NULL){
    this->value.bool_value = value;
    this->type = type;
  }
  return this;
}
