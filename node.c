/*
+----------------------------------------------------------------------+
| PHP Version 7                                                        |
+----------------------------------------------------------------------+
| Copyright (c) 1997-2018 The PHP Group                                |
+----------------------------------------------------------------------+
| This source file is subject to version 3.01 of the PHP license,      |
| that is bundled with this package in the file LICENSE, and is        |
| available through the world-wide-web at the following url:           |
| http://www.php.net/license/3_01.txt                                  |
| If you did not receive a copy of the PHP license and are unable to   |
| obtain it through the world-wide-web, please send a note to          |
| license@php.net so we can mail you a copy immediately.               |
+----------------------------------------------------------------------+
| Author: No Name                                                      |
+----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <zend_interfaces.h>
#include <ext/standard/info.h>

#include "node.h"

extern HashTable         classes;
extern zend_module_entry sample_module_entry;


zend_class_entry     *php_sample_node_class_entry;
zend_object_handlers  php_sample_node_handlers;

//#define TRACE(format, string, option) php_printf(format, string, option)
#define TRACE(format, string, option)

/*----------------------------------------------------------------------+
 | Internal                                                             |
 +----------------------------------------------------------------------*/

static const zend_function_entry php_sample_node_methods[] = {
    PHP_ME(sample_node, __construct, arginfo_sample_node___construct, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* {{{ php_sample_node_create_object */
static zend_object*
php_sample_node_create_object(zend_class_entry *class_type)
{
    php_sample_node *intern = ecalloc(1, sizeof(php_sample_node) + zend_object_properties_size(class_type));

    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    //php_sample_node_properties_init(intern);
    ZVAL_NULL(&intern->data);
    intern->child = NULL;
    intern->parent = NULL;

    intern->ptr = malloc(sizeof(char)*2);
    intern->ptr[0] = 'a';
    intern->ptr[1] = '\0';

    intern->std.handlers = &php_sample_node_handlers;

    TRACE("php_sample_node_create_object(%p) / %d\n", &intern->std, intern->std.gc.refcount);
    return &intern->std;
}
/* }}} php_sample_node_create_object */


static void
php_sample_node_dtor_object(zend_object *obj) {
    php_sample_node *intern = ZOBJ_TO_PHP_SAMPLE_NODE(obj);
    TRACE("php_sample_node_dtor_object(\e[1;31m\"%s\"\e[0;m) / %d\n", intern->data.value.str->val, obj->gc.refcount);


    if (intern->child) {
        zend_object_release(&intern->child->std);
        intern->child=NULL;
    }

    if (intern->parent!=NULL) {
        zend_object_release(&intern->parent->std);
        intern->parent=NULL;
    }

}

/* {{{ php_sample_node_free_object */
static void
php_sample_node_free_object(zend_object *object)
{
    php_sample_node *intern = ZOBJ_TO_PHP_SAMPLE_NODE(object);
    TRACE("php_sample_node_free_object(\e[1;31m\"%s\"\e[0;m) / %d\n", intern->data.value.str->val, object->gc.refcount);

    free(intern->ptr);

    zend_object_std_dtor(&intern->std);
    //efree(intern);
}
/* }}} php_sample_node_free_object */


static HashTable*
php_sample_node_get_debug_info(zval *object, int *is_temp) /* {{{ */
{
    php_sample_node  *obj =  ZVAL_GET_PHP_SAMPLE_NODE(object);
    HashTable   *debug_info,
                *std_props;

    *is_temp = 1;
    std_props = zend_std_get_properties(object);
    debug_info = zend_array_dup(std_props);


    zval data; ZVAL_COPY(&data, &obj->data);
    zend_hash_str_update(debug_info, "data", sizeof("data")-1, &data);

    zval child; ZVAL_SET_PHP_SAMPLE_NODE(&child, obj->child);
    zend_hash_str_update(debug_info, "child", sizeof("child")-1, &child);

    zval parent; ZVAL_SET_PHP_SAMPLE_NODE(&parent, obj->parent);
    zend_hash_str_update(debug_info, "parent", sizeof("parent")-1, &parent);

    return debug_info;
}
/* }}} */


static zend_object_handlers*
php_sample_node_get_handlers()
{
    memcpy(&php_sample_node_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_sample_node_handlers.offset = PHP_SAMPLE_NODE_OFFSET;
    //php_sample_node_handlers.clone_obj;
    //php_sample_node_handlers.compare;
    //php_sample_node_handlers.compare_objects;
    //php_sample_node_handlers.get_constructor;
    php_sample_node_handlers.dtor_obj = php_sample_node_dtor_object;
    php_sample_node_handlers.free_obj = php_sample_node_free_object;
    //php_sample_node_handlers.read_property = php_sample_node_read_property;
    //php_sample_node_handlers.write_property = php_sample_node_write_property;
    //php_sample_node_handlers.unset_property = php_sample_node_unset_property;
    //php_sample_node_handlers.get_property_ptr_ptr = php_sample_node_get_property_ptr_ptr;

    php_sample_node_handlers.get_debug_info = php_sample_node_get_debug_info;
    //php_sample_node_handlers.get_properties = php_sample_node_get_properties;//get_properties_for TODO php 8.0
    //php_sample_node_handlers.set = php_sample_node_set;
    //php_sample_node_handlers.cast_object = php_sample_node_cast_object;

    //php_sample_node_handlers.count_elements = php_sample_node_count_elements;
    //php_sample_node_handlers.has_dimension = php_sample_node_has_dimension;
    //php_sample_node_handlers.read_dimension = php_sample_node_read_dimension;
    //php_sample_node_handlers.unset_dimension = php_sample_node_unset_dimension;
    //php_sample_node_handlers.write_dimension = php_sample_node_write_dimension;


    return &php_sample_node_handlers;
}



/*----------------------------------------------------------------------+
 | PHP_MINIT                                                            |
 +----------------------------------------------------------------------*/

/*{{{ php_sample_node_class_init */
zend_class_entry*
php_sample_node_class_init(zend_class_entry *ce) {
    php_sample_node_get_handlers();
    //INIT_NS_CLASS_ENTRY((*ce), "Sample", "Node", php_sample_node_methods);
    INIT_CLASS_ENTRY((*ce), "Node", php_sample_node_methods);
    ce->create_object = php_sample_node_create_object;
    php_sample_node_class_entry = zend_register_internal_class_ex(ce, NULL);

    return php_sample_node_class_entry;
}/*}}} */


/*----------------------------------------------------------------------+
 | Zend-User API                                                        |
 +----------------------------------------------------------------------*/

php_sample_node*
php_sample_node_append(php_sample_node *list, zval *data) {

    php_sample_node *last = NULL;

    zend_object *new_std = php_sample_node_create_object(php_sample_node_class_entry);// Ref: 1
    php_sample_node *new_node = ZOBJ_TO_PHP_SAMPLE_NODE(new_std);
    ZVAL_COPY(&new_node->data, data);

    if (list) {
        last = php_sample_node_last(list);
        last->child = new_node;
        new_node->parent = last;
        GC_REFCOUNT(&last->std)++;

        return list;
    }

    return new_node;
}

php_sample_node*
php_sample_node_last(php_sample_node *list) {
    php_sample_node *last;

    if (list==NULL) {
        return NULL;
    }

    while(list) {
        last = list;
        list = list->child;
    }

    return last;
}


#define GET_PTR(v) &((v)->std)
//#define GET_PTR(v) v
static char *php_sample_node_dump_zval(zval *data, int tab);

static char*
php_sample_node_dump_zend_string(zend_string *string) {
    char *str = malloc(string->len+100);
    if(ZSTR_IS_INTERNED(string))
        sprintf(str, "\e[2;34m%s\e[0;m(\e[2;35m%d\e[0;m) \e[1;31m\"%s\"\e[0;m", "zend_string", string->gc.refcount, string->val);
    else
        sprintf(str, "\e[2;34m%s\e[0;m(\e[2;35m%d\e[0;m) \e[2;31m\"%s\"\e[0;m", "zend_string", string->gc.refcount, string->val);
    return str;
}

static char*
php_sample_node_dump_zobj(zend_object *obj, int tab) {
    php_sample_node *node = ZOBJ_TO_PHP_SAMPLE_NODE(obj);
    int len = 0;
    char *str = NULL;
    char *t = malloc(50);
    sprintf(t, "%*.s", tab*4, "");

    char *tmp_name = php_sample_node_dump_zval(&node->data, 0);
    len += strlen(tmp_name);

    char *tmp_child;
    if (node->child==NULL) {
        tmp_child = strdup("NULL");
    } else {
        tmp_child = php_sample_node_dump_zobj(&node->child->std, tab+1);
    }
    len += strlen(tmp_child);

    char *tmp_parent;
    if (node->parent==NULL) {
        tmp_parent = strdup("NULL");
    } else {
        tmp_parent = malloc(65+20);
        sprintf(tmp_parent, "\e[2;34m%s\e[0;m(\e[2;35m%d\e[0;m)%p{*RECURSION*}", node->parent->std.ce->name->val, node->parent->std.gc.refcount, GET_PTR(node->parent));
    }
    len += strlen(tmp_parent);
    str = malloc(len+600);

    sprintf(str, "\e[1;34m%s\e[0;m(\e[1;35m%d\e[0;m)%p{\n"
                 "%s    name: %s,\n"
                 "%s    child: %s,\n"
                 "%s    parent: %s,\n"
                 "%s}",
                 obj->ce->name->val, obj->gc.refcount, GET_PTR(node),
                 t, tmp_name,
                 t, tmp_child,
                 t, tmp_parent,
                 t);

    free(t);
    free(tmp_name);
    free(tmp_child);
    free(tmp_parent);

    return str;
}

static char*
php_sample_node_dump_zval(zval *data, int tab) {
    char *str = NULL;
    int size = 0;
    if (ZVAL_IS_NULL(data)) {
        str = strdup("NULL");
    } else if (Z_TYPE_P(data)==IS_STRING) {
        char *str_val = NULL;
        str_val = php_sample_node_dump_zend_string(data->value.str);
        int size = strlen(str_val)+60;
        str = malloc(size);
        sprintf(str, "\e[2;34mzval\e[0;m(\e[2;35m%d\e[0;m){ %s}", data->value.counted->gc.refcount, str_val);
        free(str_val);
    } else if (Z_TYPE_P(data)==IS_LONG) {
        str = malloc(80);
        sprintf(str, "\e[2;34mzval\e[0;m(\e[2;35m%d\e[0;m){ %ld}", data->value.counted->gc.refcount, data->value.lval);
    } else if (Z_TYPE_P(data)==IS_OBJECT && data->value.obj->ce==php_sample_node_class_entry) {
        char *tmp_parent = php_sample_node_dump_zobj(data->value.obj, tab+1);
        size = strlen(tmp_parent)+120;
        str = malloc(size);

        sprintf(str, "\e[2;34m%s\e[0;m(\e[2;35m%d\e[0;m){ %s\n}\n",
                "zval",
                data->value.counted->gc.refcount,
                tmp_parent);
        free(tmp_parent);
    } else {
        str = malloc(32);
        sprintf(str, "%d", Z_TYPE_P(data));
    }
    return str;
}

/*----------------------------------------------------------------------+
 | PHP_METHOD                                                           |
 +----------------------------------------------------------------------*/

/* {{{ Sample\\Node::__construct() */
PHP_METHOD(sample_node, __construct)
{
    zval *data=NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(data)
    ZEND_PARSE_PARAMETERS_END();

    zend_object *zobj = Z_OBJ_P(getThis());
    php_sample_node *self = ZOBJ_TO_PHP_SAMPLE_NODE(zobj);

    if (data!=NULL) {
        ZVAL_COPY(&self->data, data);
    }
}
/* }}} Sample\\Node::__construct() */

/*----------------------------------------------------------------------+
 | PHP_FUNCTION                                                         |
 +----------------------------------------------------------------------*/

/* {{{ */
PHP_FUNCTION(sample_node_append)
{
    zval *list = NULL;
    zval *data = NULL;
    zval ret;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(list)
        Z_PARAM_ZVAL(data)
    ZEND_PARSE_PARAMETERS_END();

    php_sample_node *__list = ZVAL_IS_PHP_SAMPLE_NODE(list)? ZVAL_GET_PHP_SAMPLE_NODE(list): NULL;
    php_sample_node *__ret = php_sample_node_append(__list, data);

    if(__list)
        GC_REFCOUNT(&__ret->std)++;
    RETURN_OBJ(&__ret->std);
}
/* }}} */


/* {{{ */
PHP_FUNCTION(sample_node_last)
{
    zval *list = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(list)
    ZEND_PARSE_PARAMETERS_END();

    php_sample_node *__list = ZVAL_IS_PHP_SAMPLE_NODE(list)? ZVAL_GET_PHP_SAMPLE_NODE(list): NULL;
    php_sample_node *__ret = php_sample_node_last(__list);

    GC_REFCOUNT(&__ret->std)++;
    RETURN_OBJ(&__ret->std);
}


/* {{{ */
PHP_FUNCTION(sample_node_dump)
{
    zval *sample = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &sample) == FAILURE) {
        return;
    }

    char *str = php_sample_node_dump_zval(sample, 0);
    php_printf("%s\n", str);
    free(str);

    RETURN_ZVAL(sample, 1, 0);
}
/* }}} */
