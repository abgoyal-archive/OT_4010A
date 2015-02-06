

#ifndef AMR_CODEER_DOT_H
#define AMR_CODEER_DOT_H

#ifdef __cplusplus
extern "C"{
#endif 
 

typedef enum { BR475 = 0,
               BR515,            
               BR59,
               BR67,
               BR74,
               BR795,
               BR102,
               BR122,            
	            BRDTX	    
             }AMR_BitRate;

typedef struct {
   void *amr_enc_data;
   int dtx_enable;    
   AMR_BitRate BitRate;                	
}AMR_ENC_HANDLE;

typedef struct {
   void *amr_dec_data;
}AMR_DEC_HANDLE;
 
 
 
void AMREnc_GetBufferSize(
   unsigned int *int_buf_size,
   unsigned int *tmp_buf_size,
   unsigned int *bs_buf_size,
   unsigned int *pcm_buf_size   
);
void AMRDec_GetBufferSize(
   unsigned int *int_buf_size,
   unsigned int *tmp_buf_size,
   unsigned int *bs_buf_size,   
   unsigned int *pcm_buf_size
);


AMR_ENC_HANDLE *AMREnc_Init(
   void *int_buffer,
   AMR_BitRate BitRate,
   int dtx   
);

AMR_DEC_HANDLE *AMRDec_Init(
   void *int_buffer
);


 
int AMR_Encode(
   AMR_ENC_HANDLE *amr_enc_handle,
   void *tmp_buffer,
   short *pcm_buffer,
   unsigned char *bs_buffer, 
	AMR_BitRate bitrate						
);


 
void AMR_Decode(
   AMR_DEC_HANDLE *amr_dec_handle,
   void *tmp_buffer,
   short *pcm_buffer,	
   unsigned char *bs_buffer,
	unsigned char quality_bit,
	unsigned char frame_type      
); 
 
 
 
#ifdef __cplusplus
}
#endif 
#endif


