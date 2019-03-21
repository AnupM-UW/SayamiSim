#include "sayamisim.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

gdouble Display::_rotation = 0.0;

// cairo pieces
cairo_surface_t *_sourceImage, *_fgImage;
GtkWidget *_window = NULL,
          *_darea  = NULL;

int _width;    // window client width and height
int _height;

gint _image_w; // source image width/height
gint _image_h;

gint fg_image_w, fg_image_h;

Display::Display() : Display("unnamed Display") {}

Display::Display(string name) : Process(name), _attitudeChannel(NULL) {
    // Display::_singleton = this;
    // currentDisplay = this;
}

void Display::init() {
    _attitudeChannel = &channel("attitude");
}

void Display::start() {
    std::thread wndThread(wndproc);
    wndThread.detach();
}

void wndproc() {
    int argc = 0;
    gtk_init(&argc, NULL);

    cout<<"Inited"<<endl;
    _sourceImage = cairo_image_surface_create_from_png("test.png");
    _fgImage = cairo_image_surface_create_from_png("kisspng-A330-aircraft-cockpit2.png");
    _image_w = cairo_image_surface_get_width(_sourceImage);
    _image_h = cairo_image_surface_get_height(_sourceImage);
    fg_image_w = cairo_image_surface_get_width(_fgImage);
    fg_image_h = cairo_image_surface_get_height(_fgImage);

    _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    _darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(_window), _darea);

    g_signal_connect(G_OBJECT(_darea), "draw", G_CALLBACK(on_draw_event), NULL);
    // g_signal_connect(window, "expose-event", G_CALLBACK (on_expose_event), NULL);
    g_signal_connect(_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(_window), 800, 400);
    gtk_window_set_title(GTK_WINDOW(_window), "SayamiSim Window");
    gtk_widget_show_all(_window);

    g_timeout_add(50, (GSourceFunc)time_handler, (gpointer) _window); // 20 fps target

    gtk_main();

    cout<<"* Destroying All Cairo"<<endl;
    cairo_surface_destroy(_sourceImage);

}

void do_drawing(cairo_t *cr) {
    // draw frame
    //// cout<<"Drawing"<<endl;

    cairo_rectangle (cr, 0, 0, _width, _height/2);
    cairo_pattern_t *sky;
    sky = cairo_pattern_create_linear(20.0, 0.0, 20.0, 160.0);

    cairo_pattern_add_color_stop_rgb(sky, 0.1, 0.2, 0.2, 0.86);
    cairo_pattern_add_color_stop_rgb(sky, 0.5, 0.5, 0.5, 1);
    cairo_set_source (cr, sky);
    cairo_fill (cr);

    cairo_rectangle (cr, 0, 0, _image_w/2-200, _height);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1);
    cairo_fill (cr);
    cairo_rectangle (cr, _image_w/2 + 200, 0, 300, _height);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1);
    cairo_fill (cr);
    // cairo_scale(cr, 0.1, 0.1);
    // cairo_set_source_surface(cr, _fgImage, _width/2 - 100, _height/2 + 250);
    // cairo_paint(cr);

    cairo_save(cr); // save current transform stack

    cairo_rectangle(cr, _image_w/2-200, _image_h/2-150, 400, 150);
    cairo_clip(cr);

    //// do_perspective_scale(cr);

    cairo_translate(cr, _image_w/2, _image_h/2);
    cairo_rotate(cr, Display::rotation());  // Display::current()->rotation());  // radians
    cairo_translate(cr, -_image_w/2, -_image_h/2);
    cairo_set_source_surface(cr, _sourceImage, 0, 0);
    cairo_paint(cr);

    cairo_rectangle(cr, 0, 0, 800, 600);
    cairo_clip(cr);

    // cairo_t *fg = cairo_create(_fgImage);
    // cairo_translate(cr, -_image_w/2, -_image_h/2);
    // cairo_rotate(cr, -Display::rotation());
    // cairo_translate(cr, _image_w/2, _image_h/2);

    cairo_restore(cr); // restore saved transform stack
    cairo_scale(cr, 0.1, 0.1);
    cairo_set_source_surface(cr, _fgImage, _width/2 + 800, _height/2 + 1750);
    cairo_paint(cr);
    // cairo_destroy(fg);

    cout<<"Finished Drawing"<<endl;

}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    //// cout<<"Drawing: "<<widget<<endl;

    GtkWidget *win = gtk_widget_get_toplevel(widget);
    //// cout<<"Drawing: "<<widget<<endl;
    gtk_window_get_size(GTK_WINDOW(win), &_width, &_height);
    //// cout<<"Drawing: "<<widget<<endl;
    do_drawing(cr);
    return FALSE;
}

unsigned int* ImageExpand(unsigned int* origValues, unsigned int size,
                               double scaleFactor, unsigned int *newSz) {
    if (scaleFactor < 1) {
        throw std::range_error("scaleFactor needs to be > 1");
    }
    if (size < 1) {
        throw std::range_error("size needs to be >= 1");
    }

    unsigned int newSize = round(scaleFactor*size);
    *newSz = newSize;
    unsigned int* retval = new unsigned int[newSize];
    memset(retval, 0, newSize * sizeof(unsigned int));

    //// if (newSize > 500) {
    ////   return retval;
    //// }

    double* scaled = new double[newSize];
    double rise = (double)1 / scaleFactor;
    for (int i = 0; i < newSize; ++i) {
        scaled[i] = 0 + i*rise;
    }
    cout<<"IE1: "<<newSize<<", x: "<<retval<<endl;
    //// print_array(scaled, newSize);
    // values in the scaled array will be as follows
    // e.g. for 5 values and scale by 1.4, the values will be
    // [ 0.00000  0.714286  1.42857  2.14286  2.85714  3.57143  4.28571 ]
    // so for 1st element, we will take px[0]*(1-0.00000) + px[1]*(0.00000)
    //    for 2nd element, we will take px[0]*(1-.714286) + px[1]*(0.714286)
    //    for 3rd element, we will take px[1]*(1-.42857) + px[2]*(.42857)
    //    for 7th element, we will take px[4] and effectively disregard the *(1-.28571) (see below)
    //        note that the px[5] does not exist in the original since it is only length 5
    //    in fact, if pixel from original at the right edge is non-existent, e.g. px[5], we will always take
    //    the min(right edge, left edge + 1)
    //    so the 7th element would be, left = 4, right = min(4,5), and therefore px[4]*(1-.28571) + px[4]*(0.28571)$
    for (int i = 0; i < newSize; ++i) {
        int leftIdx = floor(scaled[i]);
        int rightIdx = min((int)size-1, leftIdx + 1);
        double mult2 = scaled[i] - leftIdx;
        double mult1 = 1 - mult2;
        if (leftIdx >= size) { cout <<"OOB: "<<leftIdx<<endl; }
        // now the real value will be origValues[leftIdx]*mult1 + origValues[ri$
        int
            r1 = (origValues[leftIdx]  & 0x00FF0000) >> 16,
            r2 = (origValues[rightIdx] & 0x00FF0000) >> 16,
            g1 = (origValues[leftIdx]  & 0x0000FF00) >>  8,
            g2 = (origValues[rightIdx] & 0x0000FF00) >>  8,
            b1 = (origValues[leftIdx]  & 0x000000FF),
            b2 = (origValues[rightIdx] & 0x000000FF);
        int
            r = min(255,(int)round(((double)r1)*mult1 + ((double)r2)*mult2)),
            g = min(255,(int)round(((double)g1)*mult1 + ((double)g2)*mult2)),
            b = min(255,(int)round(((double)b1)*mult1 + ((double)b2)*mult2));
        //// if (newSize < 500) {
        retval[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
        //// }
    }
    //// cout<<"IE2: "<<newSize<<", x: "<<retval<<endl;
    delete [] scaled;
    return retval;
}

void do_perspective_scale(cairo_t* cr) {

    // cairo_surface_t *imgCpy = cairo_surface_create_similar_image
    cairo_surface_t *doppelSurface = cairo_image_surface_create(
                       CAIRO_FORMAT_ARGB32,
                       _image_w,
                       _image_h);
    unsigned char* ptr = NULL;

    cairo_t *ic = cairo_create(doppelSurface);
    cairo_translate(ic, _image_w/2, _image_h/2);
    cairo_rotate(ic, Display::rotation()); // Display::current()->rotation()); // radians
    cairo_translate(ic, -_image_w/2, -_image_h/2);
    cairo_set_source_surface(ic, _sourceImage, 0, 0);
    cairo_paint(ic);

    int pxwidth = cairo_image_surface_get_width (doppelSurface);
    int pxheight = cairo_image_surface_get_height (doppelSurface);
    int stride = cairo_image_surface_get_stride (doppelSurface);
    // cairo_surface_flush (doppelSurface);

    ptr = (unsigned char*)malloc (stride * pxheight);
    memcpy (ptr, cairo_image_surface_get_data (doppelSurface), stride * pxheight);

    cairo_surface_t *image = cairo_image_surface_create_for_data (ptr,
                                  CAIRO_FORMAT_ARGB32, pxwidth, pxheight, stride);
    cairo_surface_flush (image);

    //// int scaleHeight = 200;
    //// double scaleAdd = 0.5, currScale = 1;
    //// double currHeight = 0;
    //// int size = 400;
    //// unsigned int newSize = 0;
    //// cout<<pxwidth<<","<<pxheight<<endl;
    //// for (int y = pxheight/2 - scaleHeight; y < pxheight/2; ++y, ++currHeight) {
    ////    currScale = 1 + currHeight/(double)(scaleHeight) * scaleAdd;
    ////    int x = pxwidth / 2 - size / 2; // midpoint in x-axis - 1/2 size, i.e. the middle$
    ////    unsigned int *cur = (unsigned int*) &ptr[y * stride + x*4];
    ////    cout<<"Scale: "<<currScale<<", x: "<<x<<endl;
    ////    unsigned int *newVals = ImageExpand(cur, size, currScale, &newSize);
    ////    cout<<"1- "<<currScale<<", x: "<<x<<", y: "<<y<<endl;
    ////    // set it in the middle
    ////    int newX = pxwidth / 2 - newSize / 2;
    ////    if (y < 200) {
    ////        unsigned int* newCur = (unsigned int*) &ptr[y * stride + newX * 4];
    ////        memcpy(newCur, newVals, newSize * 4);
    ////        // memset(cur, 0 , newSize * 4);
    ////    }
    ////    cout<<"- "<<currScale<<", x: "<<x<<", y: "<<y<<", newX: "<< newX<<endl;
    ////    delete [] newVals;
    //// }

    for (int y = 0; y < pxheight; y++) {
         for (int x = 0; x < pxwidth; x++) {
            if (y < 10) { // watch for endianness
                ptr[y*stride + x*4] = 0x00;
                ptr[y*stride + x*4 + 1] = 0x00;
                ptr[y*stride + x*4 + 2] = 0xff;
                ptr[y*stride + x*4 + 3] = 0xff;
            }
            // unsigned int z = *((unsigned int*)&ptr [y * stride + x * 4]);
            // if ((z & 0xffffff) == 0xffffff) {
            //     z = (z & ~0xff000000) | (alpha & 0xff000000);
            //     *((unsigned int*) &ptr [y * stride + x * 4]) = z;
            //        }
        }
    }

    // doppelSurface contains the rotated image now

    // now get pixels of

    cairo_set_source_surface(cr, image, 0, 0);
    //// cairo_set_source_surface(cr, doppelSurface, 0, 0);
    cairo_paint(cr);
    cairo_surface_destroy(doppelSurface);
    cairo_surface_destroy(image);
    cairo_destroy(ic);
}

gboolean time_handler(GtkWidget *widget) {
  gtk_widget_queue_draw(widget);
  return TRUE;
}

void Display::update() {
    // there is an attitude update
    if (_attitudeChannel->nonempty()) {
        json latest = _attitudeChannel->latest();
        do_display(latest);
        double hdg = latest["hdg"];
        // heading is in degrees, convert it to radians
        _rotation = hdg * PIOVER180;
    }
}

void Display::do_display(const json& att) {
    double lat = att["lat"];
    double lng = att["long"];
    double alt = att["alt"];
    double hdg = att["hdg"];
    double aoa = att["aoa"];
    cout<<"Display update .. "
        <<"Latitude: "  <<lat<< " "
        <<"Longitude: " <<lng<< " "
        <<"Altitude: "  <<alt<< " "
        <<"Heading: "   <<hdg<< " "
        <<"AoA: "       <<aoa<< endl;
}

void Display::stop() {
    gtk_main_quit();
}

double Display::rotation() {
    return (double) _rotation;
}

//Display* Display::current() {
//    // return Display::_singleton;
//    return currentDisplay;
//}
