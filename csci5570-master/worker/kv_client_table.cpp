#include "worker/kv_client_table.hpp"

namespace csci5570{


    void KVClientTable::Add(const third_party::SArray<Key>& keys, const third_party::SArray<Val>& vals) {
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