#include "parallel.h"

#include <memory>

#include "backend.h"

namespace ctranslate2 {
  namespace cpu {

#ifndef _OPENMP

    static thread_local size_t num_threads = 1;

    void set_num_threads(size_t num) {
      num_threads = num;
    }

    size_t get_num_threads() {
      return num_threads;
    }

    // Created on first use and destroyed by release_thread_resources(), not only by thread exit.
    static thread_local std::unique_ptr<BS::light_thread_pool> thread_pool;

    BS::light_thread_pool& get_thread_pool() {
      if (!thread_pool)
        thread_pool = std::make_unique<BS::light_thread_pool>(num_threads);
      return *thread_pool;
    }

#endif

    void release_thread_resources() {
#ifndef _OPENMP
      thread_pool.reset();
#endif
#ifdef CT2_WITH_RUY
      release_ruy_context();
#endif
    }

  }
}
