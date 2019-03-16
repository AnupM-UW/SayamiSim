#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h> 

cairo_surface_t *sourceImage;
GtkWidget *window, *darea;
// cairo_t *cr;
gdouble rotation = 0;
int width, height;

const double PI = 3.14150;
const double PIOVER180 = 0.01745329;

static void do_drawing(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{ 
  GtkWidget *win = gtk_widget_get_toplevel(widget);
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);
  do_drawing(cr);
  return FALSE;
}

static void do_drawing(cairo_t *cr)
{
    cairo_set_source_surface(cr, sourceImage, 0, 0);
    cairo_paint(cr);
}

gint image_w, image_h;

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
    int* newSizeArr = calloc(newSize, sizeof(int));
    
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
    image_w = cairo_image_surface_get_width(image);
    image_h = cairo_image_surface_get_height(image);

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_WINDOW(window), darea);

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