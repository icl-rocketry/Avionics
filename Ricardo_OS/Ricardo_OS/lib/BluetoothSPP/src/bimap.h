#pragma once
//a lazy naive bidirectional unordered map ;p
//both keys and values must be unique

#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <optional>

template<typename KEY, typename VALUE>
class Bimap{
    public:
    
        Bimap(){};
        
        void set(KEY key,VALUE value){
            forwardMap[key] = value;
            reverseMap[value] = key;
        };
        void eraseByKey(KEY key){
            VALUE value = getValue(key).value_or("");
            forwardMap.erase(key);
            reverseMap.erase(value);
        };
        void eraseByValue(VALUE value){
            KEY key = getKey(value).value_or("");
            forwardMap.erase(key);
            reverseMap.erase(value);
        };
        void erase(KEY key, VALUE value){
            forwardMap.erase(key);
            reverseMap.erase(value);
        };
        void clear(){
            forwardMap.clear();
            reverseMap.clear();
        };
        int keyPresent(KEY key){
            return forwardMap.count(key);
        };
        int valuePresent(VALUE value){
            return reverseMap.count(value);
        };

        std::optional<VALUE> getValue(KEY key){
            try{
                return {forwardMap.at(key)};
            }
            catch(std::out_of_range const&){
                return {};
            }
        };
        std::optional<KEY> getKey(VALUE value){
            try{
                return {reverseMap.at(value)};
            }
            catch(std::out_of_range const&){
                return {};
            }
        };


        // auto deduction of iterator types to allow range based for loops
        auto begin(){
            return forwardMap.begin();
        }
        auto end(){
            return forwardMap.end();
        }

        size_t size(){
            size_t forwardSize = forwardMap.size();
            size_t reverseSize = reverseMap.size();
            if (forwardSize != reverseSize){
                throw "Sizes do not match"; 
                return 0;
            }else{
                return forwardSize;
            }
        };
    private:
        //private to prevent unintentional modification which would cause the maps to be out of sync
        std::unordered_map<KEY,VALUE> forwardMap;
        std::unordered_map<VALUE, KEY> reverseMap;



           
};