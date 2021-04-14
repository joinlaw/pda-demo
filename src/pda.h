#ifndef PDA_H
#define PDA_H

#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#define XFAR_CENTER (coordinates.x-center.x)
#define YFAR_CENTER (coordinates.y-center.y)

static void draw_func (GtkDrawingArea *drawing_area, cairo_t *cr,
		       int width, int height, gpointer user_data);

static void moshi_cb (GtkEventControllerMotion *controller, double x,
		      double y, gpointer user_data);

static void change_slices (GtkSpinButton *spin_button,
			   gpointer user_data);

static void change_radius (GtkSpinButton *spin_button,
			   gpointer user_data);

static void change_rotation (GtkSpinButton *spin_button,
			     gpointer user_data);

static void change_blocky (GtkSpinButton *spin_button,
			   gpointer user_data);

static void clicky_cb (GtkGestureClick *gesture, int n_press,
		       double x, double y, gpointer user_data);

void activate (GtkApplication *application, gpointer user_data);

#endif /* PDA_H */
