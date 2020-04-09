#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define RAFT_LIBRARY_API

extern "C"
{
    typedef struct raft_t raft_t;
    typedef struct raft_server_t raft_server_t;
    typedef struct raft_options_t raft_options_t;
    typedef struct raft_server_options_t raft_server_options_t;
    typedef struct raft_server_cache raft_server_cache;
    typedef struct raft_cache raft_cache;
    typedef struct raft_term_info raft_term_info;
    typedef struct raft_peers_t raft_peers_t;
    typedef struct raft_state_machine_t raft_state_machine_t;
    typedef struct jim_status_t jim_status_t;
    typedef struct raft_status_t raft_status_t;
    typedef struct raft_snapshot_t raft_snapshot_t;
    typedef struct raft_conf_change_t raft_conf_change_t;
    typedef struct raft_node_resolver_t raft_node_resolver_t;
    typedef struct raft_log_reader_t raft_log_reader_t;

    //raft status related
    extern RAFT_LIBRARY_API jim_status_t *jim_status_create(uint16_t code);
    extern RAFT_LIBRARY_API jim_status_t *jim_status_create_with_msg(uint16_t code, char *msg1, char *msg2);
    extern RAFT_LIBRARY_API char *jim_status_get_string(jim_status_t *status);
    extern RAFT_LIBRARY_API uint16_t jim_status_get_code(jim_status_t *status);
    extern RAFT_LIBRARY_API void jim_free_jim_status_t(jim_status_t *status);
    extern RAFT_LIBRARY_API void jim_free_jim_status_string(char *string);

    // node_resolver related
    extern RAFT_LIBRARY_API raft_node_resolver_t *raft_node_resolver_create(
        void *nresolver_,
        void (*destructor_)(void *),
        char *(*get_node_address)(void *, uint64_t node_id));
    extern RAFT_LIBRARY_API void raft_free_raft_node_resolver(raft_node_resolver_t *nresolver);

    //raft server related
    //1.raft server options
    extern RAFT_LIBRARY_API raft_server_options_t *raft_server_options_create();
    extern RAFT_LIBRARY_API void raft_free_raft_server_options_t(raft_server_options_t *srv_options);
    extern RAFT_LIBRARY_API void raft_server_options_set_node_id(raft_server_options_t &ops, uint64_t id);
    extern RAFT_LIBRARY_API void raft_server_options_set_tick_interval(raft_server_options_t &ops, uint64_t interval);
    extern RAFT_LIBRARY_API void raft_server_options_set_election_tick(raft_server_options_t &ops, uint64_t tick);
    extern RAFT_LIBRARY_API void raft_server_options_set_use_inprocess_transport(raft_server_options_t &ops, bool flag);
    extern RAFT_LIBRARY_API void raft_server_options_set_listen_ip(raft_server_options_t &ops, char *ip);
    extern RAFT_LIBRARY_API void raft_server_options_set_listen_port(raft_server_options_t &ops, uint16_t port);
    extern RAFT_LIBRARY_API void raft_server_options_set_send_io_threads(raft_server_options_t &ops, size_t num);
    extern RAFT_LIBRARY_API void raft_server_options_set_recv_io_threads(raft_server_options_t &ops, size_t num);
    extern RAFT_LIBRARY_API void raft_server_options_set_connection_pool_size(raft_server_options_t &ops, size_t size);
    extern RAFT_LIBRARY_API void raft_server_options_set_node_resolver(raft_server_options_t &ops, raft_node_resolver_t *nr);

    //2.raft server create
    extern RAFT_LIBRARY_API raft_server_cache *raft_server_create(const raft_server_options_t &ops);
    extern RAFT_LIBRARY_API void raft_free_raft_server_cache(raft_server_cache *cache);
    //3.raft server create
    extern RAFT_LIBRARY_API void raft_server_start(raft_server_cache &server);

    //raft options related
    //1.raft state machine
    extern RAFT_LIBRARY_API
        raft_state_machine_t *
        raft_state_machine_create(
            void *state_,
            void (*destructor_)(void *),
            uint16_t (*apply)(void *, const char *data, const size_t data_size, const uint64_t index, uint64_t term, uint16_t rep_status, void *tag),
            uint16_t (*apply_member_change)(void *, const raft_conf_change_t *, uint64_t),
            uint16_t (*apply_read_index)(void *, const char *, uint16_t),
            uint64_t (*persisti_applied)(void *),
            void (*on_leader_change)(void *, uint64_t, uint64_t),
            raft_snapshot_t *(*get_snapshot)(void *),
            uint16_t (*apply_snapshot_start)(void *, const char *context, uint32_t context_size, uint64_t index),
            uint16_t (*apply_snapshot_data)(void *, const char **dataArray, uint32_t dataNums, uint32_t *dataSizeList),
            uint16_t (*apply_snapshot_finish)(void *, uint64_t));
    extern RAFT_LIBRARY_API void raft_free_raft_state_machine_t(raft_state_machine_t *sm);
    //2.raft peer
    extern RAFT_LIBRARY_API raft_peers_t *raft_peers_create();
    extern RAFT_LIBRARY_API void raft_add_peers(raft_peers_t *peers, int8_t type, uint64_t node_id, uint64_t peer_id);
    extern RAFT_LIBRARY_API void raft_free_raft_peers_t(raft_peers_t *peers);
    extern RAFT_LIBRARY_API raft_options_t *raft_options_create();
    extern RAFT_LIBRARY_API void raft_free_raft_options_t(raft_options_t *option);
    extern RAFT_LIBRARY_API void raft_options_set_id(raft_options_t &ops, uint64_t id);
    extern RAFT_LIBRARY_API void raft_options_set_peer(raft_options_t &ops, raft_peers_t &peer);
    extern RAFT_LIBRARY_API void raft_options_set_state_machine(raft_options_t &ops, raft_state_machine_t *sm);
    extern RAFT_LIBRARY_API void raft_options_use_memory_storage(raft_options_t &ops, bool flag);
    //3.raft create
    extern RAFT_LIBRARY_API raft_cache *raft_create(raft_server_cache &server, raft_options_t &ops);

    extern RAFT_LIBRARY_API void raft_free_raft_cache(raft_cache *cache);

    extern RAFT_LIBRARY_API bool raft_is_stopped(raft_cache &raft);

    extern RAFT_LIBRARY_API bool raft_is_leader(raft_cache &raft);

    extern RAFT_LIBRARY_API raft_term_info *raft_get_leader_term(raft_cache &raft);

    extern RAFT_LIBRARY_API uint64_t raft_get_leader_from_raft_term_info(raft_term_info *info);

    extern RAFT_LIBRARY_API uint64_t raft_get_term_from_raft_term_info(raft_term_info *info);

    extern RAFT_LIBRARY_API void raft_free_raft_term_info(raft_term_info *info);

    extern RAFT_LIBRARY_API jim_status_t *raft_try_to_leader(raft_cache &raft);

    extern RAFT_LIBRARY_API void raft_propose(raft_cache &raft, char *data, size_t size, uint32_t flag, void *tag);

    extern RAFT_LIBRARY_API jim_status_t *raft_read_index(raft_cache &raft, char *ctx);

    extern RAFT_LIBRARY_API raft_log_reader_t *raft_begin_read_log(raft_cache &raft, uint64_t start_index);

    extern RAFT_LIBRARY_API void raft_end_read_log(raft_log_reader_t *log_reader);

    extern RAFT_LIBRARY_API jim_status_t *raft_log_reader_next(raft_log_reader_t *log_reader, uint64_t *index, char **data, size_t *len, bool *over);

    extern RAFT_LIBRARY_API uint64_t raft_log_reader_last_index(raft_log_reader_t *log_reader);

    extern RAFT_LIBRARY_API jim_status_t *raft_change_member(raft_cache &raft, const raft_conf_change_t &conf);
}
