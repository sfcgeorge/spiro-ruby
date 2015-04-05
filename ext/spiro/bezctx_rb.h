#include "spiroentrypoints.h"
#include "bezctx.h"

// Possible values of the "ty" field.
#define SPLINE_CORNER	1
#define SPLINE_CUBIC 3
#define SPLINE_QUADRATIC 4

typedef struct {
  double x;
  double y;
  int ty;
} node;

typedef struct Splines {
  node *array;
  size_t used;
  size_t size;
} Splines;

extern Splines splines;


bezctx *new_bezctx_rb(void);

//struct splineset;
//struct Splines;

void freeSplines(Splines *a);

//struct splineset *bezctx_rb_close(bezctx *bc);
void bezctx_rb_close(bezctx *z);
