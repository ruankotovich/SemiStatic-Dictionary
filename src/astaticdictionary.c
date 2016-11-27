#include "../include/astaticdictionary.h"
#include "../include/chainable.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

#define throw(x) printf(x)

typedef struct{
  void *data;
  int key;
}TAStaticDictionaryItem;

typedef struct{
  TAStaticDictionaryItem **items;
  int length;
  int used;
  float chargeFactor;
}TAStaticDictionaryData;

static void af_insertDictionaryItem(TAStaticDictionary*,TAStaticDictionaryItem*);

static TAStaticDictionaryItem* af_createItem(void* data, int key){
  TAStaticDictionaryItem *item = malloc(sizeof(TAStaticDictionaryItem));
  item->data = data;
  item->key = key;
  return item;
}

static void af_rehash(TAStaticDictionary *dictionary){
  TAStaticDictionaryData *data = dictionary->data;
  int oldLength = data->length;
  TAStaticDictionaryItem **oldVector = data->items;

  data->length = oldLength+50;//(int)pow(2,k);//data->length + data->length;
  data->used=0;
  data->items = calloc(data->length,sizeof(TAStaticDictionaryItem*));

  for(int i=0;i<oldLength;i++){
    if(oldVector[i]){
      af_insertDictionaryItem(dictionary,oldVector[i]);
    }
  }

  free(oldVector);
}

static int af_hash(TAStaticDictionaryData *data,unsigned int key){
  return floor(data->length * fmod((key * M_PI+data->length),1.0));
}

static void af_insertDictionaryItem(TAStaticDictionary* dictionary, TAStaticDictionaryItem *item){
  TAStaticDictionaryData *data = dictionary->data;
  int pos = af_hash(data, item->key);
  int walked = 0;
  int maxProspection = (floor(log2(data->used+1))+1) *2;

  while(data->items[pos] != NULL && walked <= maxProspection){pos=(pos+1) % data->length; walked++;}

  if(walked <= maxProspection){
    data->items[pos] = item;
    data->used++;
  }else{
    printf("\n[LOG] : Expected prospection reached {expected : %d ; used : %d; prospected : %d}",maxProspection,data->used,walked);
    printf("\n\t|_");
    IChainable *chain = item->data;
    chain->chainableInstance.print(chain);
    af_rehash(dictionary);
    af_insertDictionaryItem(dictionary,item);
  }
}

static void f_printDictionary(TAStaticDictionary * dictionary){
  TAStaticDictionaryData *data = dictionary->data;
  IChainable *chainable;
  TAStaticDictionaryItem *item;
  printf("\nDictionary $ chargeFactor - %lf / chargeUsed - %lf  ||  length - %d / used - %d",data->chargeFactor,(float)data->used/(float)data->length,data->length,data->used);
  for(int i=0;i<data->length;i++){
    printf("\n");
    if(data->items[i]){
      chainable = data->items[i]->data;
      item = data->items[i];
      printf("{%d - ",i);
      chainable->chainableInstance.print(chainable);
      printf(" | Key : %u ; Hash : %d}",item->key,af_hash(data,item->key));
    }else{
      printf("{%d - NULL}",i);
    }
  }
  printf("\n");
}

static void f_insertDictionaryItem(TAStaticDictionary *dictionary, void *dataM){
  TAStaticDictionaryData *data = dictionary->data;
  IChainable* dataI = dataM;
  if(data->chargeFactor > (float)(data->used+1)/(float)data->length){
    af_insertDictionaryItem(dictionary, af_createItem(dataI, dataI->chainableInstance.getKey(dataI)));
  }else{
    printf("\n[LOG] : Charge factor overloaded! {cf : %.2f, uf : %.2f}",data->chargeFactor,(float)(data->used+1)/(float)data->length);
    af_rehash(dictionary);
    af_insertDictionaryItem(dictionary,af_createItem(dataI, dataI->chainableInstance.getKey(dataI)));
  }
}

static void* f_searchDictionaryItem(TAStaticDictionary *dictionary, int key){
  TAStaticDictionaryData *data = dictionary->data;
  int pos = af_hash(data,key);
  int walked = 0;
  int maxProspection = (floor(log2(data->used+1))+1) *2;

  while(data->items[pos] != NULL && walked <= maxProspection){
    if(data->items[pos]->key != key){
      pos = (pos+1) % data->length;
      walked++;
    }else{
      return data->items[pos]->data;
    }
  }

  return NULL;
}

TAStaticDictionary *new_astaticdictionary(int length,float chargeFactor){
  if(chargeFactor > 1.0f){
    throw("ChargeFactor cannot be bigger than 1.0f!");
    return NULL;
  }else{
    TAStaticDictionary *newoDictionary = malloc(sizeof(TAStaticDictionary));
    TAStaticDictionaryData *newoDictionaryData = malloc(sizeof(TAStaticDictionaryData));
    newoDictionary->data = newoDictionaryData;

    newoDictionaryData->used = 0;
    newoDictionaryData->length = ceil(((float)length) / chargeFactor);
    newoDictionaryData->chargeFactor = chargeFactor;
    newoDictionaryData->items = calloc(newoDictionaryData->length,sizeof(TAStaticDictionaryItem*));

    newoDictionary->insert = f_insertDictionaryItem;
    newoDictionary->search = f_searchDictionaryItem;
    newoDictionary->printDictionary = f_printDictionary;

    return newoDictionary;
  }
}

void destroy_dictionary(TAStaticDictionary *dic){
  TAStaticDictionaryData* data = dic->data;
  for(int i=0;i<data->length;i++){
    free(data->items[i]);
    data->items[i] = NULL;
  }
  data->used = 0;
}
