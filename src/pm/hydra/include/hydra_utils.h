/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2008 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef HYDRA_UTILS_H_INCLUDED
#define HYDRA_UTILS_H_INCLUDED

#include "hydra_base.h"
#include "mpl.h"

#if defined HAVE__FUNC__
#define HYDU_FUNC __func__
#elif defined HAVE_CAP__FUNC__
#define HYDU_FUNC __FUNC__
#elif defined HAVE__FUNCTION__
#define HYDU_FUNC __FUNCTION__
#endif

#if !defined COMPILER_ACCEPTS_VA_ARGS
#define HYDU_Error_printf printf
#elif defined __FILE__ && defined HYDU_FUNC
#define HYDU_Error_printf(...)                                          \
    {                                                                   \
        HYDU_dump_prefix(stderr);                                       \
        HYDU_dump_noprefix(stderr, "%s (%s:%d): ", __func__, __FILE__, __LINE__); \
        HYDU_dump_noprefix(stderr, __VA_ARGS__);                        \
    }
#elif defined __FILE__
#define HYDU_Error_printf(...)                            \
    {                                                     \
        HYDU_dump_prefix(stderr);                                       \
        HYDU_dump_noprefix(stderr, "%s (%d): ", __FILE__, __LINE__);    \
        HYDU_dump_noprefix(stderr, __VA_ARGS__);                        \
    }
#else
#define HYDU_Error_printf(...)                                          \
    {                                                                   \
        HYDU_dump_prefix(stderr);                                       \
        HYDU_dump_noprefix(stderr, __VA_ARGS__);                        \
    }
#endif

#define HYDU_ERR_POP(status, message)                                   \
    {                                                                   \
        if (status != HYD_SUCCESS && status != HYD_GRACEFUL_ABORT) {    \
            if (strlen(message))                                        \
                HYDU_Error_printf(message);                             \
            goto fn_fail;                                               \
        }                                                               \
        else if (status == HYD_GRACEFUL_ABORT) {                        \
            goto fn_exit;                                               \
        }                                                               \
    }

#define HYDU_ERR_SETANDJUMP(status, error, message)                     \
    {                                                                   \
        status = error;                                                 \
        if (status != HYD_SUCCESS && status != HYD_GRACEFUL_ABORT) {    \
            if (strlen(message))                                        \
                HYDU_Error_printf(message);                             \
            goto fn_fail;                                               \
        }                                                               \
        else if (status == HYD_GRACEFUL_ABORT) {                        \
            goto fn_exit;                                               \
        }                                                               \
    }

#define HYDU_ERR_CHKANDJUMP(status, chk, error, message)                \
    {                                                                   \
        if ((chk))                                                      \
            HYDU_ERR_SETANDJUMP(status, error, message);                \
    }

#define HYDU_ERR_CHKANDJUMP1(status, chk, error, message, arg1)         \
    {                                                                   \
        if ((chk))                                                      \
            HYDU_ERR_SETANDJUMP1(status, error, message, arg1);         \
    }

#define HYDU_ERR_SETANDJUMP1(status, error, message, arg1)              \
    {                                                                   \
        status = error;                                                 \
        if (status != HYD_SUCCESS && status != HYD_GRACEFUL_ABORT) {    \
            if (strlen(message))                                        \
                HYDU_Error_printf(message, arg1);                       \
            goto fn_fail;                                               \
        }                                                               \
        else if (status == HYD_GRACEFUL_ABORT) {                        \
            goto fn_exit;                                               \
        }                                                               \
    }

#define HYDU_ERR_SETANDJUMP2(status, error, message, arg1, arg2)        \
    {                                                                   \
        status = error;                                                 \
        if (status != HYD_SUCCESS && status != HYD_GRACEFUL_ABORT) {    \
            if (strlen(message))                                        \
                HYDU_Error_printf(message, arg1, arg2);                 \
            goto fn_fail;                                               \
        }                                                               \
        else if (status == HYD_GRACEFUL_ABORT) {                        \
            goto fn_exit;                                               \
        }                                                               \
    }

#if defined ENABLE_WARNINGS
#define HYDU_Warn_printf HYDU_Error_printf
#else
#define HYDU_Warn_printf(...) {}
#endif /* ENABLE_WARNINGS */

/* We need to add more information in here later */
#if !defined ENABLE_DEBUG
#define HYDU_FUNC_ENTER() {}
#define HYDU_FUNC_EXIT() {}
#else
#define HYDU_FUNC_ENTER() {}
#define HYDU_FUNC_EXIT() {}
#endif


/* args */
HYD_Status HYDU_find_in_path(const char *execname, char **path);
char *HYDU_getcwd(void);
HYD_Status HYDU_get_base_path(const char *execname, char *wdir, char **path);


/* bind */
HYD_Status HYDU_bind_init(char *binding, char *bindlib);
HYD_Status HYDU_bind_process(int core);
int HYDU_bind_get_core_id(int id);


/* debug */
HYD_Status HYDU_dbg_init(const char *str);
void HYDU_dump_prefix(FILE *fp);
void HYDU_dump_noprefix(FILE *fp, const char *str, ...);
void HYDU_dump(FILE *fp, const char *str, ...);


/* env */
HYD_Env_t *HYDU_str_to_env(char *str);
HYD_Env_t *HYDU_str_pair_to_env(const char *env_name, const char *env_value);
HYD_Status HYDU_list_append_env_to_str(HYD_Env_t * env_list, char **str_list);
HYD_Status HYDU_list_inherited_env(HYD_Env_t ** env_list);
HYD_Env_t *HYDU_env_list_dup(HYD_Env_t * env);
HYD_Status HYDU_env_create(HYD_Env_t ** env, const char *env_name, char *env_value);
HYD_Status HYDU_env_free(HYD_Env_t * env);
HYD_Status HYDU_env_free_list(HYD_Env_t * env);
HYD_Env_t *HYDU_env_lookup(HYD_Env_t env, HYD_Env_t * env_list);
HYD_Status HYDU_append_env_to_list(HYD_Env_t env, HYD_Env_t ** env_list);
HYD_Status HYDU_putenv(HYD_Env_t * env, HYD_Env_overwrite_t overwrite);
HYD_Status HYDU_putenv_list(HYD_Env_t * env_list, HYD_Env_overwrite_t overwrite);
HYD_Status HYDU_comma_list_to_env_list(char *str, HYD_Env_t ** env_list);


/* launch */
#if defined HAVE_THREAD_SUPPORT
struct HYD_Thread_context {
    pthread_t thread;
};
#endif /* HAVE_THREAD_SUPPORT */

HYD_Status HYDU_alloc_partition(struct HYD_Partition **partition);
void HYDU_free_partition_list(struct HYD_Partition *partition);
HYD_Status HYDU_alloc_exec_info(struct HYD_Exec_info **exec_info);
void HYDU_free_exec_info_list(struct HYD_Exec_info *exec_info_list);
HYD_Status HYDU_alloc_partition_segment(struct HYD_Partition_segment **segment);
HYD_Status HYDU_merge_partition_segment(char *name, struct HYD_Partition_segment *segment,
                                        struct HYD_Partition **partition_list);
HYD_Status HYDU_merge_partition_mapping(char *name, char *map, int num_procs,
                                        struct HYD_Partition **partition_list);
HYD_Status HYDU_alloc_partition_exec(struct HYD_Partition_exec **exec);
HYD_Status HYDU_create_node_list_from_file(char *host_file,
                                           struct HYD_Partition **partition_list);
HYD_Status HYDU_create_process(char **client_arg, HYD_Env_t * env_list,
                               int *in, int *out, int *err, int *pid, int core);
HYD_Status HYDU_fork_and_exit(int core);
#if defined HAVE_THREAD_SUPPORT
HYD_Status HYDU_create_thread(void *(*func) (void *), void *args,
                              struct HYD_Thread_context *ctxt);
HYD_Status HYDU_join_thread(struct HYD_Thread_context ctxt);
#endif /* HAVE_THREAD_SUPPORT */
int HYDU_local_to_global_id(int local_id, int partition_core_count,
                            struct HYD_Partition_segment *segment_list, int global_core_count);


/* signals */
#ifdef NEEDS_POSIX_FOR_SIGACTION
#define _POSIX_SOURCE
#endif

#include <sys/wait.h>
#if defined(USE_SIGNAL) || defined(USE_SIGACTION)
#include <signal.h>
#else
#error no signal choice
#endif
#ifdef NEEDS_STRSIGNAL_DECL
extern char *strsignal(int);
#endif

HYD_Status HYDU_set_signal(int signum, void (*handler) (int));
HYD_Status HYDU_set_common_signals(void (*handler) (int));


/* Sock utilities */
enum HYDU_sock_comm_flag {
    HYDU_SOCK_COMM_MSGWAIT = 0x01
};

HYD_Status HYDU_sock_listen(int *listen_fd, char *port_range, uint16_t * port);
HYD_Status HYDU_sock_connect(const char *host, uint16_t port, int *fd);
HYD_Status HYDU_sock_accept(int listen_fd, int *fd);
HYD_Status HYDU_sock_readline(int fd, char *buf, int maxlen, int *linelen);
HYD_Status HYDU_sock_writeline(int fd, const char *buf, int maxsize);
HYD_Status HYDU_sock_read(int fd, void *buf, int maxlen, int *count,
                          enum HYDU_sock_comm_flag flag);
HYD_Status HYDU_sock_write(int fd, const void *buf, int maxsize);
HYD_Status HYDU_sock_trywrite(int fd, const void *buf, int maxsize);
HYD_Status HYDU_sock_set_nonblock(int fd);
HYD_Status HYDU_sock_set_cloexec(int fd);
HYD_Status HYDU_sock_stdout_cb(int fd, HYD_Event_t events, int stdout_fd, int *closed);
HYD_Status HYDU_sock_stdin_cb(int fd, HYD_Event_t events, int stdin_fd, char *buf,
                              int *buf_count, int *buf_offset, int *closed);


/* Memory utilities */
#include <ctype.h>

/* FIXME: This should eventually become MPL_malloc and friends */
#define HYDU_malloc malloc
#define HYDU_calloc calloc
#define HYDU_free   free

#define HYDU_snprintf MPL_snprintf
#define HYDU_strdup MPL_strdup

#define HYDU_MALLOC(p, type, size, status)                              \
    {                                                                   \
        (p) = (type) HYDU_malloc((size));                               \
        if ((p) == NULL)                                                \
            HYDU_ERR_SETANDJUMP1((status), HYD_NO_MEM,                  \
                                 "failed to allocate %d bytes\n",       \
                                 (size));                               \
    }

#define HYDU_CALLOC(p, type, num, size, status)                         \
    {                                                                   \
        (p) = (type) HYDU_calloc((num), (size));                        \
        if ((p) == NULL)                                                \
            HYDU_ERR_SETANDJUMP1((status), HYD_NO_MEM,                  \
                                 "failed to allocate %d bytes\n",       \
                                 (size));                               \
    }

#define HYDU_FREE(p)                            \
    {                                           \
        HYDU_free(p);                           \
    }

#define HYDU_STRLIST_CONSOLIDATE(strlist, i, status)                    \
    {                                                                   \
        char *out;                                                      \
        if ((i) >= (HYD_NUM_TMP_STRINGS / 2)) {                         \
            (strlist)[(i)] = NULL;                                      \
            (status) = HYDU_str_alloc_and_join((strlist), &out);        \
            HYDU_ERR_POP((status), "unable to join strings\n");         \
            HYDU_free_strlist((strlist));                               \
            strlist[0] = out;                                           \
            (i) = 1;                                                    \
        }                                                               \
    }

HYD_Status HYDU_list_append_strlist(char **exec, char **client_arg);
HYD_Status HYDU_print_strlist(char **args);
void HYDU_free_strlist(char **args);
HYD_Status HYDU_str_alloc_and_join(char **strlist, char **strjoin);
HYD_Status HYDU_strsplit(char *str, char **str1, char **str2, char sep);
HYD_Status HYDU_strdup_list(char *src[], char **dest[]);
char *HYDU_int_to_str(int x);
char *HYDU_strerror(int error);
int HYDU_strlist_lastidx(char **strlist);
char **HYDU_str_to_strlist(char *str);


/* Timer utilities */
/* FIXME: HYD_Time should be OS specific */
#ifdef HAVE_TIME
#include <time.h>
#endif /* HAVE_TIME */
typedef struct timeval HYD_Time;
void HYDU_time_set(HYD_Time * time, int *val);
int HYDU_time_left(HYD_Time start, HYD_Time timeout);


/* checkpointing */
HYD_Status HYDU_ckpoint_init(char *ckpointlib, char *ckpoint_prefix);
HYD_Status HYDU_ckpoint_suspend(void);
HYD_Status HYDU_ckpoint_restart(HYD_Env_t *envlist, int num_ranks, int ranks[], int *in, int *out, int *err);


#endif /* HYDRA_UTILS_H_INCLUDED */
