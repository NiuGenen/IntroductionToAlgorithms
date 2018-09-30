#define fp_t double

//#define DISTANCE(p,x,y) ( (p[2*(x)]-p[2*(y)])*(p[2*(x)]-p[2*(y)]) + (p[2*(x)+1]-p[2*(y)+1])*(p[2*(x)+1]-p[2*(y)+1]) )
#define DISTANCE(px,py,a,b) ( (px[a]-px[b])*(px[a]-px[b]) + (py[a]-py[b])*(py[a]-py[b]) )
#define MIN(x,y) ( ( (x)<(y) )?( x ):( y ) )

//fp_t closest_pair( fp_t *p, int n, fp_t *p_cp);
//fp_t native_closest_pair( fp_t *p, int n );

fp_t closest_pair( fp_t *p_x, fp_t *p_y, int n, fp_t *p_cp_x, fp_t *p_cp_y);
fp_t native_closest_pair( fp_t *p_x, fp_t *p_y, int n );
