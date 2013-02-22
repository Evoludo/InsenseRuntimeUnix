#include "AnyType.h"

AnyTypePNTR Construct_IntAnyType0(int value, char *type) {
  AnyTypePNTR this = Construct_AnyType();
  if(this != NULL){
    this->value.int_value = value;
    this->type = type;
  }
  return this;
}

