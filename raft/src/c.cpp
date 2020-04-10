#include "raft/c.h"
#include "raft/raft.h"
#include "raft/options.h"
#include "raft/server.h"
#include "raft/status.h"
#include "raft/types.h"
#include "base/status.h"
#include <string>
#include "common/logger.h"
using jim::Status;
using jim::raft::CmdResult;
using jim::raft::ConfChange;
using jim::raft::LogReader;
using jim::raft::NodeResolver;
using jim::raft::Peer;
using jim::raft::PeerType;
using jim::raft::Raft;
using jim::raft::RaftOptions;
using jim::raft::RaftServer;
using jim::raft::RaftServerOptions;
using jim::raft::RaftStatus;
using jim::raft::Snapshot;
using jim::raft::SnapshotOptions;
using jim::raft::StateMachine;
using jim::raft::TransportOptions;

extern "C"
{
    struct jim_status_t
    {
        Status rep;
    };

    struct raft_status_t
    {
        RaftStatus rep;
    };

    // Note: free it after use
    jim_status_t *jim_status_create_with_msg(uint16_t code, char *msg1, char *msg2)
    {
        Status rep = Status((Status::Code)(code), std::string(msg1), std::string(msg2));
        jim_status_t *js = new jim_status_t;
        js->rep = rep;
        return js;
    }

    jim_status_t *jim_status_create(uint16_t code)
    {
        Status rep = Status((Status::Code)(code));
        jim_status_t *js = new jim_status_t;
        js->rep = rep;
        return js;
    }

    void jim_free_jim_status_t(jim_status_t *status)
    {
        free(status);
    }

    // Note: free it after use
    char *jim_status_get_string(jim_status_t *status)
    {
        FLOG_DEBUG("jim_status_string: %s\n", status->rep.ToString().c_str());
        return strdup(status->rep.ToString().c_str());
    }

    void jim_free_jim_status_string(char *str)
    {
        free(str);
    }

    uint16_t jim_status_get_code(jim_status_t *status)
    {
        return static_cast<uint16_t>(status->rep.code());
    }

    struct raft_t
    {
        Raft *rep;
    };

    struct raft_server_t
    {
        RaftServer *rep;
    };

    struct transport_options_t
    {
        TransportOptions rep;
    };

    struct snapshot_options_t
    {
        SnapshotOptions rep;
    };

    struct raft_log_reader_t
    {
        std::unique_ptr<LogReader> rep;
    };

    struct raft_server_cache
    {
        std::unique_ptr<raft_server_t> rep;
    };

    struct raft_cache
    {
        std::shared_ptr<Raft> rep;
    };

    struct raft_term_info
    {
        uint64_t leader;
        uint64_t term;
    };

    //
    struct raft_peers_t
    {
        std::vector<Peer> rep;
    };

    raft_peers_t *raft_peers_create()
    {
        raft_peers_t *peers = new raft_peers_t;
        return peers;
    };

    void raft_free_raft_peers_t(raft_peers_t *peers)
    {
        free(peers);
    }

    void raft_add_peers(raft_peers_t *peers, int8_t type, uint64_t node_id, uint64_t peer_id)
    {
        Peer peer;
        peer.node_id = node_id;
        peer.peer_id = peer_id;
        peer.type = (PeerType)type;
        peers->rep.push_back(peer);
    }

    struct raft_conf_change_t
    {
        ConfChange rep;
    };

    // raft option related
    struct raft_options_t
    {
        RaftOptions rep;
    };

    raft_options_t *raft_options_create()
    {
        return new raft_options_t;
    }

    void raft_free_raft_options_t(raft_options_t *options)
    {
        free(options);
    }

    void raft_options_set_id(raft_options_t &ops, uint64_t id)
    {
        ops.rep.id = id;
    }
    void raft_options_set_peer(raft_options_t &ops, raft_peers_t &peer)
    {
        ops.rep.peers = peer.rep;
    }
    void raft_options_set_state_machine(raft_options_t &ops, raft_state_machine_t *sm)
    {
        ops.rep.statemachine = std::static_pointer_cast<StateMachine>(std::shared_ptr<raft_state_machine_t>(sm));
    }
    void raft_options_set_storage_path(raft_options_t &ops, char *spath)
    {
        ops.rep.storage_path = std::string(spath);
    }
    void raft_options_use_memory_storage(raft_options_t &ops, bool flag)
    {
        ops.rep.use_memory_storage = flag;
    }

    //raft server option related
    struct raft_server_options_t
    {
        RaftServerOptions rep;
    };

    raft_server_options_t *raft_server_options_create()
    {
        return new raft_server_options_t;
    }
    void raft_free_raft_server_options_t(raft_server_options_t *srv_options)
    {
        free(srv_options);
    }
    void raft_server_options_set_node_id(raft_server_options_t &ops, uint64_t id)
    {
        ops.rep.node_id = id;
    }
    void raft_server_options_set_tick_interval(raft_server_options_t &ops, uint64_t interval)
    {
        ops.rep.tick_interval = std::chrono::milliseconds(interval);
    }
    void raft_server_options_set_election_tick(raft_server_options_t &ops, uint64_t tick)
    {
        ops.rep.election_tick = tick;
    }
    void raft_server_options_set_use_inprocess_transport(raft_server_options_t &ops, bool flag)
    {
        ops.rep.transport_options.use_inprocess_transport = flag;
    }
    void raft_server_options_set_listen_ip(raft_server_options_t &ops, char *ip)
    {
        ops.rep.transport_options.listen_ip = std::string(ip);
    }
    void raft_server_options_set_listen_port(raft_server_options_t &ops, uint16_t port)
    {
        ops.rep.transport_options.listen_port = port;
    }
    void raft_server_options_set_send_io_threads(raft_server_options_t &ops, size_t num)
    {
        ops.rep.transport_options.send_io_threads = num;
    }
    void raft_server_options_set_recv_io_threads(raft_server_options_t &ops, size_t num)
    {
        ops.rep.transport_options.recv_io_threads = num;
    }
    void raft_server_options_set_connection_pool_size(raft_server_options_t &ops, size_t size)
    {
        ops.rep.transport_options.connection_pool_size = size;
    }
    void raft_server_options_set_node_resolver(raft_server_options_t &ops, raft_node_resolver_t *nr)
    {
        ops.rep.transport_options.resolver = std::static_pointer_cast<NodeResolver>(std::shared_ptr<raft_node_resolver_t>(nr));
    }

    // snapshot related
    struct raft_snapshot_t : public Snapshot
    {
        void *state_;
        void (*destructor_)(void *);
        jim_status_t (*next)(std::string *, bool *);
        jim_status_t (*context_)(char *);
        uint64_t (*apply_index)(void *);
        void (*close)(void *);

        ~raft_snapshot_t() override { (*destructor_)(state_); }

        Status Next(std::string *data, bool *over) override
        {

            return (*next)(data, over).rep;
        }

        Status Context(std::string *context) override
        {
            return (*context_)(const_cast<char *>(context->c_str())).rep;
        }

        uint64_t ApplyIndex() override
        {
            return (*apply_index)(state_);
        }

        void Close() override
        {
            (*close)(state_);
        }
    };

    // node_resolver related
    struct raft_node_resolver_t : public NodeResolver
    {
        void *nresolver_;
        void (*destructor_)(void *);
        char *(*get_node_address)(void *, uint64_t node_id);

        raft_node_resolver_t()
        {
            // do nothing
        }

        raft_node_resolver_t(const raft_node_resolver_t &)
        {
            //TODO impl me
        }

        ~raft_node_resolver_t() override
        {
            (*destructor_)(nresolver_);
        }

        std::string GetNodeAddress(uint64_t node_id) override
        {
            char *ip = (*get_node_address)(nresolver_, node_id);
            return std::string(ip);
        }
    };

    raft_node_resolver_t *raft_node_resolver_create(
        void *nresolver_,
        void (*destructor_)(void *),
        char *(*get_node_address)(void *, uint64_t node_id))
    {
        raft_node_resolver_t *result = new raft_node_resolver_t;
        result->nresolver_ = nresolver_;
        result->destructor_ = destructor_;
        result->get_node_address = get_node_address;
        return result;
    }

    void raft_free_raft_node_resolver(raft_node_resolver_t *nresolver)
    {
        free(nresolver);
    }

    struct raft_state_machine_t : public StateMachine
    {
        void *state_;
        void (*destructor_)(void *);
        uint16_t (*apply)(void *, const char *data, const size_t data_size, const uint64_t index, uint64_t term, uint16_t rep_status, void *tag);
        uint16_t (*apply_member_change)(void *, const raft_conf_change_t *, uint64_t);
        uint16_t (*apply_read_index)(void *, const char *, uint16_t);
        uint64_t (*persisti_applied)(void *);
        void (*on_leader_change)(void *, uint64_t, uint64_t);
        raft_snapshot_t *(*get_snapshot)(void *);
        uint16_t (*apply_snapshot_start)(void *, const char *context, uint32_t context_size, uint64_t index);
        uint16_t (*apply_snapshot_data)(void *, const char **dataArray, uint32_t dataNums, uint32_t *dataSizeList);
        uint16_t (*apply_snapshot_finish)(void *, uint64_t);

        std::shared_ptr<Snapshot> GetSnapshot() override
        {
            //TODO impl me
            raft_snapshot_t *result = (*get_snapshot)(state_);
            return std::shared_ptr<Snapshot>(result);
        }
        raft_state_machine_t()
        {
            //do nothing
        }

        raft_state_machine_t(const raft_state_machine_t &)
        {
            //TODO impl me
        }

        ~raft_state_machine_t() override
        {
            (*destructor_)(state_);
        }

        Status Apply(const CmdResult &cmd) override
        {
            char *data = const_cast<char *>(cmd.data.c_str());
            uint64_t index = cmd.index;
            uint64_t term = cmd.index;
            Status rep_s = cmd.replicate_status;
            void *tag = cmd.tag;
            uint16_t code = (*apply)(state_, data, cmd.data.size(), index, term, rep_s.code(), tag);
            return Status((Status::Code)code);
        }

        Status ApplyMemberChange(const ConfChange &cc, uint64_t index) override
        {
            FLOG_INFO("State machine ApplyMemberChange: conf[{}], index[{}]", cc.ToString(), index);
            raft_conf_change_t *reslut = new raft_conf_change_t;
            reslut->rep = cc;
            uint16_t code = (*apply_member_change)(state_, reslut, index);
            return Status((Status::Code)code);
        }

        Status ApplyReadIndex(const std::string &cmd, uint16_t verify_result) override
        {
            uint16_t code = (*apply_read_index)(state_, cmd.c_str(), verify_result);
            return Status((Status::Code)code);
        }

        uint64_t PersistApplied() override
        {
            uint64_t result = (*persisti_applied)(state_);
            FLOG_DEBUG("State machine PersistApplied:{}", result);
            return result;
        }

        void OnLeaderChange(uint64_t leader, uint64_t term) override
        {
            FLOG_INFO("State machine OnLeaderChange,leader[{}],term[{}]", leader, term);
            (*on_leader_change)(state_, leader, term);
        }

        Status ApplySnapshotStart(const std::string &context, uint64_t index) override
        {
            FLOG_INFO("State machine ApplySnapshotStart,index[{}]", index);
            uint16_t code = (*apply_snapshot_start)(state_, context.c_str(), context.size(), index);
            return Status((Status::Code)code);
        }
        Status ApplySnapshotData(const std::vector<std::string> &datas) override
        {
            FLOG_DEBUG("State machine ApplySnapshotData");
            const char **dataArray = new const char *[datas.size()];
            uint32_t dataNums = datas.size();

            uint32_t dataSizeList[dataNums];
            for (auto i = 0; i < datas.size(); ++i)
            {
                dataArray[i] = datas[i].c_str();
                dataSizeList[datas[i].size()];
            }

            uint16_t code = (*apply_snapshot_data)(state_, dataArray, dataNums, dataSizeList);
            return Status((Status::Code)code);
        }
        Status ApplySnapshotFinish(uint64_t index) override
        {
            FLOG_INFO("State machine ApplySnapshotFinish.index[{}]", index);
            uint16_t code = (*apply_snapshot_finish)(state_, index);
            return Status((Status::Code)code);
        }
    };

    raft_snapshot_t *raft_snapshot_create_t()
    {
        //TODO impl me
        return nullptr;
    }

    raft_state_machine_t *raft_state_machine_create(
        void *state_,
        void (*destructor_)(void *),
        uint16_t (*apply)(void *, const char *data, size_t data_len, const uint64_t index, uint64_t term, uint16_t rep_status, void *tag),
        uint16_t (*apply_member_change)(void *, const raft_conf_change_t *, uint64_t),
        uint16_t (*apply_read_index)(void *, const char *, uint16_t),
        uint64_t (*persisti_applied)(void *),
        void (*on_leader_change)(void *, uint64_t, uint64_t),
        raft_snapshot_t *(*get_snapshot)(void *),
        uint16_t (*apply_snapshot_start)(void *, const char *, uint32_t, uint64_t),
        uint16_t (*apply_snapshot_data)(void *, const char **dataArray, uint32_t dataNums, uint32_t *dataSizeList),
        uint16_t (*apply_snapshot_finish)(void *, uint64_t))
    {
        raft_state_machine_t *result = new raft_state_machine_t;
        result->state_ = state_;
        result->destructor_ = destructor_;
        result->apply = apply;
        result->apply_member_change = apply_member_change;
        result->apply_read_index = apply_read_index;
        result->persisti_applied = persisti_applied;
        result->on_leader_change = on_leader_change;
        result->get_snapshot = get_snapshot;
        result->apply_snapshot_start = apply_snapshot_start;
        result->apply_snapshot_data = apply_snapshot_data;
        result->apply_snapshot_finish = apply_snapshot_finish;
        return result;
    }

    void raft_free_raft_state_machine_t(raft_state_machine_t *sm)
    {
        free(sm);
    }

    //raft server related
    //create server
    raft_server_cache *raft_server_create(const raft_server_options_t &ops)
    {
        std::unique_ptr<RaftServer> rs = CreateRaftServer(ops.rep);
        assert(rs);
        auto s = rs->Start();
        assert(s.ok());
        raft_server_cache *result = new raft_server_cache;
        raft_server_t *server_t = new raft_server_t;
        server_t->rep = rs.release();
        result->rep = std::unique_ptr<raft_server_t>(server_t);
        return result;
    }

    void raft_free_raft_server_cache(raft_server_cache *cache)
    {
        free(cache);
    }

    //start server
    void raft_server_start(raft_server_cache &server)
    {
        server.rep->rep->Start();
    }

    //raft related
    raft_cache *raft_create(raft_server_cache &server, raft_options_t &ops)
    {
        std::shared_ptr<Raft> r;
        Status s = server.rep->rep->CreateRaft(ops.rep, &r);
        FLOG_INFO("raft_create status[{},{}]", s.code(), s.ToString());
        assert(s.ok());
        raft_cache *result = new raft_cache;
        result->rep = r;
        return result;
    }

    void raft_free_raft_cache(raft_cache *cache)
    {
        free(cache);
    }

    bool raft_is_stopped(raft_cache &raft)
    {
        return raft.rep->IsStopped();
    }

    bool raft_is_leader(raft_cache &raft)
    {
        return raft.rep->IsLeader();
    }

    raft_term_info *raft_get_leader_term(raft_cache &raft)
    {
        uint64_t leader = 0;
        uint64_t term = 0;
        raft.rep->GetLeaderTerm(&leader, &term);
        raft_term_info *info = new raft_term_info;
        info->leader = leader;
        info->term = term;
        return info;
    }

    void raft_free_raft_term_info(raft_term_info *info)
    {
        free(info);
    }

    uint64_t raft_get_leader_from_raft_term_info(raft_term_info *info)
    {
        return info->leader;
    }

    uint64_t raft_get_term_from_raft_term_info(raft_term_info *info)
    {
        return info->term;
    }

    jim_status_t *raft_try_to_leader(raft_cache &raft)
    {
        Status status = raft.rep->TryToLeader();
        jim_status_t *js = new jim_status_t;
        js->rep = status;
        return js;
    }

    void raft_propose(raft_cache &raft, char *data, size_t size, uint32_t flag, void *tag)
    {
        std::string s(data, size);
        raft.rep->Propose(s, flag, tag);
    }

    // remeber free it after use
    jim_status_t *raft_read_index(raft_cache &raft, char *ctx)
    {
        //TODO impl me
        return jim_status_create(1);
    }

    // remeber free it after use
    jim_status_t *raft_change_member(raft_cache &raft, const raft_conf_change_t &conf)
    {
        Status status = raft.rep->ChangeMemeber(conf.rep);
        jim_status_t *js = new jim_status_t;
        js->rep = status;
        return js;
    }

    void raft_truncate(raft_cache &raft)
    {
        //TODO impl me
    }

    raft_log_reader_t *raft_begin_read_log(raft_cache &raft, uint64_t start_index)
    {
        std::unique_ptr<LogReader> lr = raft.rep->ReadLog(start_index);
        raft_log_reader_t *ret = new raft_log_reader_t;
        ret->rep = std::move(lr);
        return ret;
    }

    void raft_end_read_log(raft_log_reader_t *log_reader)
    {
        free(log_reader);
    }

    // note: free data after use
    jim_status_t *raft_log_reader_next(raft_log_reader_t *log_reader, uint64_t *index, char **data, size_t *len, bool *over)
    {
        std::string _data;
        bool _over;
        uint64_t _index;

        Status status = log_reader->rep->Next(_index, _data, _over);
        size_t _len = _data.size();

        if (!_over)
        {
            char *x = (char *)malloc(_len);
            memcpy(x, _data.data(), _len);
            *data = x;
        }
        *index = _index;
        *len = _len;
        *over = _over;

        jim_status_t *js = new jim_status_t;
        js->rep = status;
        return js;
    }

    uint64_t raft_log_reader_last_index(raft_log_reader_t *log_reader)
    {
        return log_reader->rep->LastIndex();
    }
}
