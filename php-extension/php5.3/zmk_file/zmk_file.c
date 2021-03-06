/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zmk_file.h"

/* If you declare any globals in php_zmk_file.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(zmk_file)
*/

#define RESOURCE_TYPE_ZMK_FILE "zmk_file"

/* True global resources - no need for thread safety here */
static int le_zmk_file;

static void zmk_file_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC){
     FILE *fp = (FILE *) rsrc->ptr;
     fclose(fp);
}

/* {{{ zmk_file_functions[]
 *
 * Every user visible function must have an entry in zmk_file_functions[].
 */
const zend_function_entry zmk_file_functions[] = {
	PHP_FE(confirm_zmk_file_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(file_open,	NULL)
	PHP_FE(file_tell,	NULL)
	PHP_FE(file_seek,	NULL)
	PHP_FE(file_read,	NULL)
	PHP_FE(file_write,	NULL)
	PHP_FE(file_close,	NULL)
	PHP_FE_END	/* Must be the last line in zmk_file_functions[] */
};
/* }}} */

/* {{{ zmk_file_module_entry
 */
zend_module_entry zmk_file_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"zmk_file",
	zmk_file_functions,
	PHP_MINIT(zmk_file),
	PHP_MSHUTDOWN(zmk_file),
	PHP_RINIT(zmk_file),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(zmk_file),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(zmk_file),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_ZMK_FILE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZMK_FILE
ZEND_GET_MODULE(zmk_file)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("zmk_file.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_zmk_file_globals, zmk_file_globals)
    STD_PHP_INI_ENTRY("zmk_file.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_zmk_file_globals, zmk_file_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_zmk_file_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_zmk_file_init_globals(zend_zmk_file_globals *zmk_file_globals)
{
	zmk_file_globals->global_value = 0;
	zmk_file_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zmk_file)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

	le_zmk_file = zend_register_list_destructors_ex(zmk_file_dtor, NULL, RESOURCE_TYPE_ZMK_FILE, module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zmk_file)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(zmk_file)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(zmk_file)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zmk_file)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zmk_file support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_zmk_file_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_zmk_file_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "zmk_file", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto resource file_open(string filename, string mode)
    */
PHP_FUNCTION(file_open)
{
	char *filename = NULL;
	char *mode = NULL;
	int argc = ZEND_NUM_ARGS();
	int filename_len;
	int mode_len;

	if (zend_parse_parameters(argc TSRMLS_CC, "ss", &filename, &filename_len, &mode, &mode_len) == FAILURE) 
		return;

	FILE *fp = VCWD_FOPEN(filename, mode);

	if (fp == NULL) {
		RETURN_FALSE;
	}

	ZEND_REGISTER_RESOURCE(return_value, fp, le_zmk_file);
}
/* }}} */

/* {{{ proto int file_tell(resource filehandle)
    */
PHP_FUNCTION(file_tell)
{
	int argc = ZEND_NUM_ARGS();
	int filehandle_id = -1;
	zval *filehandle = NULL;
	FILE *fp;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &filehandle) == FAILURE) 
		return;

	if (filehandle) {
		ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, filehandle_id, RESOURCE_TYPE_ZMK_FILE, le_zmk_file);
	}

	if (fp == NULL)
	{
		RETURN_LONG(0);
	}

	RETURN_LONG(ftell(fp));
}
/* }}} */

/* {{{ proto int file_seek(resource filehandle, int offset, int whence)
    */
PHP_FUNCTION(file_seek)
{
	int argc = ZEND_NUM_ARGS();
	int filehandle_id = -1;
	long offset;
	long whence;
	zval *filehandle = NULL;
	FILE *fp;

	if (zend_parse_parameters(argc TSRMLS_CC, "rll", &filehandle, &offset, &whence) == FAILURE) 
		return;

	if (filehandle) {
		ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, filehandle_id, RESOURCE_TYPE_ZMK_FILE, le_zmk_file);
	}

	RETURN_LONG(fseek(fp, offset, whence));
}
/* }}} */

/* {{{ proto string file_read(resource filehandle, int size)
    */
PHP_FUNCTION(file_read)
{
	int argc = ZEND_NUM_ARGS();
	int filehandle_id = -1;
	long size;
	zval *filehandle = NULL;
	FILE *fp;
	char *result;
	size_t bytes_read;

	if (zend_parse_parameters(argc TSRMLS_CC, "rl", &filehandle, &size) == FAILURE) 
		return;

	if (filehandle) {
		ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, filehandle_id, RESOURCE_TYPE_ZMK_FILE, le_zmk_file);
	}

	result = (char *) emalloc(size + 1);
	bytes_read = fread(result, 1, size, fp);
	result[bytes_read] = '\0';

	RETURN_STRING(result, 0);
}
/* }}} */

/* {{{ proto bool file_write(resource filehandle, string buffer)
    */
PHP_FUNCTION(file_write)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	int filehandle_id = -1;
	int buffer_len;
	zval *filehandle = NULL;
	FILE *fp;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &filehandle, &buffer, &buffer_len) == FAILURE) 
		return;

	if (filehandle) {
		ZEND_FETCH_RESOURCE(fp, FILE *, &filehandle, filehandle_id, RESOURCE_TYPE_ZMK_FILE, le_zmk_file);
	}

	if (fwrite(buffer, 1, buffer_len, fp) != buffer_len) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool file_close(resource filehandle)
    */
PHP_FUNCTION(file_close)
{
	int argc = ZEND_NUM_ARGS();
	int filehandle_id = -1;
	zval *filehandle = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &filehandle) == FAILURE) 
		return;

	if (zend_list_delete(Z_RESVAL_P(filehandle)) == FAILURE) {
		  RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
