#include "kNN.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "bb.h"

kNN_dataset dataset = {
    NULL,  // *data_vctr_s
    NULL,  // *data_type_id
    0,    // data_size
    0,     // data_vctr_len
    NULL,  // type
    0,     // type_nr
    NULL,  // test_data_vctr_s
    NULL,  // test_data_type_id
    0      // test_data_size
};

#define SECTION_MAX_LEN 32
#define SECTION_NR 7
const char * section_name[ SECTION_NR ] = {
    "tpnr",  // 0 : number of type 
    "type",  // 1 : type data 
    "vlen",  // 2 : vector length
    "dtnr",  // 3 : number of training data 
    "data",  // 4 : trainaing data 
    "tlen",  // 5 : number of test data 
    "test"   // 6 : test data
};

int _cmp_section_name( char * name )
{
    size_t i = 0, j = 0;

    for( i = 0; i < SECTION_NR; ++i ){
        for( j = 0; j < 4; ++j ){
            if( name[j] != section_name[i][j] ) break;
        }
        if( j != 4 ) continue;
        return i;
    }
    return -1;
}

#define ERROR_MAX_LEN 1024

void process_0_type_nr( struct bb_rw * bb , kNN_dataset *dataset);
void process_1_type_data( struct bb_rw * bb , kNN_dataset *dataset);
void process_2_vctr_len( struct bb_rw * bb , kNN_dataset *dataset);
void process_3_trainging_data_nr( struct bb_rw * bb , kNN_dataset *dataset);
void process_4_training_data( struct bb_rw * bb , kNN_dataset *dataset);
void process_5_test_data_nr( struct bb_rw * bb , kNN_dataset *dataset);
void process_6_test_data( struct bb_rw * bb , kNN_dataset *dataset);

typedef void (*process_section_fn)( struct bb_rw *bb , kNN_dataset *dataset);

process_section_fn process_fn[7] = {
    process_0_type_nr,
    process_1_type_data,
    process_2_vctr_len,
    process_3_trainging_data_nr,
    process_4_training_data,
    process_5_test_data_nr,
    process_6_test_data
};

void process_section( int section_id, struct bb_rw *bb, kNN_dataset *dataset )
{
    // debug
    printf("%-5d : process section[%d] : %s\n", __LINE__,  section_id, section_name[ section_id ]);
    // switch 
    /*switch( section_id ){*/
        /*case 0 : break; // number of type*/
        /*case 1 : break; // type data */
        /*case 2 : break; // vector length*/
        /*case 3 : break; // number of training data*/
        /*case 4 : break; // training data  */
        /*case 5 : break; // number of test data */
        /*case 6 : break; // test data*/
    /*}*/
    process_fn[ section_id ]( bb , dataset);
}

int main()
{
    size_t        i             =  0;
    size_t        j             =  0;
    int           if_cpy        =  1;
    int           if_free_data  =  1;
    int           fd            =  0;
    int           bb_rd         =  1;
    int           bb_bit        =  0;
    size_t        bb_buf_size   =  1024; 
    struct bb_rw  *bb           =  NULL;
    uint16_t      c             =  0;
    char          section[SECTION_MAX_LEN + 1];
    int           section_id    =  -1;
    kNN_type      test_type     = { 0, "\0" };
    char          error[ ERROR_MAX_LEN ];

    // open data file 
    fd = open( "./kNN.data", O_RDONLY, 0666 );
    if( fd < 0 ){
        sprintf( error, "%-5d : [ERROR] fail to open data file %s", __LINE__, "./kNN.data" );
        perror( error );
        exit(-1);
    }

    // build bb struct 
    bb = bb_rw_alloc( fd, bb_rd, bb_bit, bb_buf_size );

    // build kNN struct 
    kNN * knn = kNN_alloc();

    // read data from file
    while( !bb->rd_end && c != 256 ){
        c = bb->rd_byte( bb );
        // always start with one section
        if( c != '#' ) continue;
        if( c == 256 ) break;
        // read section name
        i = 0;
        c = bb->rd_byte( bb );
        while( c != '#' && i < SECTION_MAX_LEN && c != 256 ){
            section[i] = c;
            i += 1;
            c = bb->rd_byte( bb );
        }
        if( c == 256 ) break;
        section[i] = '\0';
        while( c != '#' && c != 256 ) c = bb->rd_byte(bb);
        if( c == 256 ) break;
        // debug
        printf("%-5d : Section: %s\n", __LINE__, section);
        // get section id 
        section_id = _cmp_section_name( section );
        if( section_id < 0 ){
            sprintf( error, "%-5d : [WARNING] Section \"%s\" is not supported. ", __LINE__, section );
            perror( error );
            continue;
        }
        // process section 
        process_section( section_id, bb , &dataset);
    }
    if( c == 256 ){
        // bad read 
        sprintf( error, "%-5d : [ERROR] fail to process file : bad charactor" , __LINE__);
        perror( error );
        exit(-2);
    }

    // set dataset 
    knn->op_set_dataset( knn, &dataset, if_cpy );

    // print data 
    knn->op_pr_data( knn );

    // test data 
    printf("------ running test ------\n");
    for( i = 0; i < dataset.test_data_size; ++i ){
        printf("Vector : ");
        for( j = 0; j < dataset.data_vctr_len; ++j ){
            printf(" %lf,", dataset.test_data_vctr_s[i].elem[j]);
        }
        printf("\n");
        printf("Original type : id = %lu, str = %s\n", dataset.test_data_type_id[i], dataset.type[ dataset.test_data_type_id[i] ].type_str );
        knn->op_identy( knn, &(dataset.test_data_vctr_s[i]), &test_type);
        printf("Identity type : id = %d, str = %s\n", test_type.type_id, test_type.type_str);
        printf("\n");
    }

    // free kNN 
    kNN_free( knn , if_free_data );
}

void process_0_type_nr( struct bb_rw * bb , kNN_dataset *dataset)
{
    char c = 0;
    char buf[32];
    int buf_i = 0;
    int type_nr = 0;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // get number
    c = bb->rd_byte( bb );
    while( c != '\n' ){
        buf[ buf_i++ ] = c;
        c = bb->rd_byte( bb );
    }
    if( buf_i == 0 ) buf[ buf_i++ ] = '0';
    buf[ buf_i ] = '\0';
    // convert string into number
    type_nr = atoi( buf );
    printf("%-5d : get type_nr = %d\n", __LINE__, type_nr);
    // store into dataset
    dataset->type_nr = type_nr;
}

void process_1_type_data( struct bb_rw * bb , kNN_dataset *dataset)
{
    char    c        =  0;
    char    buf[32]  =  {'0'};
    int     buf_i    =  0;
    int     type_id  =  0;
    size_t  i        =  0;
    size_t  type_nr  =  dataset->type_nr;
    char    str[ KNN_TYPE_STR_LEN ] = {'\0'};
    int     str_i    =  0;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // get type data 
    dataset->type = _MALLOC_( kNN_type, type_nr );
    while( i < type_nr ){
        i += 1;
        // get type_id
        buf_i = 0;
        c = bb->rd_byte( bb );
        while( c != ',' ){
            buf[ buf_i++ ] = c;
            c = bb->rd_byte( bb );
        }
        buf[ buf_i ] = '\0';
        // convert string into number
        type_id = atoi( buf );
        printf("%-5d : get type_id = %d\n", __LINE__, type_id);
        // get type str 
        str_i = 0;
        c = bb->rd_byte( bb );
        while( c != '\n' ){
            str[ str_i++ ] = c;
            c = bb->rd_byte( bb );
        }
        str[ str_i ] = '\0';
        printf("%-5d : get type name = %s\n", __LINE__, str);
        // store into dataset
        dataset->type[ type_id ].type_id = type_id;
        memcpy( dataset->type[ type_id ].type_str, str, KNN_TYPE_STR_LEN );
    }
}

void process_2_vctr_len( struct bb_rw * bb , kNN_dataset *dataset)
{ 
    char  c        =  0;
    char  buf[32]  =  {'\0'};
    int   buf_i    =  0;
    int   vctr_len =  0;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // get number
    c = bb->rd_byte( bb );
    while( c != '\n' ){
        buf[ buf_i++ ] = c;
        c = bb->rd_byte( bb );
    }
    if( buf_i == 0 ) buf[ buf_i++ ] = '0';
    buf[ buf_i ] = '\0';
    // convert string into number
    vctr_len = atoi( buf );
    printf("%-5d : get vector length = %d\n", __LINE__, vctr_len );
    // store into dataset
    dataset->data_vctr_len = vctr_len;
}

void process_3_trainging_data_nr( struct bb_rw * bb , kNN_dataset *dataset)
{ 
    char  c        =  0;
    char  buf[32]  =  {'\0'};
    int   buf_i    =  0;
    int   data_nr  =  0;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // get number
    c = bb->rd_byte( bb );
    while( c != '\n' ){
        buf[ buf_i++ ] = c;
        c = bb->rd_byte( bb );
    }
    if( buf_i == 0 ) buf[ buf_i++ ] = '0';
    buf[ buf_i ] = '\0';
    // convert string into number
    data_nr = atoi( buf );
    printf("%-5d : get training data nr = %d\n", __LINE__, data_nr );
    // store into dataset
    dataset->data_size = data_nr;

}

void process_4_training_data( struct bb_rw * bb , kNN_dataset *dataset)
{
    char    c        =  0;
    char    buf[32]  =  {'0'};
    int     buf_i    =  0;
    size_t  i        =  0;
    size_t  v        =  0;
    int     v_elem   =  0;
    size_t  vctr_len =  dataset->data_vctr_len;
    int     type_id  =  0;
    size_t  data_nr  =  dataset->data_size;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // alloc mem
    dataset->data_vctr_s  = _MALLOC_( kNN_vector, data_nr );
    dataset->data_type_id = _MALLOC_( size_t    , data_nr );
    //
    // get training data 
    // 
    // [vector] [type_id]
    // 
    while( i < data_nr ){
        // get vector
        dataset->data_vctr_s[i].elem = _MALLOC_( kNN_elem_t, vctr_len );
        v        = 0;
        v_elem   = 0;
        while( v < vctr_len ){
            buf_i = 0;
            c = bb->rd_byte( bb );
            while( c != ',' ){
                buf[ buf_i++ ] = c;
             c = bb->rd_byte( bb );
            }
            buf[ buf_i ] = '\0';
            // convert string into number
            v_elem = atoi( buf );
            dataset->data_vctr_s[i].elem[ v ] = v_elem;
            v++;
        }
        printf("%-5d : get vector = ", __LINE__ );
        for( v = 0; v < vctr_len; ++v )
            printf(" %d,", (int)dataset->data_vctr_s[i].elem[v]);
        printf("\b\n");
        // get type str 
        buf_i = 0;
        c = bb->rd_byte( bb );
        while( c != '\n' ){
            buf[ buf_i++ ] = c;
            c = bb->rd_byte( bb );
        }
        buf[ buf_i ] = '\0';
        type_id = atoi( buf );
        printf("%-5d : get type id = %d\n", __LINE__, type_id );
        // store into dataset 
        dataset->data_type_id[i] = type_id;
        // loop
        i++;
    }
}

void process_5_test_data_nr( struct bb_rw * bb , kNN_dataset *dataset)
{
    char c           =  0;
    char buf[32]     =  {'\0'};
    int buf_i        =  0;
    int test_data_nr =  0;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // get number
    c = bb->rd_byte( bb );
    while( c != '\n' ){
        buf[ buf_i++ ] = c;
        c = bb->rd_byte( bb );
    }
    if( buf_i == 0 ) buf[ buf_i++ ] = '0';
    buf[ buf_i ] = '\0';
    // convert string into number
    test_data_nr = atoi( buf );
    printf("%-5d : get test_data_nr = %d\n", __LINE__, test_data_nr );
    // store into dataset
    dataset->test_data_size = test_data_nr;
}

void process_6_test_data( struct bb_rw * bb , kNN_dataset *dataset)
{
    char    c        =  0;
    char    buf[32]  =  {'0'};
    int     buf_i    =  0;
    size_t  i        =  0;
    size_t  v        =  0;
    int     v_elem   =  0;
    size_t  vctr_len =  dataset->data_vctr_len;
    int     type_id  =  0;
    size_t  data_nr  =  dataset->test_data_size;
    // skip section name line
    c = bb->rd_byte( bb );
    while( c != '\n' ) c = bb->rd_byte( bb );
    // alloc mem
    dataset->test_data_vctr_s  = _MALLOC_( kNN_vector, data_nr );
    dataset->test_data_type_id = _MALLOC_( size_t    , data_nr );
    //
    // get training data 
    // 
    // [vector] [type_id]
    // 
    while( i < data_nr ){
        // get vector
        dataset->test_data_vctr_s[i].elem = _MALLOC_( kNN_elem_t, vctr_len );
        v        = 0;
        v_elem   = 0;
        while( v < vctr_len ){
            buf_i = 0;
            c = bb->rd_byte( bb );
            while( c != ',' ){
                buf[ buf_i++ ] = c;
             c = bb->rd_byte( bb );
            }
            buf[ buf_i ] = '\0';
            // convert string into number
            v_elem = atoi( buf );
            dataset->test_data_vctr_s[i].elem[ v ] = v_elem;
            v++;
        }
        printf("%-5d : get vector = ", __LINE__ );
        for( v = 0; v < vctr_len; ++v )
            printf(" %d,", (int)dataset->test_data_vctr_s[i].elem[v]);
        printf("\b\n");
        // get type str 
        buf_i = 0;
        c = bb->rd_byte( bb );
        while( c != '\n' ){
            buf[ buf_i++ ] = c;
            c = bb->rd_byte( bb );
        }
        buf[ buf_i ] = '\0';
        type_id = atoi( buf );
        printf("%-5d : get type id = %d\n", __LINE__, type_id );
        // store into dataset 
        dataset->test_data_type_id[i] = type_id;
        // loop
        i++;
    }
}
