#include <cairo/cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>

using namespace std;

cairo_surface_t *sourceImage;
GtkWidget *window, *darea;
// cairo_t *cr;
gdouble rotation = 0;
int width, height;
gint image_w, image_h;

const double PI = 3.14150;
const double PIOVER180 = 0.01745329;
double rot = 0.5;

static void do_drawing(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  // printf("Draw");
  GtkWidget *win = gtk_widget_get_toplevel(widget);
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);
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
    if (newSize > 500) {
       return retval;
    }
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
        if (newSize < 500) {        retval[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
        }
    }
    cout<<"IE2: "<<newSize<<", x: "<<retval<<endl;
    delete [] scaled;
    return retval;
}

static void do_perspective_scale(cairo_t* cr) {
    // cairo_surface_t *imgCpy = cairo_surface_create_similar_image
    cairo_surface_t *doppelSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, image_w, image_h);
    unsigned char* ptr = NULL;

    cairo_t *ic = cairo_create(doppelSurface);
    cairo_translate(ic, image_w/2, image_h/2);
    cairo_rotate(ic, rot); // radians
    cairo_translate(ic, -image_w/2, -image_h/2);
    cairo_set_source_surface(ic, sourceImage, 0, 0);
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
    ////    int x = pxwidth / 2 - size / 2; // midpoint in x-axis - 1/2 size, i.e. the middle 'size' pixels
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

    rot += 0.1;
    // doppelSurface contains the rotated image now

    // now get pixels of

    cairo_set_source_surface(cr, image, 0, 0);
    //// cairo_set_source_surface(cr, doppelSurface, 0, 0);
    cairo_paint(cr);
    cairo_surface_destroy(doppelSurface);
    cairo_surface_destroy(image);
    cairo_destroy(ic);
}

static void do_drawing(cairo_t *cr) {
    // draw frame
    cairo_rectangle (cr, 0, 0, width, height/2);
    cairo_set_source_rgba (cr, 0.1, 0.1, 0.76, 0.80);
    cairo_fill (cr);
    cairo_rectangle (cr, 0, 0, image_w/2-200, height);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1);
    cairo_fill (cr);
    cairo_rectangle (cr, image_w/2 + 200, 0, 300, height);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1);
    cairo_fill (cr);

    cairo_rectangle(cr, image_w/2-200, image_h/2-200, 400, 200);
    cairo_clip(cr);

    //// do_perspective_scale(cr);

    cairo_translate(cr, image_w/2, image_h/2);
    cairo_rotate(cr, rot); // radians
    cairo_translate(cr, -image_w/2, -image_h/2);
    cairo_set_source_surface(cr, sourceImage, 0, 0);
    cairo_paint(cr);

    rot += 0.1;


}


gboolean rotate_image_by(double degrees) {
    // Any rotation applied to cr here will be lost, as we create
    // a new cairo context on every expose event

    //cairo_rotate (cr, 4);
    rotation += degrees * PIOVER180; // in radians

    //cairo_paint(cr);
    //      printf("rotating\n");

    // force repaint
    gtk_widget_queue_draw(window);
    return TRUE;
}

gboolean rotate_clip() {
    return TRUE;
}

void resizeRow(double factor, int* orig, int sizeOrig) {
    int newSize = floor(factor * sizeOrig);
    int* newSizeArr = (int*) calloc(newSize, sizeof(int));
    
}

// static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event,gpointer data)
// {
//     // from surfaceImage, do a translate/rotation on it from the middle of the image. Then translate the center of the image 
//     // to middle, bottom
//     // then perspective transform (fake it)
//     // then clip to window bounds.
//     // finally create a new cairo context and draw the surface on to it

//     // cairo_translate(cr, width / 2.0, height / 2.0);
//     // cairo_rotate(cr, rotation_amt);
//     // cairo_translate(cr, - image_w / 2.0, - image_h / 2.0);
//     // cairo_set_source_surface(cr, image, 0, 0);
//     // cairo_paint(cr);



//     // Make sure our window wasn't destroyed yet
//     // (to silence a warning)
//     g_return_if_fail(GTK_IS_WIDGET(widget));

//     cr = gdk_cairo_create (widget->window);
//     // We need to apply transformation before setting the source surface
//     // We translate (0, 0) to the center of the screen,
//     // so we can rotate the image around its center point,
//     // not its upper left corner
//     cairo_translate(cr, image_w/2, image_h/2);
//     cairo_rotate(cr, rotation);
//     cairo_set_source_surface(cr, image, -image_w/2, -image_h/2);
    
//     // apply clip
//     cairo_rectangle(cr, -image_w/2, -image_h/2, image_w, image_h);
//     cairo_clip(cr);

//     cairo_paint(cr);
//     //printf("Paint\n");
//     cairo_destroy(cr);
//     return FALSE;
// }

static gboolean time_handler(GtkWidget *widget)
{
  gtk_widget_queue_draw(widget);  
  return TRUE;
}

int main(int argc, char *argv[])
{
    // double degrees = 0;
    // int speed = 125;
    sourceImage = cairo_image_surface_create_from_png("test.png");
    image_w = cairo_image_surface_get_width(sourceImage);
    image_h = cairo_image_surface_get_height(sourceImage);

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL); 
    // g_signal_connect(window, "expose-event", G_CALLBACK (on_expose_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_title(GTK_WINDOW(window), "SayamiSim Window");
    gtk_widget_show_all(window);

    // gtk_widget_set_app_paintable(window, TRUE);

    g_timeout_add(1500, (GSourceFunc) time_handler, (gpointer) window);

    // g_timeout_add(1500, (GSourceFunc) rotate_cb, NULL);

    // g_timeout_add(speed, (GSourceFunc) rotate_cb, degrees);

    gtk_main();
    cairo_surface_destroy(sourceImage);

    return 0;
}
