#include "squid.h"
#
struct shaga_plugin * SHAGA_MODULE;
//
struct shaga_plugin * loadShagaPlugin(){

_db_print("Loading shaga module\n");
       lib_handle = dlopen(SHAGA_LIB_NAME, RTLD_NOW);

       if (lib_handle == NULL) {
           snprintf(TMP_MSG,LINE_MAXLEN,"%s\n",dlerror());
	_db_print(TMP_MSG);
	  /* shutdown */
	return (struct shaga_plugin *)1; 
       }
int (*initShagaConnection)(char *,char *,char *,char *,unsigned int) = dlsym(lib_handle,"initShagaConnection");
	if(initShagaConnection == NULL){
	  snprintf(TMP_MSG,LINE_MAXLEN,"%s",dlerror());
        _db_print(TMP_MSG);
        dlclose(lib_handle);
          return (struct shaga_plugin *)1;
	}

int (*writeToDB)(char * __time,char * username,char * ip_address,char * byte) = dlsym(lib_handle, "writeToDB");
         if (writeToDB  == NULL) {
	   snprintf(TMP_MSG,LINE_MAXLEN,"%s\n",dlerror());
	_db_print(TMP_MSG);
	dlclose(lib_handle);
	  return (struct shaga_plugin *)1; 
       }

void (*mysqlClose)() = dlsym(lib_handle,"mysqlClose");
if (mysqlClose  == NULL) {
           snprintf(TMP_MSG,LINE_MAXLEN,"%s\n",dlerror());
        _db_print(TMP_MSG);
        dlclose(lib_handle);
          return (struct shaga_plugin *)1;
       }


if( (initShagaConnection(Config.Shaga.db_host,Config.Shaga.db_user,Config.Shaga.db_pwd,Config.Shaga.db_name,Config.Shaga.db_port) ) == 1){
	_db_print("Cannot connect to shaga database\n");
	return (struct shaga_plugin *)1;

}else _db_print("Connected to database successfully\n");
//
SHAGA_MODULE = (struct shaga_plugin *) xmalloc (sizeof (struct shaga_plugin));
SHAGA_MODULE->handle = lib_handle;
SHAGA_MODULE->storeToDatabase = writeToDB;
SHAGA_MODULE->mysqlClose = mysqlClose;

	snprintf(TMP_MSG,LINE_MAXLEN,"Shaga module loaded\n");
        _db_print(TMP_MSG);

	return SHAGA_MODULE;
}

void unloadShagaPlugin(struct shaga_plugin * plug){
	if(plug->handle != NULL)dlclose(plug->handle);
}
