#ifndef _XYAH_FLASHCARDS_CHAINABLE_H_
#define _XYAH_FLASHCARDS_CHAINABLE_H_

typedef struct chainableType TChainable;
typedef struct chainable IChainable;

typedef void(*FPrintChainable)(void*);
typedef short(*FCompareChainable)(void*,void*);
typedef short(*FCompareKeyChainable)(void*,void*);
typedef unsigned int(*FGetKey)(void*);

typedef struct chainableType{
  FPrintChainable print;
  FCompareChainable compare;
  FCompareKeyChainable compareKey;
  FGetKey getKey;
}TChainable;

typedef struct chainable{
  void *data;
  TChainable chainableInstance;
}IChainable;

#endif //_XYAH_FLASHCARDS_CHAINABLE_H_
