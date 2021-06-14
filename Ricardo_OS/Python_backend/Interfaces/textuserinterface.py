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
        self.statusbar.update()

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
        self.errorlist = urwid.ListBox([])
        self.top = urwid.LineBox(self.errorlist,'Status',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        pass

class ErrorLog(urwid.WidgetWrap):
    def __init__(self):
        self.errorlist = urwid.ListBox([])
        self.top = urwid.LineBox(self.errorlist,'Log',title_align='left')
        super().__init__(self.top)
        
    def update(self,data):
        pass

class TelemetryView(urwid.WidgetWrap):
    def __init__(self):
        self.body = urwid.Text('telemetry',align=CENTER)
        self.errorlist = urwid.Filler(self.body)
        self.top = urwid.LineBox(self.errorlist,'Telemetry',title_align='left')
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
            ('error','dark red','','blink','#ff0000','#0B1518'),
            ('nominal','light green','','','#20ff00','#0B1518'),
            ('logo_blue','dark blue','','','#001240','#0B1518'),
            ('logo_red','dark red','','','#ff0000','#0B1518'),
            ('logo_white','white','','','#ffffff','#0B1518')
        }
        self.uiLoop = urwid.MainLoop(self.loadingScreen,palette=self.palette,unhandled_input=self.__exit_on_q__)
        #self.uiLoop.screen.set_terminal_properties(colors=256)
        self.uiLoop.set_alarm_in(3,self.__transitionToMain__)
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
 
def decodeSystemStatus(SystemStatusVariable): #decodes system status variable
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