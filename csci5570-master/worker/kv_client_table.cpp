#include "worker/kv_client_table.hpp"

namespace csci5570{
    KVClientTable::KVClientTable(uint32_t app_thread_id, uint32_t model_id, ThreadsafeQueue<Message>* const sender_queue,
        const AbstractPartitionManager* const partition_manager, AbstractCallbackRunner* const callback_runner)
        : app_thread_id_(app_thread_id),
          model_id_(model_id),
          sender_queue_(sender_queue),
          partition_manager_(partition_manager),
          callback_runner_(callback_runner){
      }

    void KVClientTable::Add(const third_party::SArray<Key>& keys,const third_party::SArray<Val>& vals) {
        KVPairs kvtmp=std::make_pair(keys,vals);
        std::vector<std::pair<int,KVPairs> sliced;
        partition_manager_.Slice(kvtmp,&sliced);
        uint32_t count=0;
        while(count<sliced.size()){
          Message m;
          third_party::SArray<char> datatmp;
          datatmp.push_back(sliced[count].second.first);
          datatmp.push_back(sliced[count].second.second);
          m.data.push_back(datatmp);
          count++;
          m.meta.sender=app_thread_id_;
          m.meta.recver=sliced[count].first;
          m.meta.flag=Flag::kAdd;
          m.model_id=model_id_;
          sender_queue_.push(m);
        }     
      }     
    void KVClientTable::Get(const third_party::SArray<Key>& keys, third_party::SArray<Val>* vals){}
}