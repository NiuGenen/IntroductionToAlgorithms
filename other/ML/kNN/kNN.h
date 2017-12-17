/** @file kNN.h
Basic kNN alg

Author: niugen

GitHub: NiuGenen

2017/12/16

**/

#ifndef _NG_KNN_H_
#define _NG_KNN_H_

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{ \
    if( p != NULL ){  \
        free(p);      \
        p = NULL;     \
    }                 \
}while(0);            \

#define IN
#define OUT
#define OPTIONAL
#define CONST const

typedef double kNN_elem_t;

typedef struct kNN_vector{
    kNN_elem_t*  elem;
}kNN_vector;

#define KNN_TYPE_STR_LEN 32

typedef struct kNN_type{
    int   type_id;
    char  type_str[ KNN_TYPE_STR_LEN ];
}kNN_type;

typedef struct kNN_dataset{
    kNN_vector         *data_vctr_s;
    size_t             *data_type_id;
    size_t             data_size;
    size_t             data_vctr_len;
    kNN_type           *type;
    size_t             type_nr;
    kNN_vector         *test_data_vctr_s;
    size_t             *test_data_type_id;
    size_t             test_data_size;
}kNN_dataset;

struct kNN;

typedef void (*kNN_set_dataset_fn)( struct kNN *knn, kNN_dataset *data, int if_cpy );
typedef void (*kNN_identy_fn)( struct kNN *knn, const kNN_vector *vctr, kNN_type *type);
typedef void (*kNN_set_K_fn)( struct kNN *knn, size_t K);
typedef void (*kNN_pr_data_fn)( struct kNN *knn);

typedef void (*com_normalize_fn)( struct kNN *knn, const kNN_vector *vctr, kNN_vector *_vctr );

typedef struct kNN{
    kNN_dataset         dataset        ;
    int                 has_dataset    ;
    kNN_vector          data_min_vctr  ;
    kNN_vector          data_max_vctr  ;  
    kNN_vector          data_dif_vctr  ;  
    kNN_elem_t          K              ;
    kNN_set_dataset_fn  op_set_dataset ;
    kNN_identy_fn       op_identy      ;
    kNN_set_K_fn        op_set_K       ;
    kNN_pr_data_fn      op_pr_data     ;
    com_normalize_fn    com_normal     ;
}kNN;

/**
  Alloc an empry kNN
  
  @retval struct knn *    The pointer to an empty kNN struct
  
**/
kNN*
kNN_alloc();

/**
  Free an kNN struct 

  @param knn             The pointer to a kNN struct 

  @param free_data       If is true( > 0 ), then the dataset inside
                         this kNN struct will be freeed

**/
void
kNN_free(
    IN       kNN   *knn,
    OPTIONAL int   free_data 
);


/**
  Init function for kNN struct. Set the data set of this kNN struct.

  Note: Each kNN struct can only call this function once. Controled by
  field @has_dataset

  @param knn            The pointer to this kNN struct

  @param dataset        The dataset of this kNN function

  @param if_cpy         If is true( > 0 ), then the data inside this dataset 
                        will be copied into this kNN struct.
                        Else the kNN struct's dataset will point to this dataset.

**/
void 
kNN_set_dataset(
    IN       kNN          *knn,
    IN       kNN_dataset  *dataset, 
    OPTIONAL int          if_cpy 
);

/**
  Classy a knn_vector's type by this kNN struct.

  @param knn            The pointer to this kNN struct.

  @param vctr           The vector to be classied. If it is not 
                        normalized, then it will be normalized to 1.
                        This function will NOT change the vctr's value.

  @retval               The kNN Type of this vctr.

**/ 
void
kNN_identy(
    IN        kNN          *knn, 
    IN  CONST kNN_vector   *vctr,
    OUT       kNN_type     *type
);

/**
  Set the essential value K for this kNN struct.

  This function can be called more than once.

  @param[in] knn            The pointer to ehis kNN struct.

  @param[in] K              The value K for kNN alg    

**/
void 
kNN_set_K(
    IN kNN         *knn,
    IN size_t      K
);

/**
  Print the data of thie kNN struct 

  @param[in] knn          The pointer to this kNN struct

NOTE:
  If kNN->has_dataset if false, then print waring

**/
void
kNN_pr_data(
    IN kNN *knn
);

/**
  Normalize kNN_vector 

  newValue = ( old_value - kNN.MinValue ) / ( kNN.MaxValue - kNN.MinValue )

  @param[in]  vctr           The kNN_vector to be normalized

  @param[out] _vctr          The kNN_vector after normalize

**/
void
com_normalize(
    IN       kNN        *knn,
    IN CONST kNN_vector *vctr,
    OUT      kNN_vector *_vctr
);




















#endif
