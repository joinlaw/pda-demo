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

int main(int argc, char **argv)
{
  GtkApplication *app = gtk_application_new ("org.test.first", G_APPLICATION_FLAGS_NONE);
  
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  
  int status = g_application_run (G_APPLICATION(app), argc, argv);
  
  g_object_unref (app);
  return status;
}
