// Copyright 2018 Evandro Luis Copercini
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "sdkconfig.h"

#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BLUEDROID_ENABLED)

#include "Arduino.h"
#include "Stream.h"
#include <esp_gap_bt_api.h>
#include <esp_spp_api.h>
#include <functional>
#include "BTScan.h"

#include <string>

typedef std::function<void(const uint8_t *buffer, size_t size)> BluetoothSPPDataCb;
typedef std::function<void(uint32_t num_val)> ConfirmRequestCb;
typedef std::function<void(boolean success)> AuthCompleteCb;
typedef std::function<void(BTAdvertisedDevice* pAdvertisedDevice)> BTAdvertisedDeviceCb;

class seg_header{ // segmentation header with basic serializaiton and deserialization 
    public:
        uint8_t seg_id; // segment id -> this counts up with each recieved segment of the full packet -> zero indicates the start of a new segment
        size_t len; // total expected length

        seg_header(size_t _len=0):
        seg_id(0),
        len(_len)
        {};
        seg_header(uint8_t* data, size_t offset=0){
            seg_id = (uint8_t)*(data+offset);
            memcpy(&len,data+offset+1,sizeof(size_t));
        }
        void serialize(uint8_t* buf,size_t offset=0){
            memcpy(buf+offset,&seg_id,sizeof(uint8_t));
            memcpy(buf+offset+sizeof(uint8_t),&len,sizeof(size_t));
        }
        constexpr size_t size(){return sizeof(uint8_t)+sizeof(size_t);};
};

struct recieved_packet_t{
    seg_header segHeader;
    std::string bt_address;
    std::vector<uint8_t> data; 
};

class BluetoothSPP
{
    public:

        BluetoothSPP();
        ~BluetoothSPP();

        bool begin(std::string localName="ESP32", bool isMaster=false);
        // bool begin(unsigned long baud){//compatibility
        //     return begin();incinclu#
        // }
        int available(void);
        // int peek(void);
        bool hasClient(void);
        // int read(void);

        // size_t write(uint8_t c);
        // size_t write(const uint8_t *buffer, size_t size);

        void flush();
        void end(void);
        // void onData(BluetoothSPPDataCb cb);
        // esp_err_t register_callback(esp_spp_cb_t * callback);
        
        void onConfirmRequest(ConfirmRequestCb cb);
        void onAuthComplete(AuthCompleteCb cb);
        void confirmReply(bool confirm);

        void enableSSP();
        bool setPin(const char *pin);
        // bool connect(String remoteName);
        bool connect(uint8_t remoteAddress[]);
        bool connect(std::string remoteAddress);
        // bool connect();
        bool connected(int timeout=0);

        bool isReady(bool checkMaster=false, int timeout=0);

        bool disconnect();
        bool disconnect(uint8_t remoteAddress[]);
        bool disconnect(std::string remoteAddress);

        bool unpairDevice(uint8_t remoteAddress[]);

        recieved_packet_t readPacket();
        bool sendPacket(std::vector<uint8_t> &data, std::string bt_address);



        // BTScanResults* discover(int timeout=0x30*1280);
        // bool discoverAsync(BTAdvertisedDeviceCb cb, int timeout=0x30*1280);
        // void discoverAsyncStop();
        // void discoverClear();
        // BTScanResults* getScanResults();
        
        // const int INQ_TIME = 1280;   // Inquire Time unit 1280 ms
        // const int MIN_INQ_TIME = (ESP_BT_GAP_MIN_INQ_LEN * INQ_TIME);
        // const int MAX_INQ_TIME = (ESP_BT_GAP_MAX_INQ_LEN * INQ_TIME);
        
        // operator bool() const;

    private:
        std::string _localName;

      
      



};

#endif

