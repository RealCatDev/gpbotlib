#ifndef   _GPBOTLIB_NBT_H_
#define   _GPBOTLIB_NBT_H_

#include <stdint.h>
#include <gpbotlib/buffer.h>
#include <gpbotlib/result.h>

typedef uint8_t Gp_Nbt_Tag_Type;
enum {
  GP_NBT_TAG_END = 0,
  GP_NBT_TAG_BYTE,
  GP_NBT_TAG_SHORT,
  GP_NBT_TAG_INT,
  GP_NBT_TAG_LONG,
  GP_NBT_TAG_FLOAT,
  GP_NBT_TAG_DOUBLE,
  GP_NBT_TAG_BYTE_ARRAY,
  GP_NBT_TAG_STRING,
  GP_NBT_TAG_LIST,
  GP_NBT_TAG_COMPOUND,
  GP_NBT_TAG_INT_ARRAY,
  GP_NBT_TAG_LONG_ARRAY
};

typedef struct Gp_Nbt_Tag {
  Gp_Nbt_Tag_Type type;
  uint16_t nameLength;
  char *name;
  void *data;
} Gp_Nbt_Tag;

typedef struct {} Gp_Nbt_Tag_None_Data;
typedef int8_t Gp_Nbt_Tag_Byte_Data;
typedef int16_t Gp_Nbt_Tag_Short_Data;
typedef int32_t Gp_Nbt_Tag_Int_Data;
typedef int64_t Gp_Nbt_Tag_Long_Data;
typedef float Gp_Nbt_Tag_Float_Data;
typedef double Gp_Nbt_Tag_Double_Data;
typedef struct Gp_Nbt_Tag_Byte_Array_Data {
  int32_t size;
  uint8_t *array;
} Gp_Nbt_Tag_Byte_Array_Data;
typedef struct Gp_Nbt_Tag_Byte_Array_Data {
  uint16_t lenght;
  char *string;
} Gp_Nbt_Tag_Byte_Array_Data;
typedef struct Gp_Nbt_Tag_List_Data {
  Gp_Nbt_Tag_Type tagType;
  int32_t lenght;
  void *payload;
} Gp_Nbt_Tag_List_Data;
typedef struct Gp_Nbt_Tag *Gp_Nbt_Tag_Compound_Data;
typedef struct Gp_Nbt_Tag_Int_Array_Data {
  int32_t size;
  Gp_Nbt_Tag_Int_Data *data;
} Gp_Nbt_Tag_Int_Array_Data;
typedef struct Gp_Nbt_Tag_Long_Array_Data {
  int32_t size;
  Gp_Nbt_Tag_Long_Data *data;
} Gp_Nbt_Tag_Long_Array_Data;

typedef Gp_Nbt_Tag Gp_Nbt;
void gp_is_valid_nbt(const Gp_Nbt nbt);
Gp_Result gp_nbt_parse(void *buffer, Gp_Nbt *nbt, const Gp_Read_Byte_From_Buffer read);
Gp_Result gp_nbt_write(void *buffer, Gp_Nbt nbt, const Gp_Write_Byte_From_Buffer write);

#endif // _GPBOTLIB_NBT_H_
