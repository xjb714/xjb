#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

// 定义线程参数结构
typedef struct {
    uint32_t start;
    uint32_t end;
    uint64_t local_error_sum;  // 线程局部累加结果
    uint32_t (*check_func_float)(float);  // 要执行的检查函数
} thread_data_t;

// 线程函数
void* thread_worker(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    data->local_error_sum = 0;

    // 处理分配的区间
    for (uint32_t i = data->start; i <= data->end; ++i) {
        float f = *(float*)&i;
        data->local_error_sum += data->check_func_float(f);
    }

    return NULL;
}

// 主函数：创建线程并执行并行计算
uint64_t parallel_reduce_pthreads(uint32_t start_val, uint32_t end_val,
                                unsigned (*check_func_float)(float), int num_threads) {
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    thread_data_t* thread_data = (thread_data_t*)malloc(num_threads * sizeof(thread_data_t));

    // 计算每个线程处理的区间大小
    uint32_t total_values = end_val - start_val + 1;
    uint32_t values_per_thread = total_values / num_threads;
    uint32_t remainder = total_values % num_threads;

    // 创建并启动线程
    uint32_t current_start = start_val;
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].start = current_start;

        // 分配任务，处理余数分布
        uint32_t thread_count = values_per_thread;
        if (t < remainder) {
            thread_count++;  // 前几个线程多处理一个元素
        }

        thread_data[t].end = current_start + thread_count - 1;
        thread_data[t].check_func_float = check_func_float;

        pthread_create(&threads[t], NULL, thread_worker, &thread_data[t]);

        current_start += thread_count;
    }

    // 等待所有线程完成
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    // 归约所有线程的局部结果
    uint64_t global_error_sum = 0;
    for (int t = 0; t < num_threads; t++) {
        global_error_sum += thread_data[t].local_error_sum;
    }

    // 清理资源
    free(threads);
    free(thread_data);

    return global_error_sum;
}
