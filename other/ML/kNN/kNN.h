/*
 * basic kNN alg
 *
 * Author: niugen
 *
 * GitHub: NiuGenen
 *
 * 2017/12/16
 *
 */

#ifndef _NG_KNN_H_
#define _NG_KNN_H_

#include <sys/types.h>

struct knn_vector{
    void * elem;
    size_t len;
};

struct knn_type{
    int type_id;
    char type_str[32];
};

struct knn_dataset{
    struct knn_vector * data;
    struct knn_type   * type;
    size_t size;
}

struct knn{
    struct knn_dataset data_training;
    // operations
    void (*reset_training_data)( struct knn * knn, struct knn_dataset * data );
    struct knn_type (*identy)( struct knn * knn, struct knn_vector * vctr );
};

struct knn * knn_alloc();
void knn_free( struct knn * knn , int free_data);

#endif
