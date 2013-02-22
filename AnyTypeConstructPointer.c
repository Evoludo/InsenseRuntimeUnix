#include "AnyType.h"

AnyTypePNTR Construct_PointerAnyType0(void* value, char* type) {
  AnyTypePNTR this = Construct_AnyType();
  if(this != NULL){
    DAL_assign(&this->value.pointer_value, value);
    this->type = type;
  }	
  return this;
}
