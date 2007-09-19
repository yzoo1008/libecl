#include <multflt.h>
#include <multflt_config.h>
#include <util.h>

static multflt_config_type  * MULTFLT_CONFIG = NULL;
static multflt_type        ** MULTFLT_LIST        = NULL;

/*****************************************************************/

void multflt_inter_init__(const char * _config_file, const int * config_file_len , const int * ens_size, const int * n_multflt, const int *nx , const int * ny , const int * nflt) {
  int iens;
  char * config_file = util_alloc_cstring(_config_file , config_file_len);
  MULTFLT_CONFIG = multflt_config_fscanf_alloc(config_file , "MULTFLT.INC" , NULL);
  
  if (*n_multflt != multflt_config_get_serial_size(MULTFLT_CONFIG)) {
    fprintf(stderr,"%s: size mismatch config_file:%d  mod_dimensions.F90/num_multflt:%d - aborting \n",__func__ , multflt_config_get_serial_size(MULTFLT_CONFIG) , *n_multflt);
    abort();
  }


  MULTFLT_LIST = malloc(*ens_size * sizeof * MULTFLT_LIST);
  
  for (iens = 0; iens < *ens_size; iens++) 
    MULTFLT_LIST[iens] = multflt_alloc(MULTFLT_CONFIG);

  free(config_file);
}



/*
  -1 to account for Fortran offset
*/

void multflt_inter_ecl_write__(const char *_path , const int * path_len , const double *data , const int *iens) {
  char *path = util_alloc_cstring(_path , path_len);

  multflt_set_data(MULTFLT_LIST[(*iens) - 1] , data);
  multflt_ecl_write(MULTFLT_LIST[(*iens) - 1] , path);
  
  free(path);
}


void multflt_get_data__(const int * iens, double * data) {
  multflt_get_data(MULTFLT_LIST[(*iens) - 1] , data);
}


void multflt_inter_sample__(const int * iens, double * data) {
  multflt_sample(MULTFLT_LIST[(*iens) - 1]);
  multflt_get_data(MULTFLT_LIST[(*iens) - 1] , data);
}


void multflt_inter_truncate__(const int * iens , double * data) {
  multflt_set_data(MULTFLT_LIST[(*iens) - 1] , data);
  multflt_truncate(MULTFLT_LIST[(*iens) - 1]);
  multflt_get_data(MULTFLT_LIST[(*iens) - 1] , data);
}
