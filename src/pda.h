#ifndef PDA_H
#define PDA_H

#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#define XFAR_CENTER (coordinates.x-center.x)
#define YFAR_CENTER (coordinates.y-center.y)

void activate (GtkApplication *application, gpointer user_data);

#endif /* PDA_H */
