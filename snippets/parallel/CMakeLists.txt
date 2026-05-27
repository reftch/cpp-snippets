
add_executable(async_pools async_pools.cc)
add_executable(mutex mutex.cc)
add_executable(thread_pool thread_pool.cc)

# Disable optimizations for testing
set_target_properties(mutex PROPERTIES
    COMPILE_FLAGS "-O0"
    LINK_FLAGS "-O0"
)
