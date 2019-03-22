/*
 * display.h
 *
 *      Created class March 21, 2019
 *      Author: anupm
 *
 *      Display class for Raspberry Pi that displays content into a Gtk window
 *      using Cairo graphics
 * 
 *      derived from Elma Process class
 *
 */

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



//! A Display class to display flight simulator output to a Window
//! Derived from the Process class and gets scheduled by the Manager class
//! Listens to the Attitude channel to gather data display a flight simulator
//! visual to the screen. Uses Gtk 3.0 and Cairo graphics to display graphical content
//! to the screen in a window

class Display : public Process {
    public:
        //! Constructor for UI
        Display();

        //! Constructor for UI (takes a process name)
        //! \param name Name of the process
        Display(string name);

        //! Initialization method.
        //! It will usually be called once, after all processes and
        //! communication objects have been added to the manager, but before
        //! the Manager starts running.
        void init();

        //! Start method. Set up the UI for the app
        //! Will set up a separate thread to 
        //! update the Gtk windowed UI using a window procedure (wndproc)
        //! Called just before the manager starts running.
        //! It may be called multiple times, if the manager is started and stopped.
        //! Session type initialization should happen here.
        void start();

        //! Update method. Will update the App UI at a set frequency
        //! Target frame rate is 20fps, but can be pushed higher.
        //! Determined by the period used when the process is scheduled with the
        //! Manager (see Manager::schedule).
        void update();

        //! Stop method. Will send a quit message to the UI, which will terminate
        //! the window procedure thread. Simulator will still continue to run without
        //! UI
        //! It may be called multiple times, if the manager is started and stopped.        
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
