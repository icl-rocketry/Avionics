Eigen3.4.rc1 Library from https://gitlab.com/libeigen/eigen.git
The original Library can be found under lib/unused

Modifed to work with the anoying arduino framework 
***** multiple files have been modifed some inside the arudino framework *****
1. Eigen/src/Core/util/Meta.h Line 502 
    From:
        template<typename F, typename... ArgTypes>
        struct result_of<F(ArgTypes...)> {
            typedef typename std::invoke_result<F, ArgTypes...>::type type1;
            typedef typename remove_all<type1>::type type;
        };
    To:
        template<typename F_EIGEN, typename... ArgTypes>
        struct result_of<F_EIGEN(ArgTypes...)> {
            typedef typename std::invoke_result<F_EIGEN, ArgTypes...>::type type1;
            typedef typename remove_all<type1>::type type;
        };
    This is due to the F macro in the arudino framework.

2. framework-arduinoespressif32-src-.../cores/esp32/binary.h Line 31
    From:
        #define B1 1
    To: 
        #define _B1 1

3. Copy the Eigen directory into a seperate eigen folder as platformio will try to compile everything
in the original library as there is no library.json.