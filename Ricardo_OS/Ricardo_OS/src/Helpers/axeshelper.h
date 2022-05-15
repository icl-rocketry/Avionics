#pragma once
/**
 * @file axesHelper.h
 * @author Kiran de Silva
 * @brief templated axes helper to standardize and configure orientation and flipping of sensor frame axes
 * to body frame axes
 * @version 0.1
 * @date 2022-05-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <array>
#include <algorithm>
#include <exception>

template<size_t N=3,bool AXESHELPER_NOEXCEPT =false>
class AxesHelper{
    public:
        AxesHelper(){};
        /**
         * @brief Set the Order of axes
         * 
         * @param axesOrder 
         */
        void setOrder(const std::array<uint8_t,N>& axesOrder){
             if (*std::max_element(axesOrder.begin(),axesOrder.end()) >= N){
                 if constexpr(AXESHELPER_NOEXCEPT){
                     return;
                 }else{
                    throw std::runtime_error("Order Index Out Of Bounds!");
                 }
             }
            _axesOrder = axesOrder;
            };

        /**
         * @brief Set the flips of the axes
         * 
         * @param axesFlip 
         */
        void setFlip(const std::array<bool,N>& axesFlip){
            for (size_t i = 0;i<N; i++){
                _axesFlipConstant[i] = (axesFlip[i]) ? -1 : 1;
            }
            };

        /**
         * @brief Get a transformed axis from an array of axes
         *
         * @tparam T 
         * @param data 
         * @param idx 
         * @return T 
         */
        template<uint8_t IDX,typename T>
        T operator[](const std::array<T,N>& data){
            static_assert(IDX<N,"Index Out of Bounds!");
            return data[_axesOrder[IDX]]*_axesFlipConstant[IDX];
        };

        /**
         * @brief Transform an array of axes 
         * 
         * @tparam T 
         * @param data 
         * @return std::array<T,N> 
         */
        template<typename T>
        std::array<T,N> operator()(const std::array<T,N>& data){
            std::array<T,N> ret;
            assign_axes<N,T>(ret,data);
            return ret;
        };

    private:
        std::array<uint8_t,N> _axesOrder{0,1,2};
        std::array<int8_t,N> _axesFlipConstant{1,1,1};

        template<size_t I,typename T>
        void assign_axes(std::array<T,N>& arr,const std::array<T,N>& data)
        {
            if constexpr(I > 0){
                arr[I-1] = operator[]<I-1,T>(data);
                assign_axes<I-1,T>(arr,data);
            }
        };

};