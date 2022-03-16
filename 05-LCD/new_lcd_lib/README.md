<br>

# LCD Example


This project shows an alternate LCD driver which for example implements a printf equivalent. Documentation in this page is incomplete, but check the app.c file for some functions, and the lio_lcd.h file in /lib/lcd/include/ for function documentation.

### Graph Objects

Since it can often be useful to graph sensor inputs there's also a utility for graphing in conjunction with the LCD library.
```c
typedef struct{
    uint16_t color;
    uint16_t background;
    uint32_t x_origin, y_origin;
    scale_state_t scale_setting;
    uint32_t scaling;
    int32_t lower_bound;
    
    /* Please don't mess with */
    graph_internals_t *internals;
}graph_config_t;
```
This struct is used as the handle for every graph function. It sets configurations for the graph such as color and scaling behaviour. It also keeps track och allocated memory associated with the object.

- uint16_t **color**
    - Color setting for the graph
- uint16_t **background**
    - Background color setting for the graph
- uint32_t **x_origin, y_origin**
    - Coordinates for the top left corner of the graph
- scale_state_t **scale_setting**
    - Sets the scaling behaviour of the graph
        - SCALE_TO_FIT - Scales values so that all values fits inside the window
        - USE_CUSTOM - Uses "scaling" variable as the divisor
        - NO_SCALING - Maps values 1:1 on the pixels in the graph
- int32_t **scaling**
    - Current divisor for the graph. USE_CUSTOM setting uses this value for scaling, while SCALE_TO_FIT updates this value to indicate current scaling.
- int32_t **lower_bound** 
    - This value sets the value that the bottom pixel corresponds to when graphing. To set an arbitrary value in the middle of the graph use: <br>lower_bound = -(graph_height * scaling)/2 + (value in middle of graph)
<br>
```c
int gui_init_graph(graph_config_t* graph, uint32_t width, uint32_t height)
```
Graph initialization, height and width in pixels. This function handles allocation of memory, so do not call repeatedly without using **gui_remove_graph()**

```c
int gui_remove_graph(graph_config_t* graph)
```
Frees allocated memory associated with the graph.
```c
int gui_append_graph(graph_config_t* graph, int32_t* values, size_t append_n)
```
Adds n values to the graph. One value is one pixel in the graph. The graph takes values as a ring buffer. So for each value added in front one is discarded in the back.

```c
int gui_render_graph(graph_config_t* graph)
```
Renders the graph to the screen.

```c
int gui_erase_graph(graph_config_t* graph)
```
Erases the last drawn graph. It remembers the last render and erases correctly even if values have been appended between render and erase.