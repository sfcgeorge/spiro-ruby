#include <stdlib.h>
#include "bezctx_rb.h"

Splines splines;

void initSplines(Splines *a, size_t initialSize) {
  a->array = (node *)malloc(initialSize * sizeof(node));
  a->used = 0;
  a->size = initialSize;
}

void appendSplines(Splines *a, node element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (node *)realloc(a->array, a->size * sizeof(node));
  }
  a->array[a->used++] = element;
}

void freeSplines(Splines *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

typedef struct {
  bezctx base; // This is a superclass of bezctx, entry for the base
  int is_open;
} bezctx_rb;

/* This routine starts a new contour */
static void bezctx_rb_moveto(bezctx *z, double x, double y, int is_open) {
  bezctx_rb *bc = (bezctx_rb *)z;

  bc->is_open = is_open;

  node n = { x, y, SPLINE_CORNER };
  appendSplines(&splines, n);
}

/* This routine creates a linear spline from the previous point specified to this one */
void bezctx_rb_lineto(bezctx *z, double x, double y) {
  bezctx_rb *bc = (bezctx_rb *)z;

  node n = { x, y, SPLINE_CORNER };
  appendSplines(&splines, n);
}

/* This creates a qubic curve */
void bezctx_rb_quadto(bezctx *z, double xm, double ym, double x3, double y3) {
  bezctx_rb *bc = (bezctx_rb *)z;

  node n1 = { xm, ym, SPLINE_QUADRATIC };
  appendSplines(&splines, n1);

  node n2 = { x3, y3, SPLINE_CORNER };
  appendSplines(&splines, n2);
}

/* And this creates a cubic */
void bezctx_rb_curveto(bezctx *z, double x1, double y1, double x2, double y2,
    double x3, double y3) {
  bezctx_rb *bc = (bezctx_rb *)z;

  node n1 = { x1, y1, SPLINE_CUBIC };
  appendSplines(&splines, n1);
  node n2 = { x2, y2, SPLINE_CUBIC };
  appendSplines(&splines, n2);
  node n3 = { x3, y3, SPLINE_CORNER };
  appendSplines(&splines, n3);
}

/* Allocates and initializes a new bezier context */
bezctx * new_bezctx_rb(void) {
  bezctx_rb *result = (bezctx_rb *)malloc(sizeof(bezctx_rb));
  initSplines(&splines, 20);

  result->base.moveto = bezctx_rb_moveto;
  result->base.lineto = bezctx_rb_lineto;
  result->base.quadto = bezctx_rb_quadto;
  result->base.curveto = bezctx_rb_curveto;
  result->base.mark_knot = NULL;
  result->is_open = 0;
  return &result->base;
}

/* Finishes an old bezier context */
void bezctx_rb_close(bezctx *z) {
  bezctx_rb *bc = (bezctx_rb *)z;
  free(bc);
}
