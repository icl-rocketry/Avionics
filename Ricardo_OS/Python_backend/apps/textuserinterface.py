from codecs import decode
from math import ceil
import redis
import multiprocessing
import time
import json
import urwid
import argparse
import os
import signal
import sys
import time
from datetime import datetime
from urwid.widget import CENTER, LEFT, RIGHT, Widget
import random

flag_lookup = [([0,0])] * 32 #assing list of 32 elements to represent 32 bits in flag string
#list structure [flag level, flag name]
flag_lookup[0] = ['state','DEBUG']
flag_lookup[1] = ['state','PREFLIGHT']
flag_lookup[2] = ['state','LAUNCH']
flag_lookup[3] = ['state','FLIGHT']
flag_lookup[4] = ['state','RECOVERY']
flag_lookup[5] = ['state','SETUP']
flag_lookup[6] = ['state','GROUNDSTATION']
flag_lookup[7] = ['info','DEBUG']
flag_lookup[8] = ['error','SPI']
flag_lookup[9] = ['error','I2C']
flag_lookup[10] = ['error','SERIAL']
flag_lookup[11] = ['error','LORA']
flag_lookup[12] = ['error','BARO']
flag_lookup[13] = ['error','BATT']
flag_lookup[14] = ['error','GPS']
flag_lookup[15] = ['error','IMU']
flag_lookup[16] = ['error','ESTIMATOR']
flag_lookup[17] = ['error','SD']
flag_lookup[18] = ['error','FLASH']
flag_lookup[19] = ['error','CAN']
flag_lookup[20] = ['error','ORIENTATION']
flag_lookup[21] = ['warn','BATT']
flag_lookup[22] = ['warn','PYRO1']
flag_lookup[23] = ['warn','PYRO2']
flag_lookup[24] = ['flightphase','BOOST']
flag_lookup[25] = ['flightphase','COAST']
flag_lookup[26] = ['flightphase','APOGEE']
flag_lookup[27] = ['flightphase','DROGUE CHUTE']
flag_lookup[28] = ['flightphase','MAIN CHUTE']

#loading screen view
class LoadingView(urwid.WidgetWrap):
    def __init__(self):
        
        self.logotext = [('logo_blue',u"██╗ ██████╗██╗     "),('logo_red',u"██████╗ \n"),('logo_blue',u"██║██╔════╝██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")]
        self.logo = urwid.Text(self.logotext,align=CENTER)

        #stores the top widget
        self.top = urwid.Filler(self.logo)
        super().__init__(self.top)

    

#main screen view
class MainView(urwid.WidgetWrap):
    def __init__(self,redishost,redisport):
        #stores the top widget
        self.systemstatus = SystemStatus()
        self.telmetryview = TelemetryView()
        self.tophalf = urwid.Columns([self.telmetryview,self.systemstatus])

        self.errorlog = ErrorLog()
        self.righthalf = urwid.Pile([self.tophalf,self.errorlog])

        self.errorstatus = ErrorStatus()
        self.body = urwid.Columns([self.errorstatus,('weight',3,self.righthalf)])

        self.statusbar = StatusBar(redishost,redisport)
        
        self.top = urwid.Frame(header=self.statusbar,body=self.body)

 
        super().__init__(self.top)

    def update(self,data):

        
        self.statusbar.update(data)
        self.errorstatus.update(data)
        self.errorlog.update(data)
        self.systemstatus.update(data)
        self.telmetryview.update(data)
        


class StatusBar(urwid.WidgetWrap):
    def __init__(self,redishost,redisport):
        
        self.hostportString = str(redishost) + ":" + str(redisport)
        
        self.hostport = urwid.Text(self.hostportString)
   
        self.time = urwid.Text(datetime.now().strftime("%H:%M:%S"),align=CENTER)
        self.exithint = urwid.Text(u'<q> to exit',align=RIGHT)

        self.top = urwid.AttrMap(urwid.Columns([self.hostport,self.time,self.exithint]),'status_bar')
        super().__init__(self.top)

    def update(self,data):
        connected = data.get("connectionstatus",False)
        #connected = True # just for testing
        self.time.set_text(datetime.now().strftime("%H:%M:%S"))
        if connected:
            self.top.set_attr_map({None:"status_bar"}) 
        else:
            self.top.set_attr_map({None:'status_bar_no_data'})
            self.time.set_text(datetime.now().strftime("%H:%M:%S") + ' - NO DATA')

class ErrorStatus(urwid.WidgetWrap):
    def __init__(self):
        self.errorlist= [item[1] for item in flag_lookup if item[0] is 'error']
        self.elements = [urwid.AttrMap(urwid.Filler(urwid.Text(text,align=CENTER)),'nominal') for text in self.errorlist]
        
        self.body = urwid.Pile(self.elements)
        self.top = urwid.LineBox(self.body,'Status',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        allEvents = decodeSystemStatus(data.get("system_status",0))
        errorEvents = [item[1] for item in allEvents if item[0] is 'error']
        #update any errors passed
        for idx in range(len(self.errorlist)):
            item = self.errorlist[idx]
            if item in errorEvents:
                self.elements[idx].set_attr_map({None:'error'})
            else:
                self.elements[idx].set_attr_map({None:'nominal'})

class ErrorLog(urwid.WidgetWrap):
    def __init__(self):
        self.prevSystemStatus :int= 0
        self.errorlist = urwid.SimpleFocusListWalker([])
        self.errorlistbox = urwid.ListBox(self.errorlist)        
        self.top = urwid.LineBox(self.errorlistbox,'Log',title_align='left')
        super().__init__(self.top)
        
    def update(self,data): #method compares new system status to old system status and displays when there is a change
        current_time = datetime.now().strftime("%H:%M:%S.%f")[:-3]

        currentSystemStatus : int = data.get("system_status",0)
        changedStatus = currentSystemStatus ^ self.prevSystemStatus#xor to find changed status ( returns a 1 if x and y are different and zero if they are the same)
        flagTurnedOn = changedStatus&currentSystemStatus #  we 'and' the changed status and the current system status so see which flags have just turned on
        flagTurnedOff = changedStatus&self.prevSystemStatus 
        flagListOn = decodeSystemStatus(flagTurnedOn)
        flagListOff = decodeSystemStatus(flagTurnedOff)
        
        for item in flagListOn:
            error_level :str = item[0]
            attribute = 'log_' + error_level
            error_string = "["+current_time+"] -> " + error_level.upper() +": " + item[1] + " Flag Raised"
            self.errorlist.append(urwid.AttrMap(urwid.Text(error_string),attribute))
            self.__setToBottom__()

        for item in flagListOff:
            error_level :str = item[0]
            attribute = 'log_' + error_level
            error_string = "["+current_time+"] -> " + error_level.upper() +": " + item[1] + " Flag Removed"
            self.errorlist.append(urwid.AttrMap(urwid.Text(error_string),attribute))
            self.__setToBottom__()

        self.prevSystemStatus = currentSystemStatus

    def addLog(self,string): #public interface to add entry to log
        current_time = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        self.errorlist.append(urwid.AttrMap(urwid.Text(string),None))
        self.__setToBottom__()


    def __setToBottom__(self):#helper function to ensure lisbox lists from bottom up
        self.errorlist.set_focus(self.errorlist.positions(reverse=True)[0])

class TelemetryView(urwid.WidgetWrap):
    def __init__(self):

        self.displayStrings = ["GPS (Lat,Long,Alt) : ",
                               "Position NED (m) : ",
                               "Velocity NED (m/s) : ",
                               "Acceleration NED (m/s^2) : ",
                               "Orientation (Roll,Pitch,Yaw) (Rad) : ",
                               "Baro Temperature (Celcius) : ",
                               "Baro Pressure (Pa) : ",
                               "Baro Altitude (m) : "]
        self.displayWidgetsList = [urwid.Filler(urwid.Text(text,align="center")) for text in self.displayStrings]

        self.displayWidgets = urwid.Pile(self.displayWidgetsList)
        self.top = urwid.LineBox(self.displayWidgets,'Telemetry',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        self.__updateDisplayData__(0,"("+self.__getFromDict__(data,"lat") + "," + self.__getFromDict__(data,"lng") + "," + self.__getFromDict__(data,"alt") + ")" )
        self.__updateDisplayData__(1,"("+self.__getFromDict__(data,"pn") + "," + self.__getFromDict__(data,"pe") + "," + self.__getFromDict__(data,"pd") + ")" )
        self.__updateDisplayData__(2,"("+self.__getFromDict__(data,"vn") + "," + self.__getFromDict__(data,"ve") + "," + self.__getFromDict__(data,"vd") + ")" )
        self.__updateDisplayData__(3,"("+self.__getFromDict__(data,"an") + "," + self.__getFromDict__(data,"ae") + "," + self.__getFromDict__(data,"ad") + ")" )
        self.__updateDisplayData__(4,"("+self.__getFromDict__(data,"roll") + "," + self.__getFromDict__(data,"pitch") + "," + self.__getFromDict__(data,"yaw") + ")" )
        self.__updateDisplayData__(5,"("+self.__getFromDict__(data,"baro_temp")  + ")" )
        self.__updateDisplayData__(6,"("+self.__getFromDict__(data,"baro_press")  + ")" )
        self.__updateDisplayData__(7,"("+self.__getFromDict__(data,"baro_alt")  + ")" )

    def __updateDisplayData__(self,idx,text):
        self.displayWidgetsList[idx].base_widget.set_text(self.displayStrings[idx] + text)

    def __getFromDict__(self,data,key,specifier='{:.4f}'):
        ret = data.get(key)
        if ret is None:
                return "NULL"
        else:
            return specifier.format(ret)
class SystemStatus(urwid.WidgetWrap):
    def __init__(self):
        self.displayStrings = ["StateMachine State : ",
                               "Flight Phase : ",
                               "System Time (ms) : ",
                               "RSSI (dBm) : ",
                               "SNR (dB) : ",
                               "Battery Voltage (mv) : "
                               ]
        self.displayWidgetsList = [urwid.Filler(urwid.Text(text,align="center")) for text in self.displayStrings]
        #self.dataWidgetList = [urwid.Filler(urwid.Text(" NULL",align=LEFT)) for all in self.displayStrings] 

        #add battery percentage bar
        self.batteryPercentText = urwid.Filler(urwid.Text("Battery Percentage",align="right"))
        self.batteryPercentBar = urwid.ProgressBar('error','nominal')
        self.batteryPercentBar.set_completion(50)
       
        #self.batteryPercent = urwid.Pile([urwid.Filler(self.batteryPercentText),urwid.Filler(self.batteryPercentBar)])
        self.batteryPercentWidget = urwid.Columns([self.batteryPercentText,
                                                   urwid.Padding(urwid.Filler(self.batteryPercentBar),width=('relative',50),left=1)
                                                   ])
        self.displayWidgetsList.append(self.batteryPercentWidget)

        self.displayWidgets = urwid.Pile(self.displayWidgetsList)
        #self.dataWidgets = urwid.Pile(self.dataWidgetList)
        #self.main = urwid.Columns([self.displayWidgets,self.dataWidgets])
        self.top = urwid.LineBox(self.displayWidgets,'System',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        systemStatusList : int= decodeSystemStatus(data.get("system_status",0))
        #these lists should only have 1 item each. if not, someone has meesed up the flag defintions either locally or on ricardo
        statemachineState = [item[1] for item in systemStatusList if item[0] is 'state']
        flightPhase = [item[1] for item in systemStatusList if item[0] is 'flightphase']

        self.__updateDisplayData__(0,str(self.__getFromList__(statemachineState,"NULL")))
        self.__updateDisplayData__(1,str(self.__getFromList__(flightPhase,"NULL")))
        self.__updateDisplayData__(2,"("+ self.__getFromDict__(data,"system_time",'{:.0f}')  + ")" )
        self.__updateDisplayData__(3,"("+ self.__getFromDict__(data,"rssi",'{:.0f}')  + ")" )
        self.__updateDisplayData__(4,"("+ self.__getFromDict__(data,"snr",'{:.2f}') + ")" )
        self.__updateDisplayData__(5,"("+ self.__getFromDict__(data,"batt_voltage",'{:.0f}') + ")" )
        self.batteryPercentBar.set_completion(data.get("batt_percent",0))

    def __getFromDict__(self,data,key,specifier='{:.4f}'):
        ret = data.get(key)
        if ret is None:
                return "NULL"
        else:
            return specifier.format(ret)
    
    def __getFromList__(self,l,default):
        try:
            ret = ''
            for flag in l:
                ret += flag + " " #display all flags even if there are multiple -> to let user know someone has messed up
        except:
            ret = default
        return ret   

    def __updateDisplayData__(self,idx,text):
        self.displayWidgetsList[idx].base_widget.set_text(self.displayStrings[idx] + text)
class TextUserInterface(multiprocessing.Process):
    def __init__(self,redishost = 'localhost',redisport = 6379,refreshRate = .2):
        super(TextUserInterface,self).__init__()
        
        self.loadingScreen :LoadingView = LoadingView()
        self.mainScreen :MainView = MainView(redishost,redisport)

        self.refreshRate = refreshRate
        self.palette = {
            (None,'light gray','','','#0c272f','#0B1518'),#default
            ('status_bar','black','light gray','','#0b1518','#dbdbdb'),
            ('status_bar_no_data','light gray','dark red','','#dbdbdb','#ff0000'),
            ('error','light gray','dark red','blink','#0B1518','#ff0000'),
            ('nominal','light gray','dark green','','#0B1518','#20ff00'),
            ('logo_blue','dark blue','','','#001240','#0B1518'),
            ('logo_red','dark red','','','#ff0000','#0B1518'),
            ('logo_white','white','','','#ffffff','#0B1518'),
            ('highlight','light gray','dark red','blink','#0c272f','#ff0000'),
            ('log_error','dark red','','','#ff0000','#0B1518'),
            ('log_info','light cyan','','','#00ffff','#0B1518'),
            ('log_flightphase','light magenta','','','#f003fc','#0B1518'),
            ('log_warn','yellow','','','#ffd900','#0B1518'),
            ('log_state','light blue','','','#1500ff','#0B1518')
        }
        self.uiLoop = urwid.MainLoop(self.loadingScreen,palette=self.palette,unhandled_input=self.__exit_on_q__)
        #self.uiLoop.screen.set_terminal_properties(colors=1)
        self.uiLoop.set_alarm_in(1,self.__transitionToMain__)
        #self.r = None
        self.systemstatus = 0
        self.count = 0
        self.r = redis.Redis(host=redishost,port=redisport)
        self.exit_event = multiprocessing.Event()


    def run(self):
        self.uiLoop.run()
    
    def stop(self): #potentially unused
        raise urwid.ExitMainLoop()
        
    def __updateData__(self,main,user_data):
        self.mainScreen.update(self.__getTelemetry__())
        self.uiLoop.set_alarm_in(self.refreshRate,self.__updateData__)#set next alarm at refresh rate 

    def __getTelemetry__(self):
        #for demo purposes
        # if self.count%5 == 0:
        #     self.systemstatus = random.randint(0,2**28)
        # self.count += 1 


        try:
            data = json.loads(self.r.get("telemetry"))
            #data["connectionstatus"] = True
        except:
            data = {"connectionstatus":False}
        # data["system_status"] = self.systemstatus
        return data

   

    def __exit_on_q__(self,key):
        if key in ('q', 'Q'):
            raise urwid.ExitMainLoop()

    def __transitionToMain__(self,main,data):
        self.uiLoop.widget = self.mainScreen #update main widget to the main view
        self.uiLoop.set_alarm_in(0,self.__updateData__)#call update function in event loop
   
    def __connected__(self):
        try:
            self.r.ping()
            return True
        except redis.exceptions.ConnectionError:
            return False
 
def decodeSystemStatus(SystemStatusVariable): #decodes system status variable returns list of events
    binaryString = "{0:b}".format(SystemStatusVariable)[::-1]#get binary representation and reverse
    return [flag_lookup[idx] for idx in range(len(binaryString)) if binaryString[idx] is '1']

def checkRedis():
    try:
        server = redis.Redis(host=args["redis_host"],port=args["redis_port"])
        server.ping()
    except redis.exceptions.ConnectionError:
        errormsg = "[ERROR] -> Redis server not found at host:'" + args["redis_host"] + "' port:" + str(args["redis_port"]) + "\nPlease check redis is running"
        sys.exit(errormsg)





if __name__ == '__main__':
    #allow standalone opeartion
    ap = argparse.ArgumentParser()
    ap.add_argument("--redis-host", required=False, help="redis host", type=str,default = "localhost")
    ap.add_argument("--redis-port", required=False, help="redis port", type=int,default = 6379)
    ap.add_argument("--refresh", required=False, help="data refresh rate", type=float,default = .2)
    args = vars(ap.parse_args())

    checkRedis()#check redis server exists

    TUIinstance = TextUserInterface(redishost=args['redis_host'],redisport=args['redis_port'],refreshRate=args['refresh'])
    TUIinstance.run() #call run instad of start as we dont need a new process