#include <mkl.h>
#include "hbwmalloc.h"


//implement scratch buffer on HBM and compute FFTs, refer instructions on Lab page
void runFFTs( const size_t fft_size, const size_t num_fft, MKL_Complex8 *data, DFTI_DESCRIPTOR_HANDLE *fftHandle) {

  MKL_Complex8 *buffer;
  hbw_posix_memalign((void**) &buffer, 4096, sizeof(MKL_Complex8)*fft_size);

  for(size_t i = 0; i < num_fft; i++) {

    MKL_Complex8 *ptr = &data[i*fft_size];
    
    #pragma omp parallel for
    for(size_t j = 0; j <fft_size; j++) {
      buffer[j] = ptr[j];
    }

    DftiComputeForward (*fftHandle, buffer);

    #pragma omp parallel for
    for(size_t j = 0; j <fft_size; j++) {
      ptr[j] = buffer[j];
    }

  }

  hbw_free(buffer);
}
