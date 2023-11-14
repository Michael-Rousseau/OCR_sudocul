#include <gtk/gtk.h>
#include <string.h>

GtkWidget *image;
int is_loaded = 0;
char *file_path;


static void clicked_image(GtkWidget *widget, gpointer data) {
    if (!is_loaded || file_path == NULL) {
        g_print("Image has not been loaded or file path is NULL\n");
        return;
    }


    char *base_path = "/mnt/c/Users/roman/OneDrive/Desktop/EPITA/SPE/SUDOCUL/OCR_sudocul/ui/data";
    int length = strlen(base_path) + strlen(data) + 2;  // +2 for the null terminator and a slash

    // Allocate memory
    char *full_path = malloc(length);

    // Check if memory allocation was successful
    if (full_path != NULL) {
        // Construct the full path
        strcpy(full_path, base_path);
        strcat(full_path, "/");
        strcat(full_path, data);

        // Free the old file_path and update it to full_path
        if (file_path != NULL) {
            free(file_path);
        }
        file_path = full_path;

        // Note: Do not free full_path here, as file_path is now using it
    }
    else {
        g_print("Memory allocation failure\n");
        return;
    }

    if (GTK_IS_IMAGE(image)) {
        g_print("image is a GtkImage\n");
    } else {
        g_print("image is not a GtkImage\n");
    }

    g_print("Attempting to load image from path: %s\n", file_path);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(file_path, 600, 600, TRUE, NULL);
    if (pixbuf) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
        g_object_unref(pixbuf);
        g_print("Image has been successfully loaded from: %s\n", file_path);
    } else {
        g_print("Failed to load image from: %s\n", file_path);
    }
}


static void on_file_chosen(GtkFileChooserButton *button, gpointer user_data) {

    gchar *uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(button));


    if (uri) {

        file_path = g_filename_from_uri(uri, NULL, NULL);
        g_free(uri);

        // Load the image using GdkPixbuf
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(file_path, 600, 600, TRUE, NULL);
        if (pixbuf) {
            // Update the GtkImage with the new Pixbuf
            gtk_image_set_from_pixbuf(GTK_IMAGE(user_data), pixbuf);

            // Decrease reference count to avoid memory leak
            g_object_unref(pixbuf);
            is_loaded = 1;
        }

        // Free the file_path when done
        //g_free(file_path);
    }
}




static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grayscale, *threshold, *contrast, *reducenoise, *erosion, *dilation, *canny;
    GtkWidget *file_chooser;
    GtkWidget *grid;
    GtkWidget *button_grid;



    grid = gtk_grid_new();
    button_grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), button_grid, 1, 2, 1, 1);


    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);


    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "SUDOCUL");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    gtk_container_add(GTK_CONTAINER(window), grid);


    grayscale = gtk_button_new_with_label("GRAYSCALE");
    g_signal_connect(grayscale, "clicked", G_CALLBACK(clicked_image), "grayscale.png");
    gtk_container_add(GTK_CONTAINER(grayscale), grid);
    gtk_grid_attach(GTK_GRID(button_grid), grayscale, 0, 0, 1, 1);


    threshold = gtk_button_new_with_label("THRESHOLD");
    g_signal_connect(threshold, "clicked", G_CALLBACK(clicked_image), "canny.png");
    gtk_container_add(GTK_CONTAINER(threshold), grid);
    gtk_grid_attach(GTK_GRID(button_grid), threshold, 0, 1, 1, 1);


    contrast = gtk_button_new_with_label("CONTRAT");
    g_signal_connect(contrast, "clicked", G_CALLBACK(clicked_image), "contrast.png");
    gtk_container_add(GTK_CONTAINER(contrast), grid);
    gtk_grid_attach(GTK_GRID(button_grid), contrast, 0, 2, 1, 1);


    reducenoise = gtk_button_new_with_label("REDUCENOISE");
    g_signal_connect(reducenoise, "clicked", G_CALLBACK(clicked_image), "reducenoise.png");
    gtk_container_add(GTK_CONTAINER(reducenoise), grid);
    gtk_grid_attach(GTK_GRID(button_grid), reducenoise, 0, 3, 1, 1);


    erosion  = gtk_button_new_with_label("EROSION");
    g_signal_connect(erosion, "clicked", G_CALLBACK(clicked_image), "erosion.png");
    gtk_container_add(GTK_CONTAINER(erosion), grid);
    gtk_grid_attach(GTK_GRID(button_grid), erosion, 0, 4, 1, 1);


    dilation = gtk_button_new_with_label("DILATION ");
    g_signal_connect(dilation, "clicked", G_CALLBACK(clicked_image), "dilation.png");
    gtk_container_add(GTK_CONTAINER(dilation), grid);
    gtk_grid_attach(GTK_GRID(button_grid), dilation, 0, 5, 1, 1);

    canny = gtk_button_new_with_label("CANNY");
    g_signal_connect(canny, "clicked", G_CALLBACK(clicked_image), "canny.png");
    gtk_container_add(GTK_CONTAINER(canny), grid);
    gtk_grid_attach(GTK_GRID(button_grid), canny, 0, 6, 1, 1);



    GtkWidget *solve;

    solve = gtk_button_new_with_label("SOLVE");
    g_signal_connect(solve, "clicked", G_CALLBACK(clicked_image), image);
    gtk_container_add(GTK_CONTAINER(solve), grid);
    gtk_grid_attach(GTK_GRID(button_grid), solve, 0, 7, 1, 1);






    file_chooser = gtk_file_chooser_button_new("Select a PNG File", GTK_FILE_CHOOSER_ACTION_OPEN);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file_chooser), filter);

    image = gtk_image_new();
    gtk_grid_attach(GTK_GRID(grid), image, 0, 2, 1, 1);

    g_signal_connect(file_chooser, "file-set", G_CALLBACK(on_file_chosen), image);
    gtk_grid_attach(GTK_GRID(grid), file_chooser, 0, 1, 1, 1);



    gtk_widget_show_all(window);
}




int main(int argc, char **argv) {
    GtkApplication *app;
    int status;



    app = gtk_application_new("org.gtk.sudocul", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

