/** @file kNN.c
Basic kNN alg

Author: niugen

GitHub: NiuGenen

2017/12/16

**/
#include "kNN.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
  Alloc an empry kNN
  
  @retval struct knn *    The pointer to an empty kNN struct
  
**/
kNN*
kNN_alloc()
{
    kNN * knn            =   _MALLOC_( kNN , 1 );
    knn->has_dataset     =   0;
    knn->op_set_dataset  =   kNN_set_dataset;
    knn->op_identy       =   kNN_identy;
    knn->op_set_K        =   kNN_set_K;
    knn->op_pr_data      =   kNN_pr_data;
    knn->com_normal      =   com_normalize;
    knn->K               =   3;

    return knn;
}

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
    )
{
    if( knn == NULL ) return;

    if( free_data ){
        for( size_t i = 0; i < knn->dataset.data_size; ++i ){
            _FREE_( knn->dataset.data_vctr_s[i].elem );
        }
        _FREE_( knn->dataset.data_vctr_s );
        _FREE_( knn->dataset.type );
    }
    _FREE_( knn );
}

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
    )
{
    if( knn == NULL || dataset == NULL ) return;

    kNN_vector        *data_vctr_s    =  dataset->data_vctr_s   ;
    size_t            *data_type_id   =  dataset->data_type_id  ;
    size_t             data_size      =  dataset->data_size     ;
    size_t             data_vctr_len  =  dataset->data_vctr_len ;
    kNN_type          *type           =  dataset->type          ;
    size_t             type_nr        =  dataset->type_nr       ;
    size_t             i              =  0                      ;
    size_t             j              =  0                      ;
    kNN_elem_t        *elem           =  NULL                   ;
    
    if( knn->has_dataset ) return;

    knn->dataset.data_size      =  data_size     ;
    knn->dataset.data_vctr_len  =  data_vctr_len ;
    knn->dataset.type_nr        =  type_nr       ;

    knn->data_max_vctr.elem = _MALLOC_( kNN_elem_t, data_vctr_len );
    knn->data_min_vctr.elem = _MALLOC_( kNN_elem_t, data_vctr_len );
    knn->data_dif_vctr.elem = _MALLOC_( kNN_elem_t, data_vctr_len );

    if( !if_cpy ){
        // not copy data
        knn->dataset.data_vctr_s   =  data_vctr_s  ;
        knn->dataset.data_type_id  =  data_type_id ;
        knn->dataset.type          =  type         ;
    }else{ // need to copy data from dataset into kNN->dataset
        // copt data vector
        knn->dataset.data_vctr_s   = _MALLOC_( kNN_vector, data_size );
        for( i = 0; i < data_size; ++i ){
            elem =  _MALLOC_( kNN_elem_t, data_vctr_len );
            memcpy( elem,
                    data_vctr_s[i].elem,
                    sizeof( kNN_elem_t ) * data_vctr_len );
            knn->dataset.data_vctr_s[i].elem = elem;
        }
        // copy type_id
        knn->dataset.data_type_id  = _MALLOC_( size_t , data_size );
        memcpy( knn->dataset.data_type_id, data_type_id, sizeof( size_t ) * data_size );
        // copy type
        knn->dataset.type = _MALLOC_( kNN_type, type_nr );
        memcpy( knn->dataset.type,
                dataset->type,
                sizeof( kNN_type ) * type_nr );
    }

    // calculate min_vctr & max_vctr
    for( j = 0; j < data_vctr_len; ++j ){
        knn->data_min_vctr.elem[j] = data_vctr_s[0].elem[j];
        knn->data_max_vctr.elem[j] = data_vctr_s[0].elem[j];
    }
    for( i = 1; i < data_size; ++i ){
        for( j = 0; j < data_vctr_len; ++j ){
            if( data_vctr_s[i].elem[j] < knn->data_min_vctr.elem[j] )
                knn->data_min_vctr.elem[j] = data_vctr_s[i].elem[j];
            if( data_vctr_s[i].elem[j] > knn->data_max_vctr.elem[j] )
                knn->data_max_vctr.elem[j] = data_vctr_s[i].elem[j];
        }
    }
    for( j = 0; j < data_vctr_len; ++j ){
        knn->data_dif_vctr.elem[j] = knn->data_max_vctr.elem[j] - knn->data_min_vctr.elem[j];
    }

    // normalize all the vector 
    for( i = 0; i < data_size; ++i ){
        knn->com_normal( knn, &(knn->dataset.data_vctr_s[i]), &(knn->dataset.data_vctr_s[i]) );
    }

    knn->has_dataset = 1;
}

/**
  Classy a knn_vector's type by this kNN struct.

  @param knn            The pointer to this kNN struct

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
    )
{
    if( knn == NULL ) return;

    size_t        knn_K          =  knn->K;
    size_t        knn_type_nr    =  knn->dataset.type_nr;
    kNN_type     *knn_type       =  knn->dataset.type;
    size_t        knn_vctr_nr    =  knn->dataset.data_size;
    kNN_vector   *knn_vctr       =  knn->dataset.data_vctr_s;
    size_t       *knn_type_id    =  knn->dataset.data_type_id;
    size_t        knn_vctr_len   =  knn->dataset.data_vctr_len;
    kNN_elem_t    dis            =  0;
    kNN_elem_t    dis_elem       =  0;
    kNN_elem_t   *distance       =  _MALLOC_( kNN_elem_t, knn_K ); // minium distance
    size_t       *vctr_id        =  _MALLOC_( size_t, knn_K );     // its data id
    size_t       *type_count     =  _MALLOC_( size_t ,knn_type_nr ); // type_count[ type_id ] = count
    size_t        min_count      =  0;
    size_t        min_id         =  0;
    size_t        i              =  0;
    size_t        j              =  0;

    // 0. init
    for( i = 0; i < knn_type_nr; ++i ){
        type_count[i] = -1;
    }
    for( i = 0; i < knn_K; ++i ){
        distance[i] = 0;
        vctr_id[i]  = 0;
    }

    // 1. normalize this vector
    kNN_vector _vctr;
    _vctr.elem = _MALLOC_( kNN_elem_t, knn->dataset.data_vctr_len );
    knn->com_normal( knn, vctr, &_vctr );

    //
    // 2. calculate distance
    // 
    // data_id[K]  = { data_id , ... }
    // 
    // distance[K] = { dis( knn_data[data_id], vctr ) , ... }
    // 
    for( i = 0; i < knn_vctr_nr; ++i ){ // @i as data_id
        dis      = 0;
        dis_elem = 0;
        for( j = 0; j < knn_vctr_len; ++j ){
            dis_elem = knn_vctr[i].elem[j] - _vctr.elem[j];
            dis_elem *= dis_elem;
            dis += dis_elem;
        }
        if( i < knn_K ){
            distance[i] = dis;
            vctr_id[i] = i;
        }else{
            for( j = 0; j < knn_K; ++j ){
                if( dis < distance[j] ) break;
            }
            if( j != knn_K ){
                distance[j] = dis;
                vctr_id[j] = i;
            }
        }
    }
    
    //
    // 3. count type
    // 
    // type_count[ type_id ] = count
    // 
    for( i = 0; i < knn_K; ++i ){
        type_count[ knn_type_id[ vctr_id[i] ] ] += 1;
    }

    // 4. get min type
    min_count = type_count[ 0 ];
    for( i = 1; i < knn_type_nr; ++i ){
        if( type_count[i] < min_count ){
            min_count = type_count[i];
            min_id = i;
        }
    }

    // 5. return type
    *type = knn_type[ min_id ];

    // 6. print middle result
    printf("After normalize : ");
    for( i = 0; i < knn_vctr_len; ++i ){
        printf("%lf,", _vctr.elem[i]);
    }
    printf("\n");
    for( i = 0; i < knn_K; ++i ){
        printf("distance with data[%2lu] is %lf , type_id = %lu\n",
                vctr_id[i],
                distance[i],
                knn_type_id[ vctr_id[i] ]
        );
    } 
}

/**
  Set the essential value K for this kNN struct.

  This function can be called more than once.

  @param knn            The pointer to ehis kNN struct.

  @param K              The value K for kNN alg    

**/
void 
kNN_set_K(
    IN kNN         *knn,
    IN size_t      K
    )
{
    if( knn == NULL ) return;

    knn->K = K;
}

/**
  Print the data of thie kNN struct 

  @param[in] knn          The pointer to this kNN struct

NOTE:
  If kNN->has_dataset if false, then print waring

**/
void
kNN_pr_data(
    IN kNN *knn
)
{
    if( knn == NULL ) return;

    if( knn->has_dataset == 0 ){
        perror("[WARNING] no dataset inside this kNN struct");
        return;
    }

    size_t       data_nr     =   knn->dataset.data_size;
    size_t       vctr_l      =   knn->dataset.data_vctr_len;
    kNN_vector  *vctr_s      =   knn->dataset.data_vctr_s;
    size_t      *type_id_s   =   knn->dataset.data_type_id;
    kNN_type    *type_s      =   knn->dataset.type;
    size_t       type_nr     =   knn->dataset.type_nr;
    kNN_vector  *min_vctr    =  &knn->data_min_vctr;
    kNN_vector  *max_vctr    =  &knn->data_max_vctr;
    size_t       i           =   0;
    size_t       v           =   0;

    printf("----------------\n");
    for( i = 0; i < type_nr; ++i ){
        printf("Type[%2lu] id = %2d , str = %s\n", i, type_s[i].type_id, type_s[i].type_str );
    }
    printf("----------------\n");
    for( i = 0; i < data_nr; ++i ){
        printf("Data[%2lu] type_id = %2lu , ", i, type_id_s[i] );
        for( v = 0; v < vctr_l; ++v ){
            printf(" %lf," , vctr_s[i].elem[v]);
        }
        printf("\n");
    }
    printf("----------------\n");
    printf("Min vector = ");
    for( v = 0; v < vctr_l; ++v ){
        printf(" %lf," , min_vctr->elem[v]);
    }
    printf("\n");
    printf("Max vector = ");
    for( v = 0; v < vctr_l; ++v ){
        printf(" %lf," , max_vctr->elem[v]);
    }
    printf("\n");
}

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
)
{
    if( knn == NULL ) return;

    size_t     i     =  0;
    kNN_elem_t elem  =  0;

    for( i = 0; i < knn->dataset.data_vctr_len; ++i ){
        elem = vctr->elem[i];
        elem = ( elem - knn->data_min_vctr.elem[i] ) / knn->data_dif_vctr.elem[i];
        _vctr->elem[i] = elem;
    }
}

















