#ifndef   _GPBOTLIB_NBT_H_
#define   _GPBOTLIB_NBT_H_

#include <gpbotlib/result.h>
#include <gpbotlib/buffer.h>

#include <stdint.h>
#include <stdbool.h>

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

typedef struct Gp_Nbt_Tag Gp_Nbt_Tag;

typedef int8_t Gp_Nbt_Tag_Byte;
typedef int16_t Gp_Nbt_Tag_Short;
typedef int32_t Gp_Nbt_Tag_Int;
typedef int64_t Gp_Nbt_Tag_Long;
typedef float Gp_Nbt_Tag_Float;
typedef double Gp_Nbt_Tag_Double;

typedef struct Gp_Nbt_Tag_Compound {
  Gp_Nbt_Tag *tags;
  size_t capacity;
  size_t count;
} Gp_Nbt_Tag_Compound;

typedef struct Gp_Nbt_Tag_Byte_Array {
  int32_t size;
  uint8_t *array;
} Gp_Nbt_Tag_Byte_Array;

typedef struct Gp_Nbt_Tag_String {
  uint16_t length;
  char *string;
} Gp_Nbt_Tag_String;

typedef struct Gp_Nbt_Tag_As Gp_Nbt_Tag_As;

typedef struct Gp_Nbt_Tag_List {
  Gp_Nbt_Tag_Type tagType;
  int32_t length;
  Gp_Nbt_Tag_As *payload;
} Gp_Nbt_Tag_List;

typedef struct Gp_Nbt_Tag_Int_Array {
  int32_t size;
  Gp_Nbt_Tag_Int *data;
} Gp_Nbt_Tag_Int_Array;

typedef struct Gp_Nbt_Tag_Long_Array {
  int32_t size;
  Gp_Nbt_Tag_Long *data;
} Gp_Nbt_Tag_Long_Array;

struct Gp_Nbt_Tag_As {
  union {
    Gp_Nbt_Tag_Byte Byte;
    Gp_Nbt_Tag_Short Short;
    Gp_Nbt_Tag_Int Int;
    Gp_Nbt_Tag_Long Long;
    Gp_Nbt_Tag_Float Float;
    Gp_Nbt_Tag_Double Double;
    Gp_Nbt_Tag_Byte_Array ByteArray;
    Gp_Nbt_Tag_String String;
    Gp_Nbt_Tag_List List;
    Gp_Nbt_Tag_Compound Compound;
    Gp_Nbt_Tag_Int_Array IntArray;
    Gp_Nbt_Tag_Long_Array LongArray;
  };
};

struct Gp_Nbt_Tag {
  Gp_Nbt_Tag_Type type;
  uint16_t nameLength;
  char *name;
  Gp_Nbt_Tag_As as;
};

typedef Gp_Nbt_Tag Gp_Nbt;

bool gp_is_valid_nbt(Gp_Nbt nbt);

Gp_Result gp_parse_nbt_tag_compound(void *buffer, Gp_Nbt_Tag_Compound *compound);
Gp_Result gp_write_nbt_tag_compound(void *buffer, Gp_Nbt_Tag_Compound compound);

Gp_Result gp_parse_nbt_tag(void *buffer, Gp_Nbt_Tag *tag);
Gp_Result gp_write_nbt_tag(void *buffer, Gp_Nbt_Tag tag);

#endif // _GPBOTLIB_NBT_H_