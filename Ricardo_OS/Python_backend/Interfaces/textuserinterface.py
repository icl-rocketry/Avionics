from codecs import decode
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
flag_lookup[0] = ['state','USBMODE']
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
flag_lookup[24] = ['info','BOOST']
flag_lookup[25] = ['info','COAST']
flag_lookup[26] = ['info','APOGEE']
flag_lookup[27] = ['info','DROGUE CHUTE']
flag_lookup[28] = ['info','MAIN CHUTE']

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
        self.count = 0
        self.data = {"systemstatus":0}
        super().__init__(self.top)

    def update(self,data):
        if self.count%5 == 0:
            self.data["systemstatus"] = random.randint(0,2**28)
        
        self.statusbar.update()
        self.errorstatus.update(self.data)
        self.errorlog.update(self.data)
        self.count += 1

class StatusBar(urwid.WidgetWrap):
    def __init__(self,redishost,redisport):
        
        self.hostportString = str(redishost) + ":" + str(redisport)
        self.hostport = urwid.Text(('status_bar',self.hostportString))
        self.time = urwid.Text(('status_bar',datetime.now().strftime("%H:%M:%S")),align=CENTER)
        self.exithint = urwid.Text(('status_bar',u'<q> to exit'),align=RIGHT)
        self.top = urwid.AttrMap(urwid.Columns([self.hostport,self.time,self.exithint]),'status_bar')
        super().__init__(self.top)

    def update(self):
        self.time.set_text(('status_bar',datetime.now().strftime("%H:%M:%S")))

class ErrorStatus(urwid.WidgetWrap):
    def __init__(self):
        self.subsystemlist= [item[1] for item in flag_lookup if item[0] is 'error']
        #self.listwalker = urwid.SimpleFocusListWalker([urwid.Text(text) for text in self.subsystemlist])
        self.elements = [urwid.AttrMap(urwid.Filler(urwid.Text(text,align=CENTER)),'nominal') for text in self.subsystemlist]
        
        #self.decorated[0].set_attr_map({None:'error'})
        self.body = urwid.Pile(self.elements)
        self.top = urwid.LineBox(self.body,'Status',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        allEvents = decodeSystemStatus(data["systemstatus"])
        errorEvents = [item[1] for item in allEvents if item[0] is 'error']
        #update any errors passed
        for idx in range(len(self.subsystemlist)):
            item = self.subsystemlist[idx]
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

        currentSystemStatus : int= data["systemstatus"]
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
       
       
        

    def __setToBottom__(self):#helper function to ensure lisbox lists from bottom up
        self.errorlist.set_focus(self.errorlist.positions(reverse=True)[0])

class TelemetryView(urwid.WidgetWrap):
    def __init__(self):
        self.body = urwid.Text('telemetry',align=CENTER)
        self.main = urwid.Filler(self.body)
        self.top = urwid.LineBox(self.main,'Telemetry',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        pass

class SystemStatus(urwid.WidgetWrap):
    def __init__(self):
        self.body = urwid.Text('system',align=CENTER)
        self.errorlist = urwid.Filler(self.body)
        self.top = urwid.LineBox(self.errorlist,'System',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        pass

class TextUserInterface(multiprocessing.Process):
    def __init__(self,redishost = 'localhost',redisport = 6379,refreshRate = .2):
        super(TextUserInterface,self).__init__()
        
        self.loadingScreen :LoadingView = LoadingView()
        self.mainScreen :MainView = MainView(redishost,redisport)

        self.refreshRate = refreshRate
        self.palette = {
            (None,'light gray','','','#0c272f','#0B1518'),#default
            ('status_bar','black','light gray','','#0b1518','#dbdbdb'),
            ('error','light gray','dark red','blink','#0B1518','#ff0000'),
            ('nominal','light gray','dark green','','#0B1518','#20ff00'),
            ('logo_blue','dark blue','','','#001240','#0B1518'),
            ('logo_red','dark red','','','#ff0000','#0B1518'),
            ('logo_white','white','','','#ffffff','#0B1518'),
            ('highlight','light gray','dark red','blink','#0c272f','#ff0000'),
            ('log_error','dark red','','','#ff0000','#0B1518'),
            ('log_info','light cyan','','','#00ffff','#0B1518'),
            ('log_warn','yellow','','','#ffd900','#0B1518'),
            ('log_state','light blue','','','#1500ff','#0B1518')
        }
        self.uiLoop = urwid.MainLoop(self.loadingScreen,palette=self.palette,unhandled_input=self.__exit_on_q__)
        #self.uiLoop.screen.set_terminal_properties(colors=256)
        self.uiLoop.set_alarm_in(1,self.__transitionToMain__)
        self.r = None
        #self.r = redis.Redis(host=redishost,port=redisport)
        self.exit_event = multiprocessing.Event()


    def run(self):
        self.uiLoop.run()
    
    def stop(self): #potentially unused
        raise urwid.ExitMainLoop()
        
    def __updateData__(self,main,data):
        self.mainScreen.update(data)
        self.uiLoop.set_alarm_in(self.refreshRate,self.__updateData__)#set next alarm at refresh rate 

    def __getTelemetry__(self):
        return self.r.get("telemetry")
   

    def __exit_on_q__(self,key):
        if key in ('q', 'Q'):
            raise urwid.ExitMainLoop()

    def __transitionToMain__(self,main,data):
        self.uiLoop.widget = self.mainScreen #update main widget to the main view
        self.uiLoop.set_alarm_in(0,self.__updateData__)#call update function in event loop
 
def decodeSystemStatus(SystemStatusVariable): #decodes system status variable returns list of events
    binaryString = "{0:b}".format(SystemStatusVariable)[::-1]#get binary representation and reverse
    return [flag_lookup[idx] for idx in range(len(binaryString)) if binaryString[idx] is '1']


    pass

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