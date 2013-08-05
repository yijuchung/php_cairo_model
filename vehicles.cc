#include "php_vehicles.h"
#include "/usr/include/cairo/cairo.h"
#include "car.h"
#define CAIRO_HAS_FT_FONT 1
//#define CAIRO_HAS_FC_FONT 1

zend_object_handlers car_object_handlers;

struct car_object {
    zend_object std;
    Car *car;
};

zend_class_entry *car_ce;

void car_free_storage(void *object TSRMLS_DC)
{
    car_object *obj = (car_object *)object;
    delete obj->car; 

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value car_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    car_object *obj = (car_object *)emalloc(sizeof(car_object));
    memset(obj, 0, sizeof(car_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->default_properties,
        (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

    retval.handle = zend_objects_store_put(obj, NULL,
        car_free_storage, NULL TSRMLS_CC);
    retval.handlers = &car_object_handlers;

    return retval;
}
PHP_METHOD(Car, __construct)
{
    Car *car = NULL;
    zval *object = getThis();
    car = new Car();
    car_object *obj = (car_object *)zend_object_store_get_object(object TSRMLS_CC);
    obj->car = car;

}
PHP_METHOD(Car, font)
{
    char *font;
    int font_len;
    char *text;
    int text_len;
    bool bold,slant;
    double size;
    bool matrix;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssbbdb",&font,&font_len,&text,&text_len,&bold,&slant,&size,&matrix ) == FAILURE) {
        RETURN_NULL();
    }


    cairo_surface_t *surface =  cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1, 1);
    cairo_t *cr =  cairo_create (surface);


	if( slant == 1 && bold == 1 )
	{
        	cairo_select_font_face (cr, font, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
	}	
	else if( bold == 1 )
	{
        	cairo_select_font_face (cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	}
	else if( slant == 1 )
	{
        	cairo_select_font_face (cr, font, CAIRO_FONT_SLANT_OBLIQUE, CAIRO_FONT_WEIGHT_NORMAL);
	}
	else
	{	
        	cairo_select_font_face (cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	}

	//cairo_set_font_size (cr, size);

        cairo_font_extents_t et;
        cairo_font_extents(cr ,&et);

	cairo_matrix_t ctm;

        if(text == NULL)
             return;
	
	if( matrix == 1 )
	{
		cairo_matrix_init(&ctm, 1,0,-0.2,1,0,0);
		cairo_matrix_scale(&ctm, size,size);
		cairo_set_font_matrix(cr,&ctm);
		
	}else
	{
		cairo_set_font_size(cr,size);
		//cairo_move_to(car, 0.0, et.ascent);
	}
	

	cairo_scaled_font_t *csf;

	csf = cairo_get_scaled_font(cr);
	cairo_scaled_font_extents(csf,&et);
	
	cairo_text_extents_t ce;
	cairo_scaled_font_text_extents(csf,text,&ce);

	//cairo_set_font_matrix(cr,&mt2);
	//	cairo_translate(cr,2.0,1);

	Car *car;
	car_object *obj = (car_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	car = obj->car;

	//zend_printf("text %f %f %f %f %f %f",ce.x_bearing,ce.y_bearing,ce.width,ce.height,ce.x_advance,ce.y_advance);
	//zend_printf("font %f %f %f %f %f", et.ascent,et.descent, et.height, et.max_x_advance,et.max_y_advance);
	cairo_move_to(cr, car->startx , et.descent);

	cairo_text_path (cr, text);

        int i;


        cairo_path_t *path;
        cairo_path_data_t *data;

        //FILE *fp;
        //fp = fopen("1.txt","w");

        path = cairo_copy_path (cr);

	for (i=0; i < path->num_data; i += path->data[i].header.length) {
            data = &path->data[i];
                switch (data->header.type) {
                case CAIRO_PATH_MOVE_TO:
                        //do_move_to_things (data[1].point.x, data[1].point.y);
                                //cairo_move_to(data[1].point.x, data[1].point.y);
                                //fprintf(fp,"M%f,%f ",data[1].point.x, data[1].point.y);
                                //fprintf(fp,"M%f,%f ",data[1].point.x, data[1].point.y);
			zend_printf("M%f,%f ",data[1].point.x, data[1].point.y);
					if(car->tempmaxy > data[1].point.y)
					car->tempmaxy = data[1].point.y;
                        break;
                case CAIRO_PATH_LINE_TO:
                        //fprintf(fp,"L%f,%f ",data[1].point.x, data[1].point.y);
                                zend_printf("L%f,%f ",data[1].point.x, data[1].point.y);
                                if(car->tempmaxy > data[1].point.y)
                                        car->tempmaxy = data[1].point.y;
                        break;
                case CAIRO_PATH_CURVE_TO:
                        //fprintf(fp,"C%f,%f,%f,%f,%f,%f ",data[1].point.x, data[1].point.y, data[2].point.x, data[2].point.y,data[3].point.x, data[3].point.y);
                                zend_printf("C%f,%f,%f,%f,%f,%f ",data[1].point.x, data[1].point.y,data[2].point.x, data[2].point.y, data[3].point.x, data[3].point.y);
                                if(car->tempmaxy > data[1].point.y)
                                        car->tempmaxy = data[1].point.y;
                                if(car->tempmaxy > data[2].point.y)
                                        car->tempmaxy = data[2].point.y;
                                if(car->tempmaxy > data[3].point.y)
                                        car->tempmaxy = data[3].point.y;

                        break;
                case CAIRO_PATH_CLOSE_PATH:
//                      fprintf(fp,"z");
                                zend_printf("z ");
                        break;
            }
        }
        cairo_path_destroy (path);

        cairo_destroy (cr);
        //cairo_surface_write_to_png (surface, "hello.png");
        cairo_surface_destroy (surface);

	car->startx=car->startx+ce.width+5;
	
	RETURN_DOUBLE(car->tempmaxy);

}


PHP_METHOD(Car, accelerate)
{
    Car *car;
    car_object *obj = (car_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    car = obj->car;
    if (car != NULL) {
        car->accelerate();
    }
}

PHP_METHOD(Car, getCurrentSpeed)
{
    Car *car;
    car_object *obj = (car_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    car = obj->car;
    if (car != NULL) {
        RETURN_LONG(car->getCurrentSpeed());
    }
    RETURN_NULL();
}
PHP_METHOD(Car, shift)
{
}
PHP_METHOD(Car, brake)
{
}
PHP_METHOD(Car, getCurrentGear)
{
}

function_entry car_methods[] = {
    PHP_ME(Car,  __construct,     NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Car,  font,            NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Car,  shift,           NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Car,  accelerate,      NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Car,  brake,           NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Car,  getCurrentSpeed, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Car,  getCurrentGear,  NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(vehicles)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Car", car_methods);
    car_ce = zend_register_internal_class(&ce TSRMLS_CC);
    car_ce->create_object = car_create_handler;
    memcpy(&car_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    car_object_handlers.clone_obj = NULL;
    return SUCCESS;
}

zend_module_entry vehicles_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_VEHICLES_EXTNAME,
    NULL,        /* Functions */
    PHP_MINIT(vehicles),        /* MINIT */
    NULL,        /* MSHUTDOWN */
    NULL,        /* RINIT */
    NULL,        /* RSHUTDOWN */
    NULL,        /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_VEHICLES_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_VEHICLES
extern "C" {
ZEND_GET_MODULE(vehicles)
}
#endif
