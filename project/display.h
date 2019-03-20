#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <iostream>
#include <string>
#include <cairo/cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>
#include <algorithm>
#include <thread>

#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

const double PI = 3.14159;
const double PIOVER180 = 0.01745329;

void wndproc();

void do_drawing(cairo_t *);

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);

unsigned int* ImageExpand(unsigned int* origValues, unsigned int size,
                         double scaleFactor, unsigned int *newSz);

void do_perspective_scale(cairo_t* cr);

gboolean time_handler(GtkWidget *widget);

// Display* currentDisplay = NULL;

class Display : public Process {
    public:
        Display();

        Display(string name);

        void init();

        void start();

        void update();

        void stop();

	static double rotation();

        // static Display* current();

    private:
        Channel* _attitudeChannel;

        //void wndproc();

        void do_display(const json& att);

        // static void do_drawing(cairo_t *);

        // static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);

        // unsigned int* ImageExpand(unsigned int* origValues, unsigned int size,
        //                       double scaleFactor, unsigned int *newSz);

        // static void do_perspective_scale(cairo_t* cr);

        // static gboolean time_handler(GtkWidget *widget);


        // cairo pieces
        //static cairo_surface_t *_sourceImage;
        //GtkWidget
        //          *_window,
        //          *_darea;

        //static int _width;    // window client width and height
        //static int _height;

        //static gint _image_w; // source image width/height
        //static gint _image_h;

        static gdouble _rotation;

        // static Display * _singleton;

};

#endif
