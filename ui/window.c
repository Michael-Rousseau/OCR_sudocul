#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


GtkWidget *image;
int is_loaded = 0;
char *file_path;

const char *image_paths[] = {
	"./data/images/grayscale.png",
	"./data/images/inverse.png",
	"./data/images/contrast.png",
	"./data/images/reducenoise.png",
	"./data/images/erosion.png",
	"./data/images/dilation.png",
	"./data/images/canny.png"
};


int start_matrix[9][9] = {{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9},
	{1, 2, 3, 4, 5, 6, 7, 8, 9}};


int end_matrix[9][9] = {{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1},
	{9, 8, 7, 6, 5, 4, 3, 2, 1}};


typedef struct {
	GtkWidget *mainGrid;
	GtkWidget *sudokuGrid;
} GridData;



void update_matrices_from_grid(const char *grid_string, int var) {
	int row = 0, col = 0;
	while (*grid_string) {
		if (*grid_string >= '1' && *grid_string <= '9') {
			if (var == 0){
				start_matrix[row][col] = *grid_string - '0';
			}
			else {
				end_matrix[row][col] = *grid_string - '0';
			}
		} else if (*grid_string == '.') {
			start_matrix[row][col] = 0;
		}

		// Move to next cell
		col++;
		if (col == 9) {
			col = 0;
			row++;
		}

		// Skip to next character
		grid_string++;

		// Stop if we reach the end of the grid
		if (row == 9) {
			break;
		}
	}
}


static gboolean display_next_image(gpointer user_data) {
	static int current_image = 0;
	const int total_images = sizeof(image_paths) / sizeof(image_paths[0]);

	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(image_paths[current_image], 600, 600, TRUE, NULL);
	if (pixbuf) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
		g_object_unref(pixbuf);
	}

	current_image = (current_image + 1) % total_images;
	return (current_image != 0);
}


static void on_epic_button_clicked(GtkWidget *widget, gpointer user_data) {
	if (!is_loaded){
		g_print("Image has not been loaded\n");
		return;
	}
	g_timeout_add_seconds(1, display_next_image, NULL);
}


static void get_the_grid(GtkWidget *widget, gpointer user_data) {
	if (!is_loaded) {
		g_print("Image has not been loaded\n");
		return;
	}

	// Open the file containing the grid
	FILE *file = fopen("./data/grids/grid_start", "r");
	if (file == NULL) {
		g_print("Failed to open grid file\n");
		return;
	}

	char grid_string[82]; // 81 characters for the grid + 1 for null terminator
	char line[20];        // Temporary buffer for each line
	int index = 0;

	// Read the file line by line
	while (fgets(line, sizeof(line), file)) {
		// Process each character in the line
		for (int i = 0; line[i] != '\0' && index < 81; i++) {
			if ((line[i] >= '0' && line[i] <= '9') || line[i] == '.') {
				grid_string[index++] = line[i];
			}
		}
	}
	grid_string[index] = '\0'; // Null-terminate the string
	fclose(file);

	// Call update_matrices_from_grid function
	update_matrices_from_grid(grid_string, 0);

	if (GTK_IS_IMAGE(image)) {
		GridData *data = (GridData *)user_data;
		g_print("The image is a GTK_image\n");
		gtk_image_clear(image);
		is_loaded = 0;

		for (int row = 0; row < 9; row++) {
			for (int col = 0; col < 9; col++) {
				char buffer[3];
				snprintf(buffer, sizeof(buffer), "%d", start_matrix[row][col]);

				GtkWidget *number_label = gtk_button_new_with_label(buffer);
				gtk_grid_attach(GTK_GRID(data->sudokuGrid), number_label, col, row, 1, 1);
			}
		}

		gtk_grid_attach(GTK_GRID(data->mainGrid), data->sudokuGrid, 0, 2, 1, 1);
		gtk_widget_show_all(data->mainGrid);
	} else {
		g_print("Not a gtk image\n");
	}
}



static void solve_image(GtkWidget *widget, gpointer user_data) {
	GridData *data = (GridData *)user_data;

	if(!gtk_grid_get_child_at(GTK_GRID(data->sudokuGrid), 0, 0)){
		g_print("Grid has not been loaded !\n");
	}

	// Open the file containing the grid
	FILE *file = fopen("./data/grids/grid_end", "r");
	if (file == NULL) {
		g_print("Failed to open grid file\n");
		return;
	}

	char grid_string[82]; // 81 characters for the grid + 1 for null terminator
	char line[20];        // Temporary buffer for each line
	int index = 0;

	// Read the file line by line
	while (fgets(line, sizeof(line), file)) {
		// Process each character in the line
		for (int i = 0; line[i] != '\0' && index < 81; i++) {
			if ((line[i] >= '0' && line[i] <= '9') || line[i] == '.') {
				grid_string[index++] = line[i];
			}
		}
	}
	grid_string[index] = '\0'; // Null-terminate the string
	fclose(file);

	// Call update_matrices_from_grid function
	update_matrices_from_grid(grid_string, 1);

	/*
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(data->sudokuGrid), col, row);

			if (GTK_IS_BUTTON(child)) {
				char buffer[3];
				snprintf(buffer, sizeof(buffer), "%d", end_matrix[row][col]);
				gtk_button_set_label(GTK_BUTTON(child), buffer);
			}
		}
	}*/


	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(data->sudokuGrid), col, row);

			if (GTK_IS_BUTTON(child)) {
				char buffer[3];
				snprintf(buffer, sizeof(buffer), "%d", end_matrix[row][col]);
				gtk_button_set_label(GTK_BUTTON(child), buffer);

				// Check if the number has changed and change color if it has
				if (start_matrix[row][col] == 0 && end_matrix[row][col] != 0) {
					// Change button color to indicate it's a new number
					GtkStyleContext *context = gtk_widget_get_style_context(child);
					gtk_style_context_add_class(context, "changed-number");
				}
			}
		}
	}




	gtk_widget_show_all(data->mainGrid);
}



static void clicked_image(GtkWidget *widget, gpointer data) {
	if (!is_loaded || file_path == NULL) {
		g_print("Image has not been loaded or file path is NULL\n");
		return;
	}

	char *base_path = "./data/images";
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

	GtkWidget *epic_button;


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
	gtk_grid_attach(GTK_GRID(button_grid), grayscale, 0, 0, 1, 1);


	threshold = gtk_button_new_with_label("THRESHOLD");
	g_signal_connect(threshold, "clicked", G_CALLBACK(clicked_image), "inverse.png");
	gtk_grid_attach(GTK_GRID(button_grid), threshold, 0, 1, 1, 1);


	contrast = gtk_button_new_with_label("CONTRAT");
	g_signal_connect(contrast, "clicked", G_CALLBACK(clicked_image), "contrast.png");
	gtk_grid_attach(GTK_GRID(button_grid), contrast, 0, 2, 1, 1);


	reducenoise = gtk_button_new_with_label("REDUCENOISE");
	g_signal_connect(reducenoise, "clicked", G_CALLBACK(clicked_image), "reducenoise.png");
	gtk_grid_attach(GTK_GRID(button_grid), reducenoise, 0, 3, 1, 1);


	erosion  = gtk_button_new_with_label("EROSION");
	g_signal_connect(erosion, "clicked", G_CALLBACK(clicked_image), "erosion.png");
	gtk_grid_attach(GTK_GRID(button_grid), erosion, 0, 4, 1, 1);


	dilation = gtk_button_new_with_label("DILATION ");
	g_signal_connect(dilation, "clicked", G_CALLBACK(clicked_image), "dilation.png");
	gtk_grid_attach(GTK_GRID(button_grid), dilation, 0, 5, 1, 1);

	canny = gtk_button_new_with_label("CANNY");
	g_signal_connect(canny, "clicked", G_CALLBACK(clicked_image), "canny.png");
	gtk_grid_attach(GTK_GRID(button_grid), canny, 0, 6, 1, 1);

	epic_button = gtk_button_new_with_label("EPIC BUTTON");
	g_signal_connect(epic_button, "clicked", G_CALLBACK(on_epic_button_clicked), NULL);
	gtk_grid_attach(GTK_GRID(button_grid), epic_button, 0, 9, 1, 1);

	GtkWidget *solve, *get_grid;

	GridData *data = malloc(sizeof(GridData));
	data->mainGrid = grid;
	data->sudokuGrid = gtk_grid_new();


	get_grid = gtk_button_new_with_label("GET GRID");
	g_signal_connect(get_grid, "clicked", G_CALLBACK(get_the_grid), data);
	gtk_grid_attach(GTK_GRID(button_grid), get_grid, 0, 7, 1, 1);


	solve = gtk_button_new_with_label("SOLVE");
	g_signal_connect(solve, "clicked", G_CALLBACK(solve_image), data);
	gtk_grid_attach(GTK_GRID(button_grid), solve, 0, 8, 1, 1);




	// Create a new file chooser button
	file_chooser = gtk_file_chooser_button_new("Select a PNG File", GTK_FILE_CHOOSER_ACTION_OPEN);

	// Set up a filter for PNG files
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_mime_type(filter, "image/png");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file_chooser), filter);

	// Create a new image widget
	image = gtk_image_new();
	gtk_grid_attach(GTK_GRID(grid), image, 0, 2, 1, 1);

	// Connect the "file-set" signal of the file chooser to the on_file_chosen function
	g_signal_connect(file_chooser, "file-set", G_CALLBACK(on_file_chosen), image);

	// Add the file chooser to the grid
	gtk_grid_attach(GTK_GRID(grid), file_chooser, 0, 1, 1, 1);

	// Show all widgets in the window
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
