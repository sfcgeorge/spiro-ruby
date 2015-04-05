#include <ruby.h>
#include "spiroentrypoints.h"	// call spiro through here
#include "bezctx_rb.h"		// bezctx structure
#include "_spiro.h"

static ID id_corner;
static ID id_g4;
static ID id_g2;
static ID id_left;
static ID id_right;

static inline char sym_to_spiro_type(VALUE sym) {
  ID inp = rb_to_id(sym);

  if (inp == id_corner) {
    return SPIRO_CORNER;
  } else if (inp == id_g2) {
    return SPIRO_G2;
  } else if (inp == id_g4) {
    return SPIRO_G4;
  } else if (inp == id_left) {
    return SPIRO_LEFT;
  } else if (inp == id_right) {
    return SPIRO_RIGHT;
  }

  rb_raise(rb_eArgError, "%s", "Expected :g2, :g4, :node, :left or :right");
}

static inline VALUE spline_type_to_sym(int ty) {
  if (ty == SPLINE_CORNER) {
    return ID2SYM(rb_intern("node"));
  } else if (ty == SPLINE_CUBIC) {
    return ID2SYM(rb_intern("cubic"));
  } else if (ty == SPLINE_QUADRATIC) {
    return ID2SYM(rb_intern("quadratic"));
  }
}

static inline int bool_to_int(VALUE b) {
  switch (TYPE(b)) {
    case T_TRUE:
      return 1;
      break;
    case T_FALSE:
      return 0;
      break;
    default:
      rb_raise(rb_eTypeError, "not a boolean");
      break;
  }
}

static VALUE spiros_to_splines(VALUE mod, VALUE spirosValue, VALUE closedValue) {
  Check_Type(spirosValue, T_ARRAY);

  // Take Ruby array of points and turn into Spiro C struct array
  spiro_cp spiros[RARRAY_LEN(spirosValue)];
  for (int i = 0; i != RARRAY_LEN(spirosValue); i++) {
    VALUE nodeValue = rb_ary_entry(spirosValue, i);
    // Validate node is an array and length 3 (x, y, type)
    Check_Type(nodeValue, T_ARRAY);
    if (RARRAY_LEN(nodeValue) != 3) rb_raise(rb_eArgError, "%s", "Invalid node");

    // Validate individual node elements
    VALUE xValue = rb_ary_entry(nodeValue, 0);
    if (TYPE(xValue) != T_FIXNUM && TYPE(xValue) != T_FLOAT) {
      rb_raise(rb_eTypeError, "x coord must be float or fixnum");
    }
    VALUE yValue = rb_ary_entry(nodeValue, 1);
    if (TYPE(yValue) != T_FIXNUM && TYPE(yValue) != T_FLOAT) {
      rb_raise(rb_eTypeError, "y coord must be float or fixnum");
    }
    VALUE typeValue = rb_ary_entry(nodeValue, 2);
    Check_Type(typeValue, T_SYMBOL);

    spiros[i].x  = (long)(NUM2DBL(xValue));
    spiros[i].y  = (long)(NUM2DBL(yValue));
    spiros[i].ty = sym_to_spiro_type(typeValue);
  }

  int closed = bool_to_int(closedValue);

  // bezctx_rb is custom, stores the result of running Spiro
  bezctx *bc = new_bezctx_rb();
  // Run Spiro on the Spiros array and populate custom struct with splines array
  int success = SpiroCPsToBezier0(spiros, RARRAY_LEN(spirosValue), closed, bc);

  if (success) {
    // Build Ruby array from Spiro array
    VALUE splinesValue = rb_ary_new();
    // Free memory from the custom struct
    bezctx_rb_close(bc);
    for (int i = 0; i < splines.used; i++) {
      VALUE x = rb_float_new(splines.array[i].x);
      VALUE y = rb_float_new(splines.array[i].y);
      VALUE ty = spline_type_to_sym(splines.array[i].ty);
      VALUE nodeValue = rb_ary_new3(3, x, y, ty);
      rb_ary_push(splinesValue, nodeValue);
    }
    freeSplines(&splines);

    return splinesValue;
  }
  else {
    bezctx_rb_close(bc);
    freeSplines(&splines);
    return Qnil;
  }
}

void Init_spiro(void) {
  id_corner = rb_intern("node");
  id_g4 = rb_intern("g4");
  id_g2 = rb_intern("g2");
  id_left = rb_intern("left");
  id_right = rb_intern("right");

  VALUE mSpiro = rb_define_module("Spiro");
  rb_define_singleton_method(mSpiro, "spiros_to_splines", spiros_to_splines, 2);
}
