#pragma once

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "stubs.h"

/**
 * @brief Templated dynamic handler which manages objects of type T 
 * It is intended that methos are accessed only by the derived class
 * so crtp will be used to remove the overhead of virtual dispatch.
 * 
 * Handled Objects must implement a getID function 
 * 
 * 
 * 
 * @tparam T 
 */

template<class Object,class Derived>
class DynamicHandlerBase{
    public:
        DynamicHandlerBase(LogController& logcontroller):
        _logcontroller(logcontroller)
        {};

        /**
         * @brief Returns a non-owning pointer to a stored object
         * 
         * @param objectID 
         * @return T* 
         */
        Object* getObject(size_t objectID){
            try{
                Object* obj = _objectList.at(objectID).get();
                // Check id
                if (obj->getID() != objectID){
                    throw std::runtime_error("Object ID: "+std::to_string(obj->getID()) +" doesn't match index ID: "+ std::to_string(objectID));
                }
                return obj;
            }catch (const std::out_of_range& e){
                throw std::runtime_error("Object ID out of range, Object ID:" + std::to_string(objectID) + " size:" + std::to_string(_objectList.size()));
            }
        };

        auto begin(){
            return _objectList.begin();
        }
        auto end(){
            return _objectList.end();
        }

        size_t size(){return _objectList.size();};
        
    protected:

        template <class... Args>
        void addObject(Args&&... args){
            _objectList.emplace_back(std::forward<Args>(args)... );
        };
        
        LogController& _logcontroller;

    private:
        /**
         * @brief Container for constructed objects 
         * vector is private to prevent deletion of elements as these objects should not be delted during rocket runtime
         * they represent critical compontens of the rocket. We can use a unique ptr with non owning pointers as the container is private
         * so acessing the object has to be done through the getObject method. This means a network callback has to call the object thruogh getObject and hence
         * we can check if the object exists meaning no UB yay!
         */
        std::vector<std::unique_ptr<Object>> _objectList;

};
