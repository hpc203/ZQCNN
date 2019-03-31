#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "zq_cnn_base_operator.h"
#include "../ZQ_CNN_CompileConfig.h"
#include "zq_cnn_batchnormscale_nchwc.h"
#if __ARM_NEON
#include <arm_neon.h>
#else
#if defined(__GNUC__)
#if ZQ_CNN_USE_SSETYPE >= ZQ_CNN_SSETYPE_SSE
#include <smmintrin.h>
#endif
#if ZQ_CNN_USE_SSETYPE >= ZQ_CNN_SSETYPE_AVX
#include <x86intrin.h>
#endif
#elif defined(_WIN32)
#if ZQ_CNN_USE_SSETYPE >= ZQ_CNN_SSETYPE_SSE
#include <mmintrin.h> //MMX  
#include <xmmintrin.h> //SSE(include mmintrin.h)  
#include <emmintrin.h> //SSE2(include xmmintrin.h)  
#include <pmmintrin.h> //SSE3(include emmintrin.h)  
#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#endif 
#if ZQ_CNN_USE_SSETYPE >= ZQ_CNN_SSETYPE_AVX
#include <wmmintrin.h>//AES(include nmmintrin.h)  
#include <immintrin.h>//AVX(include wmmintrin.h)  
#include <intrin.h>//(include immintrin.h)  
#endif
#endif
#endif //__ARM_NEON


#if defined(__cplusplus) || defined(c_plusplus) 
extern "C" {
#endif

#ifndef FLOAT_EPS_FOR_DIV 
#define FLOAT_EPS_FOR_DIV 1e-32
#endif

#if __ARM_NEON
#define zq_cnn_batchnormscale_mean_var_scale_bias_nchwc zq_cnn_batchnormscale_mean_var_scale_bias_nchwc4
#define zq_cnn_batchnorm_mean_var_nchwc zq_cnn_batchnorm_mean_var_nchwc4
#define zq_cnn_scale_nchwc zq_cnn_scale_nchwc4
#define zq_cnn_batchnorm_b_a_nchwc zq_cnn_batchnorm_b_a_nchwc4
#define zq_mm_load_ps vld1q_f32
#define zq_mm_store_ps vst1q_f32
#define zq_mm_add_ps vaddq_f32
#define zq_mm_mul_ps vmulq_f32
#if ZQ_CNN_USE_FMADD128
#define zq_mm_fmadd_ps(A, B, C) vfmaq_f32(C, A, B)
#else
#define zq_mm_fmadd_ps(A, B, C) vaddq_f32(vmulq_f32(A, B), C)
#endif
#define zq_mm_type float32x4_t
#define zq_base_type float
#define zq_mm_align_size 4

#include "zq_cnn_batchnormscale_nchwc_raw.h"


#undef zq_cnn_batchnormscale_mean_var_scale_bias_nchwc
#undef zq_cnn_batchnorm_mean_var_nchwc
#undef zq_cnn_scale_nchwc
#undef zq_cnn_batchnorm_b_a_nchwc
#undef zq_mm_load_ps
#undef zq_mm_store_ps
#undef zq_mm_add_ps
#undef zq_mm_mul_ps
#undef zq_mm_fmadd_ps
#undef zq_mm_type
#undef zq_base_type
#undef zq_mm_align_size

#else
#if ZQ_CNN_USE_SSETYPE >= ZQ_CNN_SSETYPE_SSE
#define zq_cnn_batchnormscale_mean_var_scale_bias_nchwc zq_cnn_batchnormscale_mean_var_scale_bias_nchwc4
#define zq_cnn_batchnorm_mean_var_nchwc zq_cnn_batchnorm_mean_var_nchwc4
#define zq_cnn_scale_nchwc zq_cnn_scale_nchwc4
#define zq_cnn_batchnorm_b_a_nchwc zq_cnn_batchnorm_b_a_nchwc4
#define zq_mm_load_ps _mm_load_ps
#define zq_mm_store_ps _mm_store_ps
#define zq_mm_add_ps _mm_add_ps
#define zq_mm_mul_ps _mm_mul_ps
#if ZQ_CNN_USE_FMADD128
#define zq_mm_fmadd_ps _mm_fmadd_ps
#else
#define zq_mm_fmadd_ps(A, B, C) _mm_add_ps(_mm_mul_ps(A, B), C)
#endif
#define zq_mm_type __m128
#define zq_base_type float
#define zq_mm_align_size 4

#include "zq_cnn_batchnormscale_nchwc_raw.h"


#undef zq_cnn_batchnormscale_mean_var_scale_bias_nchwc
#undef zq_cnn_batchnorm_mean_var_nchwc
#undef zq_cnn_scale_nchwc
#undef zq_cnn_batchnorm_b_a_nchwc
#undef zq_mm_load_ps
#undef zq_mm_store_ps
#undef zq_mm_add_ps
#undef zq_mm_mul_ps
#undef zq_mm_fmadd_ps
#undef zq_mm_type
#undef zq_base_type
#undef zq_mm_align_size

#endif

#if ZQ_CNN_USE_SSETYPE >= ZQ_CNN_SSETYPE_AVX

#define zq_cnn_batchnormscale_mean_var_scale_bias_nchwc zq_cnn_batchnormscale_mean_var_scale_bias_nchwc8
#define zq_cnn_batchnorm_mean_var_nchwc zq_cnn_batchnorm_32f_mean_var_nchwc8
#define zq_cnn_scale_nchwc zq_cnn_scale_nchwc8
#define zq_cnn_batchnorm_b_a_nchwc zq_cnn_batchnorm_b_a_nchwc8
#define zq_mm_load_ps _mm256_load_ps
#define zq_mm_store_ps _mm256_store_ps
#define zq_mm_add_ps _mm256_add_ps
#define zq_mm_mul_ps _mm256_mul_ps
#if ZQ_CNN_USE_FMADD256
#define zq_mm_fmadd_ps _mm256_fmadd_ps
#else
#define zq_mm_fmadd_ps(A, B, C) _mm256_add_ps(_mm256_mul_ps(A, B), C)
#endif
#define zq_mm_type __m256
#define zq_base_type float
#define zq_mm_align_size 8

#include "zq_cnn_batchnormscale_nchwc_raw.h"


#undef zq_cnn_batchnormscale_mean_var_scale_bias_nchwc
#undef zq_cnn_batchnorm_mean_var_nchwc
#undef zq_cnn_scale_nchwc
#undef zq_cnn_batchnorm_b_a_nchwc
#undef zq_mm_load_ps
#undef zq_mm_store_ps
#undef zq_mm_add_ps
#undef zq_mm_mul_ps
#undef zq_mm_fmadd_ps
#undef zq_mm_type
#undef zq_base_type
#undef zq_mm_align_size
#endif
#endif //__ARM_NEON
	
#define zq_cnn_batchnormscale_mean_var_scale_bias_nchwc zq_cnn_batchnormscale_mean_var_scale_bias_nchwc1
#define zq_cnn_batchnorm_mean_var_nchwc zq_cnn_batchnorm_mean_var_nchwc1
#define zq_cnn_scale_nchwc zq_cnn_scale_nchwc1
#define zq_cnn_batchnorm_b_a_nchwc zq_cnn_batchnorm_b_a_nchwc1
#define zq_mm_load_ps my_mm_load_ps
#define zq_mm_store_ps my_mm_store_ps
#define zq_mm_add_ps my_mm_add_ps
#define zq_mm_mul_ps my_mm_mul_ps
#define zq_mm_fmadd_ps my_mm_fmadd_ps
#define zq_mm_type float
#define zq_base_type float
#define zq_mm_align_size 1

#include "zq_cnn_batchnormscale_nchwc_raw.h"


#undef zq_cnn_batchnormscale_mean_var_scale_bias_nchwc
#undef zq_cnn_batchnorm_mean_var_nchwc
#undef zq_cnn_scale_nchwc
#undef zq_cnn_batchnorm_b_a_nchwc
#undef zq_mm_load_ps
#undef zq_mm_store_ps
#undef zq_mm_add_ps
#undef zq_mm_mul_ps
#undef zq_mm_fmadd_ps
#undef zq_mm_type
#undef zq_base_type
#undef zq_mm_align_size

#if defined(__cplusplus) || defined(c_plusplus) 
}
#endif