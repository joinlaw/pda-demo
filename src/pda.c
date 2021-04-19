/*
 * Copyright (C) 2021  mr.fantastic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "pda.h"

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


typedef struct 
{
  double x, y;
} coordinate;

typedef struct 
{
  double segments, radius, exclude_radius, rotation;
} pizza;

// since they are static (private because they are outside any
// function) they will be initialized to 0 by default in any C/C++
// compiler
static coordinate coordinates;
static coordinate center = {200, 200}; // Initialize to default values
static pizza margherita = 
  {
    .segments		= 12,		/* 12 Pizza slice by default */
    .radius		= 200, 		/* Default radius */
    .exclude_radius	= 0,		/* Disabled by default  */
    .rotation		= 0,			/* Disabled by default  */
  };

static gboolean meat = TRUE;
static double meats[9][2];

static void draw_func (GtkDrawingArea *drawing_area, cairo_t *cr,
		       int width, int height, gpointer user_data)
{  
  double distX = coordinates.x - center.x,
    	 distY = coordinates.y - center.y,
	 distance = sqrt( (distX*distX) + (distY*distY) );

  char colors[(int) margherita.segments]; int i; for (i = 0; i < margherita.segments; ++i) colors[i]=0;
  
  double val = radToDeg(atan2(YFAR_CENTER - 0, XFAR_CENTER - 0));
  if (val < 0) val+=360.0;
  
  // if the distance is less than the circle's radius the point is inside!
  if (distance <= margherita.radius && distance >= margherita.exclude_radius)
    {
      int kk;
      for (kk = 0; kk < margherita.segments; ++kk)
	{
	  if (val >= (kk*(360.0/margherita.segments) + margherita.rotation)
	      && val <= ((kk+1)*(360.0/margherita.segments) + margherita.rotation) /* FIXME: Known rotation bug */
	      )
	    {
	      colors[kk] = 1;
	      break;
	    }
	}
    }
  
  int count = 0;
  for (i = 0; i < margherita.segments; ++i)
    {

      // start with outer circle
      cairo_move_to (cr, center.x, center.y);
      cairo_arc (cr, center.x, center.y, margherita.radius,
		 (count+margherita.rotation) * (M_PI/180), (count+360.0/margherita.segments + margherita.rotation) * (M_PI/180));
      cairo_set_source_rgb(cr, 0.8, 0.5, 0.2);
      cairo_fill(cr);
      cairo_close_path (cr);

      // pizza cuts
      cairo_move_to (cr, center.x, center.y);
      cairo_arc (cr, center.x, center.y, margherita.radius,
		 (count+margherita.rotation) * (M_PI/180), (count+360.0/margherita.segments + margherita.rotation) * (M_PI/180));
      cairo_set_source_rgb(cr,0,0,0);
      cairo_close_path (cr);
      cairo_stroke(cr);

      // cheese
      cairo_new_path (cr);
      cairo_move_to (cr, center.x, center.y);
      cairo_arc (cr, center.x, center.y, margherita.radius-(margherita.radius*0.1),
		 (count+margherita.rotation) * (M_PI/180), (count+360.0/margherita.segments + margherita.rotation) * (M_PI/180));
      if (colors[i] == 1) { cairo_set_source_rgb (cr, colors[i], 0.0, 0.0); cairo_fill(cr); }
      else
	{
	  cairo_set_source_rgb(cr,1,1,0); 
	  cairo_fill(cr);
	}
      cairo_close_path (cr);

      count+=360.0/margherita.segments;
    }

  // meat
  if (meat)
    {
      meats[0][0] = center.x + margherita.radius/2;      meats[0][1] = center.y + margherita.radius/2;
      meats[1][0] = center.x + margherita.radius/3;      meats[1][1] = center.y + margherita.radius/4;
      meats[2][0] = center.x - margherita.radius/3;      meats[2][1] = center.y - margherita.radius/4;
      meats[3][0] = center.x - margherita.radius/2;      meats[3][1] = center.y - margherita.radius/2;
      meats[4][0] = center.x + margherita.radius/2;      meats[4][1] = center.y - margherita.radius/2;
      meats[5][0] = center.x - margherita.radius/2;      meats[5][1] = center.y + margherita.radius/2;
      meats[6][0] = center.x + margherita.radius/3;      meats[6][1] = center.y - margherita.radius/4;
      meats[7][0] = center.x - margherita.radius/3;      meats[7][1] = center.y + margherita.radius/4;
      meats[7][0] = center.x - margherita.radius/2;      meats[7][1] = center.y;
      meats[8][0] = center.x;      		meats[8][1] = center.y + margherita.radius/2;
      
      meat = FALSE;
    }
  
  int mi;
  for (mi = 0; mi < 9; ++mi)
    {
      cairo_set_source_rgb(cr,1,0.2,0.2);
      cairo_arc (cr, meats[mi][0], meats[mi][1], margherita.radius*0.06, 0, 2*M_PI);
      cairo_fill(cr);
    }

  // blocky_circle
  cairo_arc (cr, center.x, center.y, margherita.exclude_radius, 0, 2*M_PI);

  // mouse pointer
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_rectangle (cr, coordinates.x-3, coordinates.y-3, 6, 6); cairo_stroke (cr);
}

static void moshi_cb (GtkEventControllerMotion *controller, double x,
		      double y, gpointer user_data)
{
  coordinates.x = x; coordinates.y = y; gtk_widget_queue_draw (user_data);
}

static void
change_slices (GtkSpinButton *spin_button, gpointer user_data)
{
  margherita.segments = gtk_spin_button_get_value_as_int (spin_button);
  gtk_widget_queue_draw(user_data);
}

static void
change_radius (GtkSpinButton *spin_button, gpointer user_data)
{
  margherita.radius = gtk_spin_button_get_value_as_int (spin_button);
  meat = TRUE;
  gtk_widget_queue_draw(user_data);
}

static void
change_rotation (GtkSpinButton *spin_button, gpointer user_data)
{
  margherita.rotation = gtk_spin_button_get_value_as_int (spin_button);
  gtk_widget_queue_draw(user_data);
}

static void
change_blocky (GtkSpinButton *spin_button, gpointer user_data)
{
  margherita.exclude_radius = gtk_spin_button_get_value_as_int (spin_button);
  gtk_widget_queue_draw(user_data);
}


static void clicky_cb (GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data)
{
  center.x = coordinates.x;
  center.y = coordinates.y;
  gtk_widget_queue_draw(user_data);
  meat = TRUE;
  printf("changed center point\n");
}

void activate (GtkApplication *application, gpointer user_data)
{
  GtkWidget *win = gtk_application_window_new (application);
  gtk_window_set_title (GTK_WINDOW(win), "Pizza Detection Algorithm Demo");
  gtk_window_set_default_size (GTK_WINDOW(win), 300, 500);

  GtkWidget *da = gtk_drawing_area_new ();
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (da), draw_func, NULL, NULL);

  gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (da), 100*4);
  gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (da), 100*4);

  GtkEventControllerMotion *moshi = GTK_EVENT_CONTROLLER_MOTION (gtk_event_controller_motion_new ());
  gtk_widget_add_controller (da, GTK_EVENT_CONTROLLER (moshi));
  g_signal_connect (moshi, "motion", G_CALLBACK (moshi_cb), da);

  GtkGesture *clicky = gtk_gesture_click_new ();
  gtk_widget_add_controller (da, GTK_EVENT_CONTROLLER(clicky));
  g_signal_connect (clicky, "pressed", G_CALLBACK (clicky_cb), da);


  GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_append (GTK_BOX(vbox), da);
  gtk_widget_set_vexpand (da, TRUE);

  GtkWidget *la = gtk_label_new("slices:");
  gtk_box_append (GTK_BOX(vbox), la);
  GtkWidget *spbut = gtk_spin_button_new_with_range (0, 360, 1);
  g_signal_connect (spbut, "value-changed", G_CALLBACK (change_slices), da);
  gtk_box_append (GTK_BOX(vbox), spbut);

  la = gtk_label_new("radius:");
  gtk_box_append (GTK_BOX(vbox), la);
  spbut = gtk_spin_button_new_with_range (5, 1000, 1);
  g_signal_connect (spbut, "value-changed", G_CALLBACK (change_radius), da);
  gtk_box_append (GTK_BOX(vbox), spbut);

  la = gtk_label_new("rotation:");
  gtk_box_append (GTK_BOX(vbox), la);
  spbut = gtk_spin_button_new_with_range (0, 360, 1);
  g_signal_connect (spbut, "value-changed", G_CALLBACK (change_rotation), da);
  gtk_box_append (GTK_BOX(vbox), spbut);

  la = gtk_label_new("excluding circle radius:");
  gtk_box_append (GTK_BOX(vbox), la);
  spbut = gtk_spin_button_new_with_range (0, 360, 1);
  g_signal_connect (spbut, "value-changed", G_CALLBACK (change_blocky), da);
  gtk_box_append (GTK_BOX(vbox), spbut);
  
  gtk_window_set_child (GTK_WINDOW (win), vbox);
  gtk_widget_show (win);
}
