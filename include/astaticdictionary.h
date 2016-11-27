#ifndef _XYAH_ASTATICDICTIONARY_H_
#define _XYAH_ASTATICDICTIONARY_H_

typedef struct dictionary TAStaticDictionary;

typedef void (*FInsertDictionaryItem)(TAStaticDictionary*,void*);
typedef void* (*FSearchDictionaryItem)(TAStaticDictionary*,int);
typedef void (*FPrintDictionary)(TAStaticDictionary*);

typedef struct dictionary{
  void *data;
  FInsertDictionaryItem insert;
  FSearchDictionaryItem search;
  FPrintDictionary printDictionary;
}TAStaticDictionary;


TAStaticDictionary *new_astaticdictionary(int,float);
void destroy_dictionary(TAStaticDictionary*);
#endif
