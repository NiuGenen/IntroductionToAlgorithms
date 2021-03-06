#include <stdio.h>

/* build for miniminze */

#define TGT_MAX -1
#define TGT_MIN 1

#define MAX_INT 0x7FFFFFFFFFFFFFFF

#define MAX_C  2000
#define MAX_ST 300

double c[MAX_C];
double oc[MAX_C];
double c_bak[MAX_C];
double A[MAX_ST][MAX_C];
double b[MAX_ST];
double x[MAX_C];
int    B[MAX_C];
int    bB[MAX_ST];
double z;

int ci = 0;
int min_bi  = -1;
int min_sti = -1;

int st_nr   = 0;
int var_nr  = 0;
int svar_nr = 0;
int tgt_t   = 1; // [1] min [-1] max


void print()
{
    /*int i = 0;*/
    /*int j = 0;*/

    /*printf("       ");*/
    /*for( i = 0; i < svar_nr; ++i ){*/
        /*if( B[i] ) printf(" B = %d  ", B[i]);*/
        /*else printf(" B = 0 ");*/
    /*}*/
    /*printf("\n");*/

    /*printf("  c[]  ");*/
    /*for( i = 0; i < svar_nr; ++i){*/
        /*if( c[i] >= 0 ) printf(" ");*/
        /*printf("%.2lf   ", c[i]);*/
    /*}*/
    /*printf(" z = %.2lf\n", z);*/

    /*for( i = 0; i < st_nr; ++i ){*/
        /*printf("st[%2d] ", i);*/
        /*for( j = 0; j < svar_nr; ++j ){*/
            /*if( A[i][j] >= 0 ) printf(" ");*/
            /*printf("%.2lf   ", A[i][j]);*/
        /*}*/
        /*printf(" b[%d] = %.2lf , bB[%2d] = %2d\n", i, b[i], i, bB[i]);*/
    /*}*/

    /*printf("       ");*/
    /*for( i = 0; i < svar_nr; ++i ){*/
        /*printf(" x[%2d]  ", i);*/
    /*}*/
    /*printf("\n");*/

    /*printf("       ");*/
    /*for( i = 0; i < svar_nr; ++i ){*/
        /*if( x[i] >= 0 ) printf(" ");*/
        /*printf("%.2lf   ", x[i]);*/
    /*}*/
    /*printf("\n");*/

    /*printf("------------------------------------\n");*/
}


void set_var_num(int n) { var_nr = n; }

void tgt_set( int t ) { tgt_t = t; }
void tgt_add(int idx, double v) { c_bak[idx] = oc[idx] = c[idx] = tgt_t * v; }

void st_next() { st_nr += 1; }
void st_add_c(int idx, double v) { A[st_nr][idx] = v; }
void st_add_b(double v) { b[st_nr] = v; }

int  aux_simplex();
int  simplex_goon();
void pivot(int in, int out, int out_st);
void construct_result();

int aux_simplex()
{
    int i = 0;
    int j = 0;
    int k = 0;
    double n;
    int bi_min_st = 0;
    int x0i = 0;

    svar_nr = var_nr + st_nr; // with slack 
    x0i = svar_nr;

    // add slack var
    for( i = 0, j = var_nr; i < svar_nr; ++i, ++j ){ // 3,4,5,6
        A[ i ][ j ] = 1;
    }
    for( i = 0; i < var_nr; ++i )
        B[i] = 0;
    for( ; i < svar_nr; ++i ){
        B[i] = 1 + i - var_nr;
        bB[i - var_nr] = i;
        c_bak[i] = oc[i] = c[i]  = 0;
    }

    print();

    // add x0
    for( i = 0; i < st_nr; ++i ){
        A[i][ x0i ] = -1;
    }
    // change target function
    for( i = 0; i < svar_nr; ++i )
        oc[i] = c[i] = 0;
    oc[i] = c[ x0i ] = 1;
    B[ x0i ] = 0;

    svar_nr += 1;

    // find s.t. with minimize b[i]
    bi_min_st = 0;
    for( i = 1; i < st_nr; ++i )
        if( b[i] < b[bi_min_st] )
            bi_min_st = i;

    pivot( x0i, bB[ bi_min_st ], bi_min_st );
    construct_result();

    print();
    // perform simplex 
    while( simplex_goon() ){
        pivot(ci, min_bi, min_sti);
        construct_result();
        print();
    }

    if( z != 0 ){
        return 0;
    }

    // remove x0i 
    if( B[x0i] != 0 ){
        // find it's s.t.[i] 
        for( i = 0; i < st_nr; ++i )
            if( A[i][x0i] == 1 )
                break;
        // find another non-bias var
        for( j = 0; j < svar_nr - 1; ++j ){
            if( A[i][j] != 0 && B[j] != 0 )
                break;
        }
        pivot( j, x0i , i );
    }

    svar_nr -= 1;
    for( i = 0; i< svar_nr; ++i )
        oc[i] = c[i] = c_bak[i];
    for( i = 0; i < svar_nr; ++i ){
        if( B[i] != 0 && c[i] != 0 ){
            for( j = 0; j < st_nr; ++j )
                if( A[j][i] == 1 )
                    break;
            n = c[i];
            for( k = 0; k < svar_nr; ++k ){
                c[k] -= n * A[j][k];
                /*oc[k] = c[k];*/
            }
        }
    }

    return 1;
}

int init_simplex()
{
    svar_nr = var_nr + st_nr; // with slack 
    
    int st_idx = 0, svar_idx = 0;
    int i = 0;

    int bi_all_ok = 1;
    for( i = 0; i < st_nr; ++i )
        if( b[i] < 0 ){
            bi_all_ok = 0;
            break;
        }

    if( !bi_all_ok ){
        return aux_simplex();
    }

    // add slack var
    for( st_idx = 0, svar_idx = var_nr; svar_idx < svar_nr; ++svar_idx, ++st_idx ){ 
        A[ st_idx ][ svar_idx ] = 1;
    }
    
    for( i = 0; i < var_nr; ++i ){
        x[i] = 0; B[i] = 0;
    }
    for( ; i < svar_nr; ++i ){
        x[i] = b[i - var_nr];
        B[i] = 1 + i - var_nr;
        bB[i - var_nr] = i;
        c[i]  = 0;
        oc[i] = 0;
    }
    return 1;
}

void pivot(int in, int out, int out_st)
{
    B[in]  = B[out];
    B[out] = 0;
    bB[out_st] = in;

    int j = 0;
    double n = 0;
    n = A[out_st][in];
    for( j = 0; j < svar_nr; ++j )
        A[out_st][j] /= n;
    b[out_st] /= n;

    int i = 0;
    for( i = 0; i < st_nr; ++i ){
        if( i == out_st )
            continue;
        n = A[i][in];
        for( j = 0; j < svar_nr; ++j ){
            A[i][j] -= n * A[out_st][j];
        }
        b[i] -= n * b[out_st];
    }
    n = c[in];
    for( j = 0; j < svar_nr; ++j ){
        c[j] -= n * A[out_st][j];
    }
}

int simplex_goon()
{
    ci = 0;
    for( ci = 0; ci < svar_nr; ++ci ){
        if( B[ci] == 0 && c[ci] < 0 )
            break;
    }
    if( ci == svar_nr ){
        return 0;
    }
    int sti = -1;
    double theta = 0;
    double min_theta = MAX_INT;
    min_bi    = -1;
    min_sti   = -1;
    for( sti = 0; sti < st_nr; ++sti ){
        if( A[sti][ci] <= 0 ) 
            continue;
        theta = b[sti] / A[sti][ci];
        if( theta >= 0 && 
            ( theta < min_theta || (theta == min_theta && B[ bB[sti] ] < B[ min_bi ]) ) ){
            min_theta = theta;
            min_bi  = bB[sti];
            min_sti = sti;
        }
    }
    if( min_bi == -1 ){
        return 0;
    }
    return 1;
}

void construct_result()
{
    int i = 0;

    for( i = 0; i < svar_nr; ++i )
        x[i] = 0;

    for( i = 0; i < st_nr; ++i ) {
        x[ bB[i] ] = b[i];
    }

    z = 0;
    for( i = 0; i < svar_nr; ++i ){
        z += x[i] * oc[i];
        /*printf("%.2f ", oc[i]);*/
    }
    /*printf("\n");*/
}

long v_day[300];
int v_day_nr;
long v_vltr_S[2000];
long v_vltr_F[2000];
long v_vltr_C[2000];
int v_vltr_nr;

int is_this_vltr_ok(long day, long st, long ed)
{
    if(st <= day && day <= ed)
        return 1;
    return 0;
}

void volunteer()
{
    int i = 0;
    int j = 0;

    scanf("%d", &v_day_nr);
    scanf("%d", &v_vltr_nr);

    for( i = 0; i < v_day_nr; ++i )
        scanf("%ld", v_day + i);

    for( j = 0; j < v_vltr_nr; ++j ){
        scanf("%ld", v_vltr_S + j);
        scanf("%ld", v_vltr_F + j);
        scanf("%ld", v_vltr_C + j);
    }

    set_var_num( v_vltr_nr );

    tgt_set( TGT_MIN );
    for( i = 0; i < v_vltr_nr; ++i )
        tgt_add( i, v_vltr_C[i] );

    for( i = 0; i < v_day_nr; ++i ){
        for( j = 0; j < v_vltr_nr; ++j ){
            st_add_c( j, -1 * is_this_vltr_ok(i+1, v_vltr_S[j], v_vltr_F[j] ) );
        }
        st_add_b( -1 * v_day[i] );
        st_next();
    }
}

void print_oj( )
{
    printf("%ld\n", (long)z );
}

int main()
{
    volunteer();
    // init
    if( !init_simplex() ){
        return 1;
    }
    // simplex 
    print();
    while( simplex_goon() ){
        pivot(ci, min_bi, min_sti);
        construct_result();
        print();
    }
    construct_result();

    print_oj();

    return 0;
}
