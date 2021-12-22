#pragma once

/**
 * @brief Exponential Moving Average Filter 
 * 
 */
class BaroFilter{

    public:
        /**
         * @brief Construct a new Baro Filter object 
         * 
         * @param alpha exponential weighting
         */
        explicit BaroFilter(float alpha);

        /**
         * @brief Get filtered altitude reading
         * 
         * @param alt - altitude in m
         * @return float 
         */
        float getFilterAlt(float alt);
      

    private:
        const float _alpha;
        
        float _prevFilterAlt;

};